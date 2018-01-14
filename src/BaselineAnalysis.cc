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
  hist_->bookEventHistograms();

  //BeamAnaBase::bookHistograms();

  for(auto& m : *modVec()){
    hist_->bookDUTHistograms(m.hdirLower_);
    hist_->bookDUTHistograms(m.hdirUpper_);
    hist_->bookStubHistograms(m.name);
    hist_->bookCorrelationHistograms(m.name);
    std::cout << "DetidLower=" << m.hdirLower_ << std::endl;
    TString s = TString(m.hdirLower_);
    hist_->bookTrackMatchHistograms(s);
    s = TString(m.hdirUpper_);
    hist_->bookTrackMatchHistograms(s);
  }
  //book common histograms of track propoerties
  hist_->bookTrackCommonHistograms();
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
  maxEvent_ = getMaxEvt();
  if(nEntries_ < maxEvent_ || maxEvent_ == 0) maxEvent_ = nEntries_;
  cout << "#Events=" << nEntries_ <<" -->  MAX EVENTS TO BE PROCESSED : "<< maxEvent_ <<endl;
  hist_->fillHist1D("EventInfo","nevents", nEntries_);

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
      cout << "Alignment Parameters" << aLparameteres();
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
    for(auto& m : *modVec()){
      std::string dnameLower = m.hdirLower_ + "/TrackMatch";
      //loop over tracks
      for(auto& tk: event()->tracks) {
        //previous hit
        float xtk_prev = tk.xPosPrevHit() + std::abs(m.z - telPlaneprev_.z)*tk.dxdz();
        float ytk_prev = tk.yPosPrevHit() + std::abs(m.z - telPlaneprev_.z)*tk.dydz();
        hist_->fillHist1D(dnameLower, "tkposx_prev", xtk_prev/1000.);
        hist_->fillHist1D(dnameLower, "tkposy_prev", ytk_prev/1000.);
        //next hit
        float xtk_next = tk.xPosNextHit() + std::abs(m.z - telPlaneprev_.z)*tk.dxdz();
        float ytk_next = tk.yPosNextHit() + std::abs(m.z - telPlaneprev_.z)*tk.dydz();
        hist_->fillHist1D(dnameLower, "tkposx_next", xtk_next/1000.);
        hist_->fillHist1D(dnameLower, "tkposy_next", ytk_next/1000.);
        //Fill hit residuals
        for(auto& h: m.lowerHits) {
          float hx = (h.strip() - m.nstrips_/2.)*m.pitch_;
          hist_->fillHist1D(dnameLower, "hitresidualX_prev", hx - xtk_prev/1000.);
          hist_->fillHist1D(dnameLower, "hitresidualX_next", hx - xtk_next/1000.);
        }
        //Fill cluster residuals
        for(auto& c: m.lowerOfflineCls) {
          float cx = (c.center() - m.nstrips_/2.)*m.pitch_;
          hist_->fillHist1D(dnameLower, "clusresidualX_prev", cx - xtk_prev/1000.);
          hist_->fillHist1D(dnameLower, "clusresidualX_next", cx - xtk_next/1000.);
        }
      }
    }
  }

}

