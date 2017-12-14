/*!
  \file                BaselineAnalysis.cc
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

#include "BaselineAnalysis.h"
using std::vector;
using std::map;
BaselineAnalysis::BaselineAnalysis() :
  BeamAnaBase::BeamAnaBase()
{
}
void BaselineAnalysis::bookHistograms() {
  BeamAnaBase::bookHistograms();
  //hist_->bookTrackMatchHistograms();
}

void BaselineAnalysis::beginJob() {
  BeamAnaBase::beginJob();
  nEntries_ = analysisTree()->GetEntries();
  hist_ = outFile();
  setAddresses();
  bookHistograms();
  //analysisTree()->GetEntry(0);
  //getCbcConfig(condEv()->cwd, condEv()->window);
}
 
void BaselineAnalysis::eventLoop()
{
  Long64_t nbytes = 0, nb = 0;
  cout << "#Events=" << nEntries_ << endl;
  hist_->fillHist1D("EventInfo","nevents", nEntries_);
  for (Long64_t jentry=0; jentry<nEntries_;jentry++) {
    clearEvent();
    Long64_t ientry = analysisTree()->GetEntry(jentry);
    if (ientry < 0) break;
    if (jentry%1000 == 0) {
      cout << " Events processed. " << std::setw(8) << jentry 
	   << endl;
    }
    if(jentry==0) {
      hist_->fillHist1D("EventInfo","hvSettings", event()->HVsettings);
      hist_->fillHist1D("EventInfo","dutAngle", event()->DUTangle);
      hist_->fillHist1D("EventInfo","vcth", event()->vcth);
      //hist_->fillHist1D("EventInfo","offset", cbcOffset1());
      //hist_->fillHist1D("EventInfo","offset", cbcOffset2());
      //hist_->fillHist1D("EventInfo","window", stubWindow());
      //hist_->fillHist1D("EventInfo","tilt", static_cast<unsigned long int>(condEv()->tilt));
      cout << "Alignment Parameters" << aLparameteres();
    }
    hist_->fillHist1D("EventInfo","condData", static_cast<unsigned int>(event()->condData));
    hist_->fillHist1D("EventInfo","tdcPhase", static_cast<unsigned int>(event()->tdcPhase));
    //fill common histograms for dut hits, clusters
    fillCommonHistograms();
  }

}

void BaselineAnalysis::clearEvent() {
  BeamAnaBase::clearEvent();
}

void BaselineAnalysis::endJob() {
  BeamAnaBase::endJob();
  hist_->closeFile();
}

BaselineAnalysis::~BaselineAnalysis(){
  delete hist_;
}
