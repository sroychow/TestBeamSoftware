#ifndef BeamAnaBase_h
#define BeamAnaBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TTree.h>

#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>

#include <map>
#include <string>

#include "DataFormats.h"
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::cerr;

class BeamAnaBase {
  public :
    BeamAnaBase();
    ~BeamAnaBase();
    int setDUTInputFile(const std::string& fname);
    int setTelescopeInputFile(const std::string& fname);
    bool branchFound(TChain* chain,const string& b);
    void setAddresses();
    TChain* telescopechain() const{ return telchain_; } 
    TChain* dutchain() const{ return dutchain_; } 
    void filltrigTrackmap();
    int getNtrack (const long int dutEvt) const;
    long int getTelEntries() const { return nTelchainentry; }
    long int getDutEntries() const { return nDutchainentry; }
    void setDetChannelVectors();    
    virtual void doClustering();
    virtual void findStub(const int stubWindow);
    vector<int>* dut0Ch0() const { return dut0_chtempC0_;}
    vector<int>* dut0Ch1() const { return dut0_chtempC1_;}
    vector<int>* dut1Ch0() const { return dut1_chtempC0_;}
    vector<int>* dut1Ch1() const { return dut1_chtempC1_;}
    std::map<std::string,std::vector<skbeam::Cluster> >* dutClustermap() const { return dutClustermap_; }
    std::map<std::string,std::vector<skbeam::Stub> >* dutRecoStubmap() const { return dutRecoStubmap_; }
    std::map<std::string,std::vector<unsigned int> >* dutCbcStubmap() const { return dutCbcStubmap_; }
    virtual void eventLoop() = 0; 
    virtual void bookHistograms() = 0;
    virtual void clearEvent();
    
  private :
    TChain *dutchain_; 
    TChain *telchain_;
    skbeam::tBeamBase* dutEvent_;
    skbeam::telescopeBase* telEvent_;
    std::map<long int,skbeam::telescopeBase*> trigTrackmap_;
    long int nTelchainentry;
    long int nDutchainentry;
    vector<int>* dut0_chtempC0_;
    vector<int>* dut0_chtempC1_;
    vector<int>* dut1_chtempC0_;
    vector<int>* dut1_chtempC1_;
    std::map<std::string,std::vector<skbeam::Cluster> >* dutClustermap_;
    std::map<std::string,std::vector<skbeam::Stub> >* dutRecoStubmap_;
    std::map<std::string,std::vector<unsigned int>>* dutCbcStubmap_;
    int nEventsNoHits;
    int nEventsHitInBoth;
    int nEventsHitInDet0;
    int nEventsHitInDet1;
};
#endif
