/*!
  \file                TelescopeAnalysis.cc
  \brief               Example user code for Analysis
  \author              Suvankar Roy Chowdhury, Rajarshi Bhattacharya
  \date                05/07/16
  Support :            mail to : suvankar.roy.chowdhury@cern.ch, rajarshi.bhattacharya@cern.ch
*/
#include "TROOT.h"
#include "TInterpreter.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TKey.h"
#include "TClass.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include <map>
#include <utility>
#include <vector>
#include <sstream>

#include "TelescopeAnalysis.h"
using std::vector;
using std::map;
TelescopeAnalysis::TelescopeAnalysis(const std::string inFilename,const std::string outFilename) :
  BeamAnaBase::BeamAnaBase(),
  outFile_(outFilename)
{
  std::cout << "EDM Tuple Analyzer initialized with the following options\n" 
            << "Infile: " << inFilename
            << "\nOutFile: " << outFile_
            << std::endl; 
  if( setInputFile(inFilename) == 0 ) {
    std::cout << "Empty Chain!!";
    exit(1);
  }
  nEntries_ = analysisTree()->GetEntries();
  hist_ = new Histogrammer(outFile_);
  beginJob();
  
}
void TelescopeAnalysis::bookHistograms() {
  hist_->bookTelescopeAnalysisHistograms();
}

void TelescopeAnalysis::beginJob() {
  setAddresses();
  bookHistograms();
  analysisTree()->GetEntry(0);
}

void TelescopeAnalysis::eventLoop()
{
  //if (fChain == 0) return;
  
  Long64_t nbytes = 0, nb = 0;
  cout << "#Events=" << nEntries_ << endl;
  hist_->fillHist1D("TelescopeAnalysis","nevents", nEntries_);
  
  for (Long64_t jentry=0; jentry<nEntries_;jentry++) {
    clearEvent();
    Long64_t ientry = analysisTree()->GetEntry(jentry);
    if (ientry < 0) break;
    if (jentry%1000 == 0) 
      cout << " Events processed. " << std::setw(8) << jentry 
	   << endl;
    hist_->fillHist1D("TelescopeAnalysis","nhitsFei4", fei4Ev()->nPixHits);
    hist_->fillHist1D("TelescopeAnalysis","nTrack", telEv()->nTrackParams);
    //get residuals for single track events
    if (telEv()->nTrackParams != 1) continue;
    double tkX = -1.*telEv()->xPos->at(0);
    double tkY = telEv()->yPos->at(0);
    hist_->fillHist1D("TelescopeAnalysis","TkXPos", tkX);
    hist_->fillHist1D("TelescopeAnalysis","TkYPos", tkY);
    //tkXPosH->Fill(xPos->at(0));
    //tkYPosH->Fill(yPos->at(0));
    double xmin = 999.9;
    double ymin = 999.9;
    for (unsigned int i = 0; i < fei4Ev()->nPixHits; i++) {   
      hist_->fillHist1D("TelescopeAnalysis","HtColumn", fei4Ev()->col->at(i));
      hist_->fillHist1D("TelescopeAnalysis","HtRow", fei4Ev()->row->at(i));
      //default pitch and dimensions of fei4 plane
      double xval = -9.875 + (fei4Ev()->col->at(i)-1)*0.250;
      double yval = -8.375 + (fei4Ev()->row->at(i)-1)*0.05;
      hist_->fillHist1D("TelescopeAnalysis","HtXPos", xval);
      hist_->fillHist1D("TelescopeAnalysis","HtYPos", yval);
      
      hist_->fillHist2D("TelescopeAnalysis","tkXPosVsHtXPos", xval, tkX);
      hist_->fillHist2D("TelescopeAnalysis","tkYPosVsHtYPos", yval, tkY);
      
      if (std::fabs(xval - tkX) < xmin) xmin = xval - tkX;
      if (std::fabs(yval - tkY) < ymin) ymin = yval - tkY;
     }
    hist_->fillHist1D("TelescopeAnalysis","deltaXPos", xmin);
    hist_->fillHist1D("TelescopeAnalysis","deltaYPos", ymin);
  }//event loop
}

void TelescopeAnalysis::clearEvent() {
  BeamAnaBase::clearEvent();
}

void TelescopeAnalysis::endJob() {
  BeamAnaBase::endJob();
  hist_->closeFile();
}

TelescopeAnalysis::~TelescopeAnalysis(){
  delete hist_;
}
