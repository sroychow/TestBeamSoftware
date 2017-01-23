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

        std::vector<tbeam::Track>  tkNoOv;
        Utility::removeTrackDuplicates(telEv(), tkNoOv);
        //Match with FEI4
        std::vector<tbeam::Track>  selectedTk;
        Utility::cutTrackFei4Residuals(fei4Ev(), tkNoOv, selectedTk, offsetfei4x(), offsetfei4y(), resfei4x(), resfei4y(), true);

        //with no duplicate removal
        //NTracks_withduplicate
        hist_->fillHist1D("TrackMatch", "NTracks_withduplicate" , telEv()->nTrackParams);
        /*
        std::vector<tbeam::Track>  noDuplicate_Me_Trkcoll;
        for(unsigned int itrk = 0; itrk< telEv()->xPos->size() ;itrk++) {

          hist_->fillHist1D("TrackMatch","track_chi2_withduplicate",  telEv()->chi2->at(itrk)    );
          hist_->fillHist1D("TrackMatch","track_ndf_withduplicate",   telEv()->ndof->at(itrk)    );
          hist_->fillHist1D("TrackMatch","track_normchi2_withduplicate",  telEv()->chi2->at(itrk)/telEv()->ndof->at(itrk));
          bool foundDuplicate = false;
          int thetrackN = -1;
          for(unsigned int itrk2 = 0; itrk2< telEv()->xPos->size() ;itrk2++) {
            if(itrk == itrk2) continue;
            hist_->fillHist1D("TrackMatch","DeltaXTrackTrack_withduplicate",  (telEv()->xPos->at(itrk) - telEv()->xPos->at(itrk2)) );
            hist_->fillHist1D("TrackMatch","DeltaYTrackTrack_withduplicate",  (telEv()->yPos->at(itrk) - telEv()->yPos->at(itrk2)) );
            hist_->fillHist1D("TrackMatch","DeltaXTrackTrack_withduplicate_zoom",  (telEv()->xPos->at(itrk) - telEv()->xPos->at(itrk2)) );
            hist_->fillHist1D("TrackMatch","DeltaYTrackTrack_withduplicate_zoom",  (telEv()->yPos->at(itrk) - telEv()->yPos->at(itrk2)) );

            if( fabs((telEv()->xPos->at(itrk) - telEv()->xPos->at(itrk2))) < 0.001){
              foundDuplicate = true;
              thetrackN = itrk2;
            }
          }
          if(foundDuplicate != true || (foundDuplicate == true && itrk <thetrackN)  ) {

            double tkX = telEv()->xPos->at(itrk);
            double tkY = telEv()->yPos->at(itrk);
            tbeam::Track t(itrk,tkX,tkY,telEv()->dxdz->at(itrk),telEv()->dydz->at(itrk),telEv()->chi2->at(itrk),telEv()->ndof->at(itrk));
            noDuplicate_Me_Trkcoll.push_back(t);

          }
        }
        */

        hist_->fillHist1D("TrackMatch", "NTracks" , selectedTk.size());
        for(unsigned int itrk = 0; itrk<selectedTk.size();itrk++) {

          hist_->fillHist1D("TrackMatch","track_chi2",     selectedTk[itrk].chi2 );
          hist_->fillHist1D("TrackMatch","track_ndf",      selectedTk[itrk].ndof );
          hist_->fillHist1D("TrackMatch","track_normchi2", selectedTk[itrk].chi2/selectedTk[itrk].ndof );
        }


	// hist_->fillHist1D("TrackMatch", "NTrack_myDuplicate",   noDuplicate_Me_Trkcoll.size());
        hist_->fillHist1D("TrackMatch", "NTrack_NicoDuplicate", tkNoOv.size());
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

        double xtkStripDet0 = 999;
        double xtkStripDet1 = 999;

        for(unsigned int itrack=0; itrack <  fidTrkcoll.size(); itrack++){

          hist_->fillHist1D("TrackMatch","track_chi2",     fidTrkcoll[itrack].chi2);
          hist_->fillHist1D("TrackMatch","track_ndf",      fidTrkcoll[itrack].ndof);
          hist_->fillHist1D("TrackMatch","track_normchi2", fidTrkcoll[itrack].chi2/fidTrkcoll[itrack].ndof);


          double x0_DUT = fidTrkcoll[itrack].xtkDut0;
          double x1_DUT = fidTrkcoll[itrack].xtkDut1;

          xtkStripDet0 = (x0_DUT/0.09) + 127;
          xtkStripDet1 = (x1_DUT/0.09) + 127;


      	  bool match_X0DUT = false;
      	  bool match_X1DUT = false;
      	  bool asStub = false;
      	  if(dutRecoStubmap()->at("C0").size()) asStub = true;




      	  for(unsigned int icl=0; icl < (dutRecoClmap()->at("det0C0")).size(); icl++ ) {

      	    double theclsX  = ((dutRecoClmap()->at("det0C0"))[icl].x -nstrips()/2)*dutpitch();
      	    double cltW     =  (dutRecoClmap()->at("det0C0"))[icl].size;
            double res = x0_DUT - theclsX ;


            if( std::fabs( res ) <= 4*resDUT() ) match_X0DUT = true;
            if(!match_X0DUT) continue;
        	  hist_->fillHist1D("TrackMatch","ResX0", res);
      	    //cout << "res " << res << endl;
      	    //cout << "resDUT() " << resDUT()<< endl;

      	    hist_->fillHist1D("TrackMatch","X0",  theclsX);
            hist_->fillHist1D("TrackMatch","X0_nstrip",  xtkStripDet0);
      	    if(asStub) hist_->fillHist1D("TrackMatch","X0_withStub",  theclsX);
      	    if(asStub) hist_->fillHist1D("TrackMatch","X0_withStub_nstrip",  xtkStripDet0);

      	    hist_->fillHist1D("TrackMatch","ClsWidthX0",  cltW);
        	  hist_->fillHist2D("TrackMatch", "ClsWidthX0_vs_X0",  cltW, theclsX);
        	  hist_->fillHist2D("TrackMatch", "ClsWidthX0_vs_X0_nstrip",  cltW, xtkStripDet0);

        	  hist_->fillHist2D("TrackMatch","ResX0_vs_X0", res, theclsX);
            hist_->fillHist2D("TrackMatch","ResX0_vs_X0_nstrip", res, xtkStripDet0);


      	    for(unsigned int icl=0; icl < (dutRecoClmap()->at("det1C0")).size(); icl++ ) {

      	      double theclsX1 =  ((dutRecoClmap()->at("det1C0"))[icl].x -nstrips()/2)*dutpitch();
              double res1 = x1_DUT - theclsX ;
      	      hist_->fillHist2D("TrackMatch","X0_vs_X1",        theclsX, theclsX1);
      	      hist_->fillHist2D("TrackMatch","X0_vs_X1_nstrip", xtkStripDet0, xtkStripDet1);
      	      hist_->fillHist1D("TrackMatch","DeltaX01", theclsX -theclsX1 );
      	      if(asStub) hist_->fillHist1D("TrackMatch","DeltaX01_withStub", theclsX -theclsX1 );

      	      hist_->fillHist2D("TrackMatch","ResX0_vs_DeltaX01", res,  theclsX -theclsX1 );
      	      hist_->fillHist2D("TrackMatch","ResX1_vs_DeltaX01", res1, theclsX -theclsX1 );

              }

          }
      	  for(unsigned int icl=0; icl < (dutRecoClmap()->at("det1C0")).size(); icl++ ) {

      	    double theclsX =  ((dutRecoClmap()->at("det1C0"))[icl].x -nstrips()/2)*dutpitch();
      	    double cltW     =  (dutRecoClmap()->at("det1C0"))[icl].size;
            double res = x1_DUT - theclsX ;
            if( std::fabs( res ) <= 4*resDUT() ) match_X1DUT = true;
            if(!match_X1DUT)  continue;

        	  hist_->fillHist1D("TrackMatch", "ResX1",                    res);
            hist_->fillHist1D("TrackMatch", "ClsWidthX1",               cltW);
        	  hist_->fillHist2D("TrackMatch", "ClsWidthX1_vs_X1",         cltW, theclsX);
        	  hist_->fillHist2D("TrackMatch", "ClsWidthX1_vs_X1_nstrip",  cltW, xtkStripDet1);


      	    hist_->fillHist1D("TrackMatch","X1",  theclsX);
            hist_->fillHist1D("TrackMatch","X1_nstrip",  xtkStripDet1);
      	    if(asStub) hist_->fillHist1D("TrackMatch","X1_withStub",  theclsX);
      	    if(asStub) hist_->fillHist1D("TrackMatch","X1_withStub_nstrip",  xtkStripDet1);
        	  hist_->fillHist2D("TrackMatch","ResX1_vs_X1", res, theclsX);
        	  hist_->fillHist2D("TrackMatch","ResX1_vs_X1_nstrip", res, xtkStripDet1);

          }

      	  //cout << "match_X0DUT match_X1DUT " << match_X0DUT << " " << match_X1DUT << endl;
      	  if(match_X0DUT && match_X1DUT ) {
      	        //cout << "in proper fill " << endl;
      	  	hist_->fillHist1D("TrackMatch","X0X1Clst_per_Track", 2.);
      		  hist_->fillHist2D("TrackMatch","X0X1Clst_per_Track_vsX0", 2, x0_DUT);
      		  hist_->fillHist2D("TrackMatch","X0X1Clst_per_Track_vsX0_nstrip", 2, xtkStripDet0);
      		  hist_->fillHist2D("TrackMatch","X0X1Clst_per_Track_vsX1", 2, x1_DUT);
      		  hist_->fillHist2D("TrackMatch","X0X1Clst_per_Track_vsX1_nstrip", 2, xtkStripDet1);
      	  }else{
      	  	hist_->fillHist1D("TrackMatch","X0X1Clst_per_Track", 1.);
      		  hist_->fillHist2D("TrackMatch","X0X1Clst_per_Track_vsX0", 1, x0_DUT);
      		  hist_->fillHist2D("TrackMatch","X0X1Clst_per_Track_vsX0_nstrip", 1, xtkStripDet0);
      		  hist_->fillHist2D("TrackMatch","X0X1Clst_per_Track_vsX1", 1, x1_DUT);
      		  hist_->fillHist2D("TrackMatch","X0X1Clst_per_Track_vsX1_nstrip", 1, xtkStripDet1);
      	  }



      	}





        for(auto &tk : fidTrkcoll) {
          double x0 = tk.xtkDut0;
	        xtkStripDet0 = (x0/0.09) + 127;
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
	        xtkStripDet1 = (x1/0.09) + 127;
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
        hist_->fillHist1D("TrackMatch", "HitEffD0_vs_X_den", xtkStripDet0);
        hist_->fillHist1D("TrackMatch", "HitEffD1_vs_X_den", xtkStripDet1);
        hist_->fillHist1D("TrackMatch", "StubEff_vs_X_den",  xtkStripDet0);
        hist_->fillHist1D("TrackMatch","effVtdc_den",static_cast<unsigned int>(condEv()->tdcPhase));
        if(trkClsmatchD0)   {
          det0clsMatch++;
          hist_->fillHist1D("TrackMatch", "trkcluseff", 4);
	        hist_->fillHist1D("TrackMatch", "HitEffD0_vs_X_num", xtkStripDet0);
        }
        if(trkClsmatchD1)   {
          det1clsMatch++;
          hist_->fillHist1D("TrackMatch", "HitEffD1_vs_X_num", xtkStripDet1);
          hist_->fillHist1D("TrackMatch", "trkcluseff", 5);
        }
        if(trkClsmatchD0 || trkClsmatchD1)   clsMatchany++;
        if(trkClsmatchD0 && trkClsmatchD1)   {
          clsMatchboth++;
          hist_->fillHist1D("TrackMatch", "trkcluseff", 6);
          hist_->fillHist1D("TrackMatch","effVtdc_num",static_cast<unsigned int>(condEv()->tdcPhase));
        }
        if(trkClsmatchD0 && trkClsmatchD1 && smatchD1)  hist_->fillHist1D("TrackMatch", "StubEff_vs_X_num", xtkStripDet0);

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
