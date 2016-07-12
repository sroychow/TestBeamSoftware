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
BaselineAnalysis::BaselineAnalysis(const std::string inFilename,const std::string outFilename) :
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
void BaselineAnalysis::bookHistograms() {
  hist_->bookEventHistograms();
  hist_->bookDUTHistograms("det0");
  hist_->bookDUTHistograms("det1");
  hist_->bookStubHistograms();
  hist_->bookCorrelationHistograms();
  hist_->bookTelescopeFitHistograms();
}

void BaselineAnalysis::beginJob() {
  setAddresses();
  bookHistograms();
  analysisTree()->GetEntry(0);
  getCbcConfig(condEv()->cwd, condEv()->window);
}
 
void BaselineAnalysis::eventLoop()
{
   //if (fChain == 0) return;

   Long64_t nbytes = 0, nb = 0;
   cout << "#Events=" << nEntries_ << endl;
   hist_->fillHist1D("EventInfo","nevents", nEntries_);

   std::cout << "CBC configuration:: SW=" << stubWindow()
             << "\tCWD=" << cbcClusterWidth()
             << "\tOffset1="<< cbcOffset1() 
             << "\tOffset2" << cbcOffset2()
   << std::endl;
 
   for (Long64_t jentry=0; jentry<nEntries_;jentry++) {
      clearEvent();
     Long64_t ientry = analysisTree()->GetEntry(jentry);
     if (ientry < 0) break;
     if (jentry%1000 == 0) {
       cout << " Events processed. " << std::setw(8) << jentry 
	    << endl;
     }
     //cout << "Point 1" << endl;
     if(jentry==0) {
       hist_->fillHist1D("EventInfo","hvSettings", condEv()->HVsettings);
       hist_->fillHist1D("EventInfo","dutAngle", condEv()->DUTangle);
       hist_->fillHist1D("EventInfo","vcth", condEv()->vcth);
       hist_->fillHist1D("EventInfo","offset", cbcOffset1());
       hist_->fillHist1D("EventInfo","offset", cbcOffset2());
       hist_->fillHist1D("EventInfo","window", stubWindow());
       hist_->fillHist1D("EventInfo","tilt", static_cast<unsigned long int>(condEv()->tilt));
     }
     hist_->fillHist1D("EventInfo","isPeriodic",isPeriodic());
     hist_->fillHist1D("EventInfo","isGoodFlag",isGoodEvent());

     if(!isGoodEvent())   continue;

     hist_->fillHist1D("EventInfo","condData", condEv()->condData);
     hist_->fillHist1D("EventInfo","tdcPhase", static_cast<unsigned int>(condEv()->tdcPhase));
      
      //cout << "Point 2" << endl;
      setDetChannelVectors();
      
      const auto& d0c0 = *det0C0();
      const auto& d0c1 = *det0C1();
      const auto& d1c0 = *det1C0();
      const auto& d1c1 = *det1C1();      
      //cout << "Point 3" << endl;
      //Fill histo for det0
      hist_->fillHist1D("det0","chsizeC0", d0c0.size());
      //hist_->fillHist1D("det0","chsizeC1", det0C1()->size());
      hist_->fillHistofromVec(d0c0,"det0","hitmapC0");
      //hist_->fillHistofromVec(d0c1,"det0","hitmapC1");
      hist_->fill2DHistofromVec(d0c0,d0c1,"det0","hitmapfull");
      hist_->fillClusterHistograms("det0",dutRecoClmap()->at("det0C0"),"C0");
      //hist_->fillClusterHistograms("det0",dutRecoClmap()->at("det0C1"),"C1");

      //Fill histo for det1
      //std::cout << "Hits det1c0=" << dut1Ch0()->size() << std::endl;
      hist_->fillHist1D("det1","chsizeC0", d1c0.size());
      //hist_->fillHist1D("det1","chsizeC1", d1c1.size());
      hist_->fillHistofromVec(d1c0,"det1","hitmapC0");
      //hist_->fillHistofromVec(d1c1,"det1","hitmapC1");
      hist_->fill2DHistofromVec(d1c0,d1c1,"det1","hitmapfull");
      hist_->fillClusterHistograms("det1",dutRecoClmap()->at("det1C0"),"C0");
      //hist_->fillClusterHistograms("det1",dutRecoClmap()->at("det1C1"),"C1");
      
      //cout << "Point 4" << endl;
      if(d0c0.size() && !d1c0.size()) hist_->fillHist1D("Correlation","cor_hitC0", 1);
      if(!d0c0.size() && d1c0.size()) hist_->fillHist1D("Correlation","cor_hitC0", 2);
      if(d0c0.size() && d1c0.size()) hist_->fillHist1D("Correlation","cor_hitC0", 3);
      if(!d0c0.size() && !d1c0.size()) hist_->fillHist1D("Correlation","cor_hitC0", 4);
      hist_->fillHist1D("Correlation","nclusterdiffC0", std::abs(dutRecoClmap()->at("det1C0").size() - 
                                                        dutRecoClmap()->at("det1C0").size())); 
      //cout << "Point 4a" << endl;
      int totStubReco = dutEv()->stubs.size();
      int nstubrecoSword = nStubsrecoSword();
      int nstubscbcSword = nStubscbcSword();
      hist_->fillHist1D("StubInfo","nstubRecoC0", dutRecoStubmap()->at("C0").size());      
      hist_->fillHist1D("StubInfo","nstubsFromReco",totStubReco);
      hist_->fillHist1D("StubInfo","nstubsFromCBCSword",nstubrecoSword);
      hist_->fillHist1D("StubInfo","nstubsFromRecoSword",nstubscbcSword);
      for(auto& c : *recostubChipids())  
        hist_->fillHistofromVec(c.second,"StubInfo","recoStubWord");
      for(auto& c : *cbcstubChipids())  
        hist_->fillHistofromVec(c.second,"StubInfo","cbcStubWord");

      if (!nstubrecoSword && !nstubscbcSword) hist_->fillHist1D("StubInfo","stubMatch", 1);
      if (!nstubrecoSword && nstubscbcSword)  hist_->fillHist1D("StubInfo","stubMatch", 2);
      if (nstubrecoSword && !nstubscbcSword)  hist_->fillHist1D("StubInfo","stubMatch", 3);
      if (nstubrecoSword && nstubscbcSword)   hist_->fillHist1D("StubInfo","stubMatch", 4);
      hist_->fillHist1D("StubInfo","nstubsdiffSword",nstubrecoSword - nstubscbcSword);      
      hist_->fillHist1D("StubInfo","nstubsdiff",totStubReco - nstubscbcSword);  
      //Telescope Matching
      if(doTelMatching() && hasTelescope()) {
        hist_->fillHist1D("TelescopeFit","nTrackParams",telEv()->nTrackParams);
        //for residual calculation use only events with 1 track and 1 cluster in both sensor
        //
        if(telEv()->nTrackParams == 1 
           && dutRecoClmap()->at("det0C0").size() == 1 
           && dutRecoClmap()->at("det1C0").size() == 1 ) {
          /* If you want to calculate the residuals with multi-track events, use the following
             The function will fill the vectors with extrapolated track psoitions on the dut plane
             std::vector<double> xtkdut0, xtkdut1;      
             getExtrapolatedTracks(xtkdut0, xtkdut1);
          */
          double xtkdut0 = (z_DUT0-z_FEI4)*(telEv()->dxdz->at(0)) + telEv()->xPos->at(0);
          double xtkdut1 = (z_DUT1-z_FEI4)*(telEv()->dxdz->at(0)) + telEv()->xPos->at(0);
          
          hist_->fillHist1D("TelescopeFit", "xpos",telEv()->xPos->at(0));
          hist_->fillHist1D("TelescopeFit", "xtkatDUT0", xtkdut0);
          hist_->fillHist1D("TelescopeFit", "xtkatDUT1", xtkdut1);
          //convert extrapolated track position to strip number of dut and check if it falls in fiducial region
          bool xtkdet0StripGood = true;
          bool xtkdet1StripGood = true;
          int xtkdut0Strip = (xtkdut0/0.09) + 127;
          int xtkdut1Strip = (xtkdut1/0.09) + 127; 
          //Masking is not done for non-irradiated module, Hence no need for fiducial cut
          if(doChannelMasking()) {
            xtkdet0StripGood =  xtkdut0Strip > 127 && std::find(getMaskedChannelMap()->at("det0").begin(), 
                                                                   getMaskedChannelMap()->at("det0").end(), 
                                                                   xtkdut0Strip) == getMaskedChannelMap()->at("det0").end();

            xtkdet1StripGood =  xtkdut1Strip > 127 && std::find(getMaskedChannelMap()->at("det1").begin(), 
                                                                   getMaskedChannelMap()->at("det1").end(), 
                                                                   xtkdut0Strip) == getMaskedChannelMap()->at("det1").end();
          }
          hist_->fillHist1D("TelescopeFit", "isTrkFiducial", 0);
          if(xtkdet0StripGood)  hist_->fillHist1D("TelescopeFit", "isTrkFiducial", 1);
          if(xtkdet1StripGood)  hist_->fillHist1D("TelescopeFit", "isTrkFiducial", 2);
          if(xtkdet0StripGood && xtkdet1StripGood)  hist_->fillHist1D("TelescopeFit", "isTrkFiducial", 3);
          double delX_CluD0= xtkdut0 - (dutRecoClmap()->at("det0C0").at(0).x-127)*0.09;
          double delX_CluD1= xtkdut1 - (dutRecoClmap()->at("det1C0").at(0).x-127)*0.09;
          hist_->fillHist1D("TelescopeFit","residualDUT0",delX_CluD0);
          hist_->fillHist1D("TelescopeFit","residualDUT1",delX_CluD1);
        } 
      }   
   }//event loop
   if(doTelMatching() && hasTelescope())    fitResidualHistograms();
}

