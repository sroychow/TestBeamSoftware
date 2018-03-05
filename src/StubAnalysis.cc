/*!
  \file                StubAnalysis.cc
  \brief               Stubs Analysis
  \author              Suvankar Roy Chowdhury, Rajarshi Bhattacharya, Alessandro Rossi
  \date                05/07/16
  Support :            mail to : suvankar.roy.chowdhury@cern.ch, rajarshi.bhattacharya@cern.ch, alessandro.rossi@cern.ch
*/
#define CW 6

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

#include "StubAnalysis.h"
using std::vector;
using std::map;
StubAnalysis::StubAnalysis() :
  BeamAnaBase::BeamAnaBase()
{
}
void StubAnalysis::bookHistograms() {
  BeamAnaBase::bookHistograms();
  hist_->bookStubsEfficiencyHistograms();
  for(auto& m : *modVec()){
    //std::cout << "Detidbottom=" << m.hdirbottom_ << std::endl;
    TString s = TString(m.hdirbottom_);
    hist_->bookTrackMatchHistograms(s);
    s = TString(m.hdirtop_);
    hist_->bookTrackMatchHistograms(s);
  }
}

void StubAnalysis::beginJob() {
  BeamAnaBase::beginJob();
  nEntries_ = analysisTree()->GetEntries();
  hist_ = outFile();
  setAddresses();
  bookHistograms();
  //analysisTree()->GetEntry(0);
  //getCbcConfig(condEv()->cwd, condEv()->window);
}

