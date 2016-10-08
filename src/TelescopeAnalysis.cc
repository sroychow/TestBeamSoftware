/*
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
TelescopeAnalysis::TelescopeAnalysis() :
  BeamAnaBase::BeamAnaBase()
{
}
void TelescopeAnalysis::bookHistograms() {
  hist_->bookTelescopeAnalysisHistograms();
}

void TelescopeAnalysis::beginJob() {
  BeamAnaBase::beginJob();
  nEntries_ = analysisTree()->GetEntries();
  hist_ = outFile();
  setAddresses();
  bookHistograms();
}

void TelescopeAnalysis::eventLoop()
{
  Long64_t nbytes = 0, nb = 0;
  cout << "#Events=" << nEntries_ << endl;
  for (Long64_t jentry=0; jentry<nEntries_;jentry++) {
    clearEvent();
    Long64_t ientry = analysisTree()->GetEntry(jentry);
    if (ientry < 0) break;
    if (jentry%1000 == 0) 
      cout << " Events processed. " << std::setw(8) << jentry 
	   << endl;
    hist_->fillHist1D("TelescopeAnalysis","nhitsFei4", fei4Ev()->nPixHits);
    hist_->fillHist1D("TelescopeAnalysis","nTrack", telEv()->nTrackParams);
    
    if(fei4Ev()->nPixHits > 2)    continue;
    if(telEv()->xPos->empty())    continue;
    //std::cout << telEv()->xPos->size() << std::endl;
    for(unsigned int i = 0; i<telEv()->xPos->size(); i++) {
      //std::cout << i<< std::endl;
      double tkX = -1.*telEv()->xPos->at(i);
      double tkY = telEv()->yPos->at(i);
      hist_->fillHist1D("TelescopeAnalysis","TkXPos", tkX);
      hist_->fillHist1D("TelescopeAnalysis","TkYPos", tkY);
    }
    //get residuals
    for (unsigned int i = 0; i < fei4Ev()->nPixHits; i++) {   
      hist_->fillHist1D("TelescopeAnalysis","HtColumn", fei4Ev()->col->at(i));
      hist_->fillHist1D("TelescopeAnalysis","HtRow", fei4Ev()->row->at(i));
      //default pitch and dimensions of fei4 plane
      double xval = -9.875 + (fei4Ev()->col->at(i)-1)*0.250;
      double yval = -8.375 + (fei4Ev()->row->at(i)-1)*0.05;
      hist_->fillHist1D("TelescopeAnalysis","HtXPos", xval);
      hist_->fillHist1D("TelescopeAnalysis","HtYPos", yval);
      //now loop over tracks
      double xmin = 999.9;
      double ymin = 999.9;
      
      for(unsigned int itk = 0; itk < telEv()->xPos->size(); itk++) {
        double tkX = -1.*telEv()->xPos->at(itk);
        double tkY = telEv()->yPos->at(itk);
        hist_->fillHist2D("TelescopeAnalysis","tkXPosVsHtXPos", xval, tkX);
        hist_->fillHist2D("TelescopeAnalysis","tkYPosVsHtYPos", yval, tkY);
        if (std::fabs(xval - tkX) < xmin) xmin = xval - tkX;
        if (std::fabs(yval - tkY) < ymin) ymin = yval - tkY;
      }
      hist_->fillHist1D("TelescopeAnalysis","deltaXPos", xmin);
      hist_->fillHist1D("TelescopeAnalysis","deltaYPos", ymin);
    }
  }//event loop

  //Fit residual in Y direction//Perpendicular to strips in DUT
  TF1* fGausResiduals_y = new TF1("fGausResidualsy", "gaus", -10., 10.);
  TH1F* htmp = dynamic_cast<TH1F*>(hist_->GetHistoByName("TelescopeAnalysis","deltaYPos"));   
  float center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals_y->SetRange(center-0.3, center+0.3);
  //fGausResiduals_y->SetParameter(1, center);
  //fGausResiduals_y->SetParameter(2, 0.015);
  htmp->Fit("fGausResidualsy");
  
  /*
  //Fit with Gaussian convoluted with StepFunc  To be Done
  center = fGausResiduals_y->GetParameter(1);
  float rms = 5*fGausResiduals_y->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  
  TF1* fStepGaus_y = new TF1("FuncStepGausy", Utility::funcStepGaus, -1., 1., 4);
  fStepGaus_y->SetLineWidth(2);
  fStepGaus_y->SetLineColor(kRed);
  fStepGaus_y->SetParLimits(0, 0., 0.1);
  fStepGaus_y->SetParameter(0, 0.05);
  fStepGaus_y->SetParLimits(1, 0.0, 1.);
  fStepGaus_y->SetParameter(1, 0.03);
  fStepGaus_y->SetParameter(2, htmp->GetMaximum());
  fStepGaus_y->SetParLimits(3, 0, 10);
  fStepGaus_y->SetParameter(3, 0);
  htmp->Fit(fStepGaus_y);
  */
  
  //Fit residual in X direction//Parallel to strips in DUT
  TF1* fGausResiduals_x = new TF1("fGausResidualsx", "gaus", -10, 10);
  htmp = dynamic_cast<TH1F*>(hist_->GetHistoByName("TelescopeAnalysis","deltaXPos"));   
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals_x->SetRange(center-0.3, center+0.3);
  //fGausResiduals_x->SetParameter(1, center);
  //fGausResiduals_x->SetParameter(2, 0.015);
  htmp->Fit("fGausResidualsx");
  
  /*
  //Fit with Gaussian convoluted with StepFunc  To be Done
  center = fGausResiduals_x->GetParameter(1);
  float rms = 5*fGausResiduals_x->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  
  TF1* fStepGaus_x = new TF1("FuncStepGausx", Utility::funcStepGaus, -1., 1., 4);
  fStepGaus_x->SetLineWidth(2);
  fStepGaus_x->SetLineColor(kRed);
  fStepGaus_x->SetParLimits(0, 0., 0.1);
  fStepGaus_x->SetParameter(0, 0.05);
  fStepGaus_x->SetParLimits(1, 0.0, 1.);
  fStepGaus_x->SetParameter(1, 0.03);
  fStepGaus_x->SetParameter(2, htmp->GetMaximum());
  fStepGaus_x->SetParLimits(3, 0, 10);
  fStepGaus_x->SetParameter(3, 0);
  htmp->Fit(fStepGaus_x);
  */

  std::cout << "Summary of the Gaussian Fits to the residuals:\n"
            << "Residual X>>>Mean=" << fGausResiduals_x->GetParameter("Mean") 
            << ">>>Sigma=" << fGausResiduals_x->GetParameter("Sigma") 
            << "\nResidual Y>>>Mean=" << fGausResiduals_y->GetParameter("Mean") 
            << ">>>Sigma=" << fGausResiduals_y->GetParameter("Sigma") 
            << std::endl;

  for (Long64_t jentry=0; jentry<nEntries_;jentry++) {
    clearEvent();
    Long64_t ientry = analysisTree()->GetEntry(jentry);
    if (ientry < 0) break;
    if (jentry%1000 == 0) 
      cout << " Events processed. " << std::setw(8) << jentry 
	   << endl;
    if(fei4Ev()->nPixHits > 2)    continue;
    if(telEv()->nTrackParams != 1)  continue;
    //get residuals
    for(unsigned int itk = 0; itk < telEv()->xPos->size(); itk++) {
      double tkX = -1.*telEv()->xPos->at(itk);
      double tkY = telEv()->yPos->at(itk);
      double minresx = 999.;
      double minresy = 999.;
      for (unsigned int i = 0; i < fei4Ev()->nPixHits; i++) {   
        //default pitch and dimensions of fei4 plane
        double xval = -9.875 + (fei4Ev()->col->at(i)-1)*0.250;
        double yval = -8.375 + (fei4Ev()->row->at(i)-1)*0.05;
        double xres = xval - tkX - fGausResiduals_x->GetParameter("Mean");
        double yres = yval - tkY - fGausResiduals_y->GetParameter("Mean");
        if(std::fabs(xres) < std::fabs(minresx))   minresx = xres;
        if(std::fabs(yres) < std::fabs(minresy))   minresy = yres;
      }
      hist_->fillHist1D("TelescopeAnalysis","deltaXPos_trkfei4", minresx) ;
      hist_->fillHist1D("TelescopeAnalysis","deltaYPos_trkfei4", minresy);
      if(std::fabs(minresx) < 4*fGausResiduals_x->GetParameter("Sigma") && 
        std::fabs(minresy) < 4*fGausResiduals_y->GetParameter("Sigma")) {
        hist_->fillHist1D("TelescopeAnalysis","deltaXPos_trkfei4M", minresx);
        hist_->fillHist1D("TelescopeAnalysis","deltaYPos_trkfei4M", minresy);
      }
    }
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
