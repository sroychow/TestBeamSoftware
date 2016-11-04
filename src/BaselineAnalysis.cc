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
  hist_->bookTrackMatchHistograms();
}

void BaselineAnalysis::beginJob() {
  BeamAnaBase::beginJob();
  nEntries_ = analysisTree()->GetEntries();
  hist_ = outFile();
  setAddresses();
  bookHistograms();
  analysisTree()->GetEntry(0);
  getCbcConfig(condEv()->cwd, condEv()->window);
}
 
void BaselineAnalysis::eventLoop()
{
   Long64_t nbytes = 0, nb = 0;
   cout << "#Events=" << nEntries_ << endl;
   hist_->fillHist1D("EventInfo","nevents", nEntries_);

   std::cout << "CBC configuration:: SW=" << stubWindow()
             << "\tCWD=" << cbcClusterWidth()
             << "\tOffset1="<< cbcOffset1() 
             << "\tOffset2" << cbcOffset2()
   << std::endl;
   long int trkFid = 0;
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
      
      setDetChannelVectors();
      const auto& d0c0 = *det0C0();
      const auto& d0c1 = *det0C1();
      const auto& d1c0 = *det1C0();
      const auto& d1c1 = *det1C1();
      fillCommonHistograms();
      //Telescope Matching
      if(doTelMatching() && hasTelescope()) {
        hist_->fillHist1D("TrackMatch","nTrackParams",telEv()->nTrackParams);
        
        hist_->fillHist1D("TrackMatch", "trkcluseff", 0);
        //Residual Calculation Now moved to AlignmentAnalysis
        //std::vector<double>  xtkDet0, xtkDet1;
        //getExtrapolatedTracks(xtkDet0, xtkDet1);
        std::vector<tbeam::Track>  fidTrkcoll;
        getExtrapolatedTracks(fidTrkcoll);
        //hist_->fillHist1D("TrackMatch", "nTrackParamsNodupl", xtkDet0.size());
        hist_->fillHist1D("TrackMatch", "nTrackParamsNodupl", fidTrkcoll.size());
        if(fidTrkcoll.empty())    continue;
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

        for(auto &tk : fidTrkcoll) {
          double x0 = tk.xtkDut0; 
          hist_->fillHist1D("TrackMatch","hposxTkDUT0",x0); 
          //matching at det0
          for(auto& h : d0c0) {
            double res = x0 - (h-nstrips()/2)*dutpitch();
            if(std::fabs(res) < std::fabs(minHitresStripD0)) {
              minHitresStripD0 = res; 
              minHitStripD0 = h;
            }
          }   
          for(auto& cl : dutRecoClmap()->at("det0C0") ) {
            double res = x0 - (cl.x-nstrips()/2)*dutpitch();
            if(std::fabs(res) <= 4*resDUT())   trkClsmatchD0 = true; 
            if(std::fabs(res) < std::fabs(minclsresD0))  {
              minclsresD0 = res;
              minclsposD0 = (cl.x-nstrips()/2)*dutpitch();
              minClusStripD0 = cl.x;
              minClusWD0 = cl.size;
            }
          }
          hist_->fillHist1D("TrackMatch","hminposClsDUT0",minclsposD0);
          hist_->fillHist1D("TrackMatch","minresidualDUT0_1trkfid", minclsresD0);
          hist_->fillHist1D("TrackMatch","clswidthDUT0_1trkfid", minClusWD0);
          hist_->fillHist2D("TrackMatch","minclsTrkPoscorrD0", x0/dutpitch() + nstrips()/2 , minClusStripD0);
          //matching at det1
          double x1 = tk.xtkDut1;
          hist_->fillHist1D("TrackMatch","hposxTkDUT1",x1); 
          for(auto& h : d1c0) {
            double res = x1 - (h-nstrips()/2)*dutpitch();
            if(std::fabs(res) < std::fabs(minHitresStripD1)) {
              minHitresStripD1 = res; 
              minHitStripD1 = h;
            }
          }   
          for(auto& cl : dutRecoClmap()->at("det1C0") ) {
            double res = x1 - (cl.x-nstrips()/2)*dutpitch();
            if(std::fabs(x1 - (cl.x-nstrips()/2)*dutpitch()) <= 4*resDUT())   trkClsmatchD1 = true;
            if(std::fabs(res) < std::fabs(minclsresD1))  {
              minclsresD1 = res;
              minclsposD1 = (cl.x-nstrips()/2)*dutpitch();
              minClusStripD1 = cl.x;
              minClusWD1 = cl.size;
            }
          }

          for(auto& s : dutRecoStubmap()->at("C0"))  {
            double sposres = x1 - (s.x-nstrips()/2)*dutpitch();
            if(std::fabs(sposres) <= 4*resDUT())  smatchD1 = true;  
            if(std::fabs(sposres) < std::fabs(minStubresC0)) {
              minStubresC0 = sposres;
              minStubposC0 = (s.x-nstrips()/2)*dutpitch();
              minStubStripC0 = s.x;
            }
          }
          hist_->fillHist1D("TrackMatch","hminposClsDUT1",minclsposD1);
          hist_->fillHist1D("TrackMatch","minresidualDUT1_1trkfid", minclsresD1);
          hist_->fillHist1D("TrackMatch","clswidthDUT1_1trkfid", minClusWD1);
          hist_->fillHist2D("TrackMatch","minclsTrkPoscorrD1", x1/dutpitch() + nstrips()/2, minClusStripD1);
          //for stub
          hist_->fillHist1D("TrackMatch","sminresidualC0_1trkfid", minStubresC0);
          hist_->fillHist1D("TrackMatch","hminposStub",minStubposC0);
          hist_->fillHist2D("TrackMatch","minstubTrkPoscorrD1", x1/dutpitch() + nstrips()/2, minStubStripC0);           
       }

        hist_->fillHist1D("TrackMatch", "trkcluseff", 3);
        trkFid++;
        hist_->fillHist1D("TrackMatch","effVtdc_den",static_cast<unsigned int>(condEv()->tdcPhase));
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
   std::cout << "\n#events with 1 fid trk(both)=" << trkFid
             << "\n#events with atleast 1 matched cluster with 1 fid trk(D0)=" << det0clsMatch
             << "\n#events with atleast 1 matched cluster with 1 fid trk(D1)=" << det1clsMatch
             << "\n#events with atleast 1 matched cluster with 1 fid trk(any)=" << clsMatchany
             << "\n#events with atleast 1 matched cluster with 1 fid trk(both)=" << clsMatchboth
             << "\n#events with atleast 1 matched reco stub in D1=" << recostubMatchD1
             << "\n#Abs Stub Efficiency=" << double(recostubMatchD1)/double(trkFid)
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