void BaselineAnalysis::fillCommonHistograms() {
  //fill sensor hits, cluster
  for(auto& m : *modVec()) {
    //Fill hit size
    hist_->fillHist1D(m.hdirLower_,"chsizeC0", m.lowerHits.size());
    hist_->fillHist1D(m.hdirUpper_,"chsizeC0", m.upperHits.size());
    //Fill lower sensor hit info
    for(auto& h: m.lowerHits) {
      hist_->fillHist1D(m.hdirLower_ ,"hitmapC0", h.strip());//put check to fill c1 histograms for full module
      hist_->fillHist1D(m.hdirLower_ ,"hitmapXposC0", (h.strip() - m.nstrips_/2.)*m.pitch_);
      std::cout << "Hit (lower) =" << h.strip() << " in mm=" << (float(h.strip()) - 127.)*0.09 << "\n";
      for(auto& hup: m.upperHits)  hist_->fillHist2D(m.name+"/Correlation", "hitposcorrelationC0", h.strip(), hup.strip());
    }
    //Fill upper sensor hit info
    for(auto& h: m.upperHits) {
      hist_->fillHist1D(m.hdirUpper_ ,"hitmapC0", h.strip());//put check to fill c1 histograms for full module
      std::cout << "Hit (upper) =" << h.strip() << " in mm=" << (float(h.strip()) - 127)*0.09<< "\n";
    }
    //Offline clusters only
    //hist_->fillClusterHistograms(m.hdirLower_, m.lowerOfflineCls, "C0");
    hist_->fillHist1D( m.hdirLower_, "nclusterC0", m.lowerOfflineCls.size() );
    for( auto& cl : m.lowerOfflineCls) {
      Utility::fillHist1D( "clusterWidthC0", cl.size() );
      Utility::fillHist1D( "clusterPosC0", cl.center() );
      Utility::fillHist1D( "clusterXPosC0", (cl.center() - m.nstrips_/2.)*m.pitch_ );
      Utility::fillHistProfile( "clusterWidthVsPosProfC0", cl.center(), cl.size() );
      Utility::fillHist2D( "clusterWidthVsPos2DC0", cl.center(), cl.size() );
    }



    hist_->fillHist2D(m.hdirLower_,"nhitvsnclusC0", m.lowerHits.size(), m.lowerOfflineCls.size());
    hist_->fillClusterHistograms(m.hdirUpper_, m.upperOfflineCls, "C0");
    hist_->fillHist2D(m.hdirUpper_,"nhitvsnclusC0", m.upperHits.size(), m.upperOfflineCls.size());
    //correlation histo for clusters
    for(auto& lcls : m.lowerOfflineCls) {
        std::cout << "Offline clus pos(lower) =" << lcls.center() << " in mm=" << (lcls.center() - 127)*0.09<< "\n";
      for(auto& ucls : m.upperOfflineCls) {
         std::cout << "Offline clus pos(upper) =" << ucls.center() << " in mm=" << (ucls.center() - 127)*0.09 << "\n";
         hist_->fillHist2D(m.name+"/Correlation", "clusterposcorrelationC0", lcls.center(), ucls.center());
      }
    }
    //Fill stub histos
    std::string sdname = m.name + "/StubInfo";
    hist_->fillHist1D(sdname,"nstubsFromCBC",  m.cbcStubs.size());
    hist_->fillHist1D(sdname,"nstubsFromReco", m.offlineStubs.size());
    hist_->fillHist2D(sdname,"nstubMatch", m.offlineStubs.size(), m.cbcStubs.size());
    for(auto& os : m.offlineStubs) {
      hist_->fillHist1D(sdname,"offlinestubPosmap", os.positionX());
      std::cout << "Offline stub pos =" << os.positionX() << " in mm=" << (os.positionX() - 127.)*0.09<< "\n";
      for(auto& cs : m.cbcStubs) {
        hist_->fillHist2D(sdname,"stubCorrelation", os.positionX(), cs.positionX());
      }
    }
    for(auto& cs : m.cbcStubs) {
      hist_->fillHist1D(sdname,"cbcstubPosmap", cs.positionX());
    }
  }
  //Fill common track histograms
  hist_->fillHist1D("TrackCommon","nTracks", event()->tracks.size());
  for(auto& tk: event()->tracks) {
    std::cout << "XPos tk=" << tk.xPos() << std::endl;
    hist_->fillHist1D("TrackCommon","tkXPosref", tk.xPos());
    hist_->fillHist1D("TrackCommon","tkYPosref", tk.yPos());
    hist_->fillHist1D("TrackCommon","errtkXPosref", tk.xPosErr());
    hist_->fillHist1D("TrackCommon","errtkYPosref", tk.yPosErr());


    hist_->fillHist1D("TrackCommon","tkXPosprev", tk.xPosPrevHit());
    hist_->fillHist1D("TrackCommon","tkYPosprev", tk.yPosPrevHit());
    hist_->fillHist1D("TrackCommon","errtkXPosprev", tk.xPosErrsPrevHit());
    hist_->fillHist1D("TrackCommon","errtkYPosprev", tk.yPosErrsPrevHit());

    hist_->fillHist1D("TrackCommon","tkXPosnext", tk.xPosNextHit());
    hist_->fillHist1D("TrackCommon","tkYPosnext", tk.yPosNextHit());
    hist_->fillHist1D("TrackCommon","errtkXPosnext", tk.xPosErrNextHit());
    hist_->fillHist1D("TrackCommon","errtkYPosnext", tk.yPosErrNextHit());

    hist_->fillHist1D("TrackCommon","tkChi2", tk.chi2());
    hist_->fillHist1D("TrackCommon","tkdXdZ", tk.dxdz());
    hist_->fillHist1D("TrackCommon","tkdYdZ", tk.dydz());
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