void StubAnalysis::eventLoop()
{
  Long64_t nbytes = 0, nb = 0;
  maxEvent_ = getMaxEvt();
  if(nEntries_ < maxEvent_ || maxEvent_ == 0) maxEvent_ = nEntries_;
  cout << "#Events=" << nEntries_ <<" -->  MAX EVENTS TO BE PROCESSED : "<< maxEvent_ <<endl;
  hist_->fillHist1D("EventInfo","nevents", nEntries_);

  long int den = 0;
  long int nClsb = 0, nClst = 0, numstub = 0, numcbcstub =0,numcbcstubBend =0;
  for (Long64_t jentry=0; jentry < maxEvent_; jentry++) {
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
      //hist_->fillHist1D("EventInfo","vcth", event()->vcth);
      //hist_->fillHist1D("EventInfo","offset", cbcOffset1());
      //hist_->fillHist1D("EventInfo","offset", cbcOffset2());
      //hist_->fillHist1D("EventInfo","window", event()->cwd);
      //hist_->fillHist1D("EventInfo","tilt", static_cast<unsigned long int>(condEv()->tilt));
      for(auto& cm: event()->conddatamap){
        uint8_t uid = (cm.first >> 24) & MASK_BITS_8;
        uint8_t i2cReg  = (cm.first >> 16)  & MASK_BITS_8;
        uint8_t i2cPage = (cm.first >> 12)  & MASK_BITS_4;
        uint8_t roId    = (cm.first >> 8)   & MASK_BITS_4;
        uint8_t feId    = (cm.first)        & MASK_BITS_8;
        std::cout << "uid=" << (int)uid
                  << ":i2cReg="<< (int)i2cReg
                  << ":i2cPage="<< (int)i2cPage
                  << ":roId="<< (int)roId
                  << ":feId="<< (int)feId
                  << ":value=" << (int)cm.second
                  << std::endl;
        //if(uid == 1)
        //  std::cout << "Value for uid=1>>>" << (int)cm.second << std::endl;
      }

    }
    hist_->fillHist1D("EventInfo","condData", static_cast<unsigned int>(event()->condData));
    hist_->fillHist1D("EventInfo","tdcPhase", static_cast<unsigned int>(event()->tdcPhase));
    //set the hits/cluster/stubs from the sensors in user accessable module
    setDetChannelVectors();
    //fill common histograms for dut hits, clusters
    fillCommonHistograms();
    //Fill track match histograms
    float resultBothPlanesConstraintShift[] = {-18.8072, -114101., 2400.02, -0.980538, 18.5594};
    //Select events with one track
    if ( event()->tracks.size() !=1 ) continue;
    auto& tk = event()->tracks[0];
    //Select tracks with chi2 > 5
    if (tk.chi2()>5.) continue;
    hist_->fillHist1D("StubEfficiency","stubeffglobal",0);    
    den++;
    auto& m = modVec()->at(0);

    std::string dnamebottom = m.hdirbottom_ + "/TrackMatch";
    std::string dnametop    = m.hdirtop_ + "/TrackMatch";
    //extrapolate the track. x direction is accross the strips
    //xTkAtDUT is a pair of the extrapolated position of the tk in the two planes of the module
    //xTkAtDUT.first will give the position of the track on  bottom sensor
    //xTkAtDUT.second will give the position of the track on top sensor
    //    std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(tk, m.z, offsetbottom(), zDUTbottom(), sensordeltaZ(), dutangle(), shiftPlanes(),bottomsensorPhi(),topsensorPhi());
    std::pair<double, double> xTkAtDUT_micron = Utility::extrapolateTrackAtDUTwithAngles(tk, telPlaneprev_.z, offsetbottom(), zDUTbottom(),
											 sensordeltaZ(), dutangle(), shiftPlanes(),
											 bottomsensorPhi(), topsensorPhi());

    std::pair<double, double> xTkAtDUT;
    xTkAtDUT.first  = xTkAtDUT_micron.first/1000.;
    xTkAtDUT.second = xTkAtDUT_micron.second/1000.;
    //extrapolate along y direction
    float yTkAtDUT_bottom = ( tk.yPosPrevHit() + (zDUTbottom() - m.z)*tk.dydz() )/1000.;
    float yTkAtDUT_top    = ( tk.yPosPrevHit() + (zDUTbottom() + sensordeltaZ() - m.z)*tk.dydz() )/1000.;
    hist_->fillHist1D(dnamebottom, "tkposx", xTkAtDUT.first);
    hist_->fillHist1D(dnametop,    "tkposx", xTkAtDUT.second); 
    //convert tk xpos to strip number
    int xTkAtDUT_strip_bottom = -1.*xTkAtDUT.first/m.pitch_  + m.nstrips_/2.;
    int xTkAtDUT_strip_top    = -1.*xTkAtDUT.second/m.pitch_ + m.nstrips_/2.;

    hist_->fillHist1D(dnamebottom, "trackpos_strip", xTkAtDUT_strip_bottom);
    hist_->fillHist1D(dnametop,    "trackpos_strip", xTkAtDUT_strip_top); 

    bool mCls_bottom = false;
    bool mCls_top = false;
    bool mOfflinestub = false;   
    float offlinestubPos=-999999;
    float offlinestubStrip=-999;
    bool mCBCstub = false;   
    bool mCBCstubBend = false;   
    float cbcstubPos=-999999;
    float cbcstubStrip=-999;
    float deltaXStubs=99999;
    float minclusStrip=-999;
    float minclusStriptop=-999;
    float deltaXclus=99999;
    //Build OfflineClusters
    std::vector<tbeam::stub> mystubs = Utility::offlineStub(m.topOfflineCls,m.bottomOfflineCls,CW);
    //    cout<<"-----> Stubs size event "<<jentry<<" : "<<mystubs.size()<<endl;

    //Fill cluster residuals
    for(auto& c: m.bottomOfflineCls) {
      float cx = -1.*(c.center() - m.nstrips_/2.)*m.pitch_;
      hist_->fillHist1D(dnamebottom, "clusresidualX", cx - xTkAtDUT.first);//xTkAtDUT.first since bottom sensor
      if(std::abs(cx - xTkAtDUT.first) < 0.1)    {
        mCls_bottom = true;
        hist_->fillHist2D(dnamebottom, "moduleSize_Cluster", c.center(), yTkAtDUT_bottom);
        hist_->fillHist1D(dnamebottom, "matchedclusterpos_strip", c.center());
      }
      if(std::abs(cx - xTkAtDUT.first) < deltaXclus){
	deltaXclus=std::abs(cx - xTkAtDUT.first);
	minclusStrip=c.center();
      }
    }

    deltaXclus=99999;
    for(auto& c: m.topOfflineCls) {
      float cx = -1.*(c.center() - m.nstrips_/2.)*m.pitch_;
      hist_->fillHist1D(dnametop, "clusresidualX", cx - xTkAtDUT.second);//xTkAtDUT.second since top sensor
      if(std::abs(cx - xTkAtDUT.second) < 0.1)  {
        mCls_top = true;
        hist_->fillHist2D(dnametop, "moduleSize_Cluster", c.center(), yTkAtDUT_top);
        hist_->fillHist1D(dnametop, "matchedclusterpos_strip", c.center());
      }
      if(std::abs(cx - xTkAtDUT.second) < deltaXclus){
	deltaXclus=std::abs(cx - xTkAtDUT.second);
	minclusStriptop=c.center();
      }
    }
    //stub Offline residual
    //    for(auto& s: m.offlineStubs) {
    for(auto& s: mystubs) {
      float sx = -1.*(float(s.positionX()/2) - m.nstrips_/2.)*m.pitch_;
      //      cout<<jentry<<" : "<<sx<<"   "<<xTkAtDUT.second<<"  "<<xTkAtDUT.first<<endl;
      hist_->fillHist1D(dnametop, "stubresidualX", sx - xTkAtDUT.second);//xTkAtDUT.first since bottom sensor
      if(std::abs(sx - xTkAtDUT.second) < 0.1){
	mOfflinestub = true;
	if(std::abs(sx - xTkAtDUT.second) < deltaXStubs){
          deltaXStubs=std::abs(sx - xTkAtDUT.first);
          offlinestubStrip=float(s.positionX()/2);
	  offlinestubPos=sx;
	}
      }
    }
    deltaXStubs=99999;
    //stub CBC residual
    for(auto& s: m.cbcStubs) {
      float sx = -1.*(float(s.positionX()/2) - m.nstrips_/2.)*m.pitch_;
      //      cout<<jentry<<" Bend Code Stub : "<<s.bendCode()<<"  "<<s.bend()<<endl;
      hist_->fillHist1D(dnametop, "CBCstubresidualX", sx - xTkAtDUT.second);//xTkAtDUT.second since bottom sensor
      if(std::abs(sx - xTkAtDUT.second) < 0.1){
        if(s.bendCode() != 7 && s.bendCode() != 8) mCBCstubBend = true;
	mCBCstub = true;
	if(std::abs(sx - xTkAtDUT.second) < deltaXStubs){
          deltaXStubs=std::abs(sx - xTkAtDUT.second);
          cbcstubStrip=(float)s.positionX()/2;
	  cbcstubPos=sx;
	}
      }

    }

    
    //Increment counters if matching is found
    if(mCls_bottom)   nClsb++;
    if(mCls_top)      nClst++;
    if(mOfflinestub){
      numstub++;
      hist_->fillHist1D("StubEfficiency","stubeffglobal",1);    
      //      hist_->fillHist1D("StubEfficiency","matchedOfflinestubspos_strip",offlinestubStrip);    
      hist_->fillHist1D("StubEfficiency","matchedOfflinestubspos_strip",xTkAtDUT_strip_bottom);    
    }
    if(mCBCstubBend){
      numcbcstubBend++;
      hist_->fillHist1D("StubEfficiency","stubeffglobal",3);    
      //      hist_->fillHist1D("StubEfficiency","matchedCBCstubspos_strip",cbcstubStrip);    
      hist_->fillHist1D("StubEfficiency","matchedCBCstubsBendpos_strip",xTkAtDUT_strip_bottom);
      if(xTkAtDUT_strip_bottom<127) hist_->fillHist1D("StubEfficiency","stubeffglobal",6);
      else if(xTkAtDUT_strip_bottom<255) hist_->fillHist1D("StubEfficiency","stubeffglobal",7);    
    }
    if(mCBCstub){
      numcbcstub++;
      hist_->fillHist1D("StubEfficiency","stubeffglobal",2);    
      //      hist_->fillHist1D("StubEfficiency","matchedCBCstubspos_strip",cbcstubStrip);    
      hist_->fillHist1D("StubEfficiency","matchedCBCstubspos_strip",xTkAtDUT_strip_bottom);    
    }
    if(xTkAtDUT_strip_bottom<127) hist_->fillHist1D("StubEfficiency","stubeffglobal",4);
    else if(xTkAtDUT_strip_bottom<255) hist_->fillHist1D("StubEfficiency","stubeffglobal",5);
    hist_->fillHist1D("StubEfficiency","ClosestClusPos_strip",minclusStrip);    
    hist_->fillHist1D("StubEfficiency","Trackpos_strip",xTkAtDUT_strip_bottom);    


  }//event Loop end
  std::cout << "Den=" << den << "\t#ClustersBottom=" << nClsb << "\t#ClustersTop=" << nClst << "\tOfflineStub=" << numstub  << "\tCBCStub=" << numcbcstub << "\tCBCStubBend=" << numcbcstubBend << std::endl;

}


void StubAnalysis::clearEvent() {
  BeamAnaBase::clearEvent();
}

void StubAnalysis::endJob() {
  BeamAnaBase::endJob();
  hist_->closeFile();
}

StubAnalysis::~StubAnalysis(){
  delete hist_;
}
