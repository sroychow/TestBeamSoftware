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
  hist_->bookTelescopeMatchedHistograms();
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
        hist_->fillHist1D("TelescopeMatch","nTrackParams",telEv()->nTrackParams);
        //for residual calculation use only events with 1 track and 1 cluster in both sensor
        if(telEv()->nTrackParams == 1 
           && dutRecoClmap()->at("det0C0").size() == 1 
           && dutRecoClmap()->at("det1C0").size() == 1 ) {
          std::vector<double> xtkdut0, xtkdut1;      
          getExtrapolatedTracks(xtkdut0, xtkdut1);
          for(const auto& x : *(telEv()->xPos))
            hist_->fillHist1D("TelescopeMatch","xpos",x);
          for(const auto& x : xtkdut0)        
            hist_->fillHist1D("TelescopeMatch","xtkatDUT0",x);
          for(const auto& x : xtkdut1)        
            hist_->fillHist1D("TelescopeMatch","xtkatDUT1",x);
          
          double delX_CluD0= xtkdut0[0] - (dutRecoClmap()->at("det0C0").at(0).x-127)*0.09;
          double delX_CluD1= xtkdut1[0] - (dutRecoClmap()->at("det1C0").at(0).x-127)*0.09;
          hist_->fillHist1D("TelescopeMatch","residualDUT0",delX_CluD0);
          if(std::fabs(delX_CluD0 - meanResDet0) <= 3*sigResDet0){
            hist_->fillHist1D("TelescopeMatch","clusterWidthD0C0",dutRecoClmap()->at("det0C0").at(0).size);
          }
          hist_->fillHist1D("TelescopeMatch","residualDUT1",delX_CluD1);
          if(std::fabs(delX_CluD1 - meanResDet1) <= 3*sigResDet1){
            hist_->fillHist1D("TelescopeMatch","clusterWidthD1C0",dutRecoClmap()->at("det1C0").at(0).size);
          }
          //Now find matched hits and clusters
          
        } 
      }   
   }
}

void BaselineAnalysis::clearEvent() {
  BeamAnaBase::clearEvent();
}
void BaselineAnalysis::fitResidualHistograms() {
  hist_->hfile()->cd("TelescopeMatch");
  TH1D* resDut0 = dynamic_cast<TH1D*>(Utility::getHist1D("residualDUT0"));
  if(resDut0) {  
    std::cout << "Fitting dut0 residual" << std::endl;
    int maxbin = resDut0->GetMaximumBin();
    double rlow = resDut0->GetBinCenter(maxbin - 20);
    double rmax = resDut0->GetBinCenter(maxbin + 20);
    resDut0->Fit("gaus", "","", rlow, rmax );
  }
  TH1D* resDut1 = dynamic_cast<TH1D*>(Utility::getHist1D("residualDUT1"));
  if(resDut1) {  
    std::cout << "Fitting dut1 residual" << std::endl;
    int maxbin = resDut1->GetMaximumBin();
    double rlow = resDut1->GetBinCenter(maxbin - 20);
    double rmax = resDut1->GetBinCenter(maxbin + 20);
    resDut1->Fit("gaus", "","", rlow, rmax );
  }

}
void BaselineAnalysis::endJob() {
  if(doTelMatching() && hasTelescope())  fitResidualHistograms();
  BeamAnaBase::endJob();
  hist_->closeFile();
}

BaselineAnalysis::~BaselineAnalysis(){
  delete hist_;
}
