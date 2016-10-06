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
  hist_->bookTrackMatchHistograms();
  //hist_->bookTelescopeAnalysisHistograms();
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
   long int trkFidbothPlane = 0;
   long int trkFidany = 0;
   long int trkFidDet0 = 0;
   long int trkFidDet1 = 0;
   long int det0clsMatch = 0;
   long int det1clsMatch = 0;
   long int clsMatchboth = 0;
   long int clsMatchany = 0;
   long int recostubMatchD1 = 0;
   unsigned long int lastBadevent = 0; 
   int nMatchedCluster = 0;
   
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

     if(!isGoodEvent())   {
      lastBadevent = jentry; 
      continue;
     }

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
      hist_->fillHist2D("det0","nhitvsnclusC0", d0c0.size(), dutRecoClmap()->at("det0C0").size());
      for(const auto& h: d0c0) {
        int minposdiff = 255;
        for(const auto& cl:dutRecoClmap()->at("det0C0")) {
          if(std::abs(cl.x-h) < minposdiff)   minposdiff = std::abs(cl.x-h);
        }
        hist_->fillHist2D("det0","nhitvsHitClusPosDiffC0", d0c0.size(), minposdiff);
      }


      //Fill histo for det1
      //std::cout << "Hits det1c0=" << dut1Ch0()->size() << std::endl;
      hist_->fillHist1D("det1","chsizeC0", d1c0.size());
      //hist_->fillHist1D("det1","chsizeC1", d1c1.size());
      hist_->fillHistofromVec(d1c0,"det1","hitmapC0");
      //hist_->fillHistofromVec(d1c1,"det1","hitmapC1");
      hist_->fill2DHistofromVec(d1c0,d1c1,"det1","hitmapfull");
      hist_->fillClusterHistograms("det1",dutRecoClmap()->at("det1C0"),"C0");
      //hist_->fillClusterHistograms("det1",dutRecoClmap()->at("det1C1"),"C1");
      hist_->fillHist2D("det1","nhitvsnclusC0", d1c0.size(), dutRecoClmap()->at("det1C0").size());
      for(const auto& h: d1c0) {
        int minposdiff = 255;
        for(const auto& cl:dutRecoClmap()->at("det1C0")) {
          if(std::abs(cl.x-h) < minposdiff)   minposdiff = std::abs(cl.x-h);
        }
        hist_->fillHist2D("det1","nhitvsHitClusPosDiffC0", d1c0.size(), minposdiff);
      }
      
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
        hist_->fillHist1D("TrackMatch","nTrackParams",telEv()->nTrackParams);
        if(telEv()->nTrackParams!=1)   continue;
        hist_->fillHist1D("TrackMatch", "trkcluseff", 0);
        //Residual Calculation Now moved to AlignmentAnalysis
        std::vector<double>  xtkDet0, xtkDet1;
        getExtrapolatedTracks(xtkDet0, xtkDet1);
        hist_->fillHist1D("TrackMatch", "nTrackParamsNodupl", xtkDet0.size());
        bool isXtkfidDUT0 = false;
        bool isXtkfidDUT1 = false;
        bool trkClsmatchD0 = false;
        bool trkClsmatchD1 = false;
        bool smatchD1 = false;
        double minclsresD0 = 9999.;
        double minclsresD1 = 9999.;
        double minclsposD0 = 9999.;
        double minclsposD1 = 9999.;
        double minStubresC0 = 9999.;
        double minStubposC0 = 9999.;
        int minStubStripC0 = 999.;
        int minClusStripD0 = 999;
        int minClusStripD1 = 999;
        int minClusWD0 = 999;
        int minClusWD1 = 999;
 
        int minHitStripD0 = 999;
        int minHitStripD1 = 999;
        double minHitresStripD0 = 999.;
        double minHitresStripD1 = 999.;

        int xtkStripDet0 = 999.;
        int xtkStripDet1 = 999.;
        for(auto &x0 : xtkDet0) { 
          //if(fabs(x0) > 5. )   continue;
          if(isTrkfiducial(x0,xtkStripDet0,"det0")) {
            isXtkfidDUT0 = true;
            hist_->fillHist1D("TrackMatch","hposxTkDUT0",x0); 
            for(auto& h : d0c0) {
              if(h < 71 || h > 183 )   continue;
              double res = x0 - (h-127)*0.09;
              if(std::fabs(res) < std::fabs(minHitresStripD0)) {
                minHitresStripD0 = res; 
                minHitStripD0 = h;
              }
            }   
            for(auto& cl : dutRecoClmap()->at("det0C0") ) {
              if(cl.x < 71 || cl.x > 183 )   continue;
              double res = x0 - (cl.x-127)*0.09;
              if(std::fabs(res) <= 4*0.026)   trkClsmatchD0 = true; 
              if(std::fabs(res) < std::fabs(minclsresD0))  {
                minclsresD0 = res;
                minclsposD0 = (cl.x-127)*0.09;
                minClusStripD0 = cl.x;
                minClusWD0 = cl.size;
              }
            }
            hist_->fillHist1D("TrackMatch","hminposClsDUT0",minclsposD0);
            hist_->fillHist1D("TrackMatch","minresidualDUT0_1trkfid", minclsresD0);
            hist_->fillHist1D("TrackMatch","clswidthDUT0_1trkfid", minClusWD0);
            hist_->fillHist2D("TrackMatch","minclsTrkPoscorrD0", xtkStripDet0, minClusStripD0);
          }
        }
        for(auto &x1 : xtkDet1) { 
          if(isTrkfiducial(x1, xtkStripDet1,"det1")) {
           //if(fabs(x1) > 5. )   continue;
            isXtkfidDUT1 = true;
            hist_->fillHist1D("TrackMatch","hposxTkDUT1",x1); 
            for(auto& h : d1c0) {
              if(h < 71 || h > 183 )   continue;
              double res = x1 - (h-127)*0.09;
              if(std::fabs(res) < std::fabs(minHitresStripD1)) {
                minHitresStripD1 = res; 
                minHitStripD1 = h;
              }
            }   
            for(auto& cl : dutRecoClmap()->at("det1C0") ) {
              if(cl.x < 71 || cl.x > 183 )   continue;
              double res = x1 - (cl.x-127)*0.09;
              if(std::fabs(x1 - (cl.x-127)*0.09) <= 4*0.026)   trkClsmatchD1 = true;
              if(std::fabs(res) < std::fabs(minclsresD1))  {
                minclsresD1 = res;
                minclsposD1 = (cl.x-127)*0.09;
                minClusStripD1 = cl.x;
                minClusWD1 = cl.size;
              }
            }

            for(auto& s : dutRecoStubmap()->at("C0"))  {
              if(s.x < 71 || s.x > 183 )   continue;
              double sposres = x1 - (s.x-127)*0.09;
              //hist_->fillHist1D("TrackMatch","sresidualC0multitrkfidNodupl", sposres);
              if(std::fabs(sposres) <= 4*0.026)  smatchD1 = true;  
              if(std::fabs(sposres) < std::fabs(minStubresC0)) {
                minStubresC0 = sposres;
                minStubposC0 = (s.x-127)*0.09;
                minStubStripC0 = s.x;
              }
            }
            hist_->fillHist1D("TrackMatch","hminposClsDUT1",minclsposD1);
            hist_->fillHist1D("TrackMatch","minresidualDUT1_1trkfid", minclsresD1);
            hist_->fillHist1D("TrackMatch","clswidthDUT1_1trkfid", minClusWD1);
            hist_->fillHist2D("TrackMatch","minclsTrkPoscorrD1", xtkStripDet1, minClusStripD1);
            //for stub
            hist_->fillHist1D("TrackMatch","sminresidualC0_1trkfid", minStubresC0);
            hist_->fillHist1D("TrackMatch","hminposStub",minStubposC0);
            hist_->fillHist2D("TrackMatch","minstubTrkPoscorrD1", xtkStripDet1, minStubStripC0);           
          }
        }
        if(isXtkfidDUT0) {
          trkFidDet0++;
          hist_->fillHist1D("TrackMatch", "trkcluseff", 1);
          for(auto& cl : dutRecoClmap()->at("det0C0") ) {
            if(cl.x < 71 || cl.x > 183 )   continue;
          }
        }
        if(isXtkfidDUT1) {
          trkFidDet1++;
          hist_->fillHist1D("TrackMatch", "trkcluseff", 2);
          for(auto& cl : dutRecoClmap()->at("det1C0") ) {
            if(cl.x < 71 || cl.x > 183 )   continue;
          }
          for(auto& s : dutRecoStubmap()->at("C0"))  {
            if(s.x < 71 || s.x > 183 )   continue;
          }
        }
        if(isXtkfidDUT0 || isXtkfidDUT1)  trkFidany++;
        if(isXtkfidDUT0 && isXtkfidDUT1)  {
          hist_->fillHist1D("TrackMatch", "trkcluseff", 3);
          trkFidbothPlane++;
          hist_->fillHist1D("TrackMatch","effVtdc_den",static_cast<unsigned int>(condEv()->tdcPhase));
        }
        if(trkClsmatchD0)   {
          det0clsMatch++;
          hist_->fillHist1D("TrackMatch", "trkcluseff", 4);
        }
        if(trkClsmatchD1)   {
          det1clsMatch++;
          hist_->fillHist1D("TrackMatch", "trkcluseff", 5);
        }
        if(trkClsmatchD0 || trkClsmatchD1)   clsMatchany++;
        if(trkClsmatchD0 && trkClsmatchD1)   {
          clsMatchboth++;
          hist_->fillHist1D("TrackMatch", "trkcluseff", 6);
          hist_->fillHist1D("TrackMatch","effVtdc_num",static_cast<unsigned int>(condEv()->tdcPhase));
        }
        if(smatchD1) {
          recostubMatchD1++;
          hist_->fillHist1D("TrackMatch", "trkcluseff", 8);
        }
        if(!trkClsmatchD0 && !trkClsmatchD1)  {
          hist_->fillHist1D("TrackMatch", "trkcluseff", 7);
        }
      }   
   }//event loop
   std::cout << "#events with 1 fid trk(D0)=" << trkFidDet0
             << "\n#events with 1 fid trk(D1)=" << trkFidDet1 
             << "\n#events with 1 fid trk(any)=" << trkFidany
             << "\n#events with 1 fid trk(both)=" << trkFidbothPlane
             << "\n#events with atleast 1 matched cluster with 1 fid trk(D0)=" << det0clsMatch
             << "\n#events with atleast 1 matched cluster with 1 fid trk(D1)=" << det1clsMatch
             << "\n#events with atleast 1 matched cluster with 1 fid trk(any)=" << clsMatchany
             << "\n#events with atleast 1 matched cluster with 1 fid trk(both)=" << clsMatchboth
             << "\n#events with atleast 1 matched reco stub in D1=" << recostubMatchD1
             << "\n#Abs Stub Efficiency=" << double(recostubMatchD1)/double(trkFidbothPlane)
             << std::endl;
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