void BaselineAnalysis::clearEvent() {
  BeamAnaBase::clearEvent();
}
void BaselineAnalysis::fitResidualHistograms() {
  hist_->hfile()->cd("TelescopeFit");
  TH1D* resDut0 = dynamic_cast<TH1D*>(Utility::getHist1D("residualDUT0"));
  if(resDut0 && resDut0->GetEntries() > 500 ) {  
    std::cout << "Fitting dut0 residual" << std::endl;
    int maxbin = resDut0->GetMaximumBin();
    double rlow = resDut0->GetBinCenter(maxbin - 20);
    double rmax = resDut0->GetBinCenter(maxbin + 20);
    resDut0->Fit("gaus", "","", rlow, rmax );
    TF1* hfit = resDut0->GetFunction("gaus");
    meanResDet0 = hfit->GetParameter("Mean"); 
    sigResDet0 = hfit->GetParameter("Sigma");
  }
  else std::cout << "Det0 residual Fit not done;#entries=" << resDut0->GetEntries() << std::endl;

  TH1D* resDut1 = dynamic_cast<TH1D*>(Utility::getHist1D("residualDUT1"));
  if(resDut1 && resDut0->GetEntries() > 500 ) {  
    std::cout << "Fitting dut1 residual" << std::endl;
    int maxbin = resDut1->GetMaximumBin();
    double rlow = resDut1->GetBinCenter(maxbin - 20);
    double rmax = resDut1->GetBinCenter(maxbin + 20);
    resDut1->Fit("gaus", "","", rlow, rmax );
    TF1* hfit = resDut1->GetFunction("gaus");
    meanResDet1 = hfit->GetParameter("Mean"); 
    sigResDet1 = hfit->GetParameter("Sigma");
  }
  else std::cout << "Det1 residual Fit not done;#entries=" << resDut1->GetEntries() << std::endl;
}

void BaselineAnalysis::endJob() {
  std::cout << "meanResDet0=" << meanResDet0 << "\tsigResDet0=" << sigResDet0 
            << "\t#StripDiff(4sig)=" << 4*sigResDet0/0.09 << std::endl;
  std::cout << "meanResDet1=" << meanResDet1 << "\tsigResDet1=" << sigResDet1 
            << "\t#StripDiff(4sig)=" << 4*sigResDet0/0.09 <<std::endl;
  BeamAnaBase::endJob();
  hist_->closeFile();
}

BaselineAnalysis::~BaselineAnalysis(){
  delete hist_;
}
