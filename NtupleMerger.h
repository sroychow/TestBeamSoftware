#ifndef NtupleMerger_h
#define NtupleMerger_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TTree.h>

#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
#include<stdint.h>
#include <map>
#include <string>

#include "DataFormats.h"
#include "TSystem.h"
#include "TChain.h"
#include "TH1I.h"
#include "TH1D.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::cerr;


struct cbcCond {
    uint32_t l1Accept_;
    uint32_t tdcCounter_;
    uint32_t totalHits_;  
    uint32_t totalStubs_;  
    bool eventFlag_;
    uint32_t eventCountCBC_;
    std::vector<unsigned int> cbcErrorVal_;
    std::vector<unsigned int> cbcPLAddressVal_;
    std::vector<unsigned int> dut0C0chData_;
    std::vector<unsigned int> dut0C1chData_;
    std::vector<unsigned int> dut1C0chData_;
    std::vector<unsigned int> dut1C1chData_;
};


class NtupleMerger {
  public :
    NtupleMerger(const std::string dutTuple, const std::string telTuple, const std::string dqmTuple, const std::string runNumber);
    virtual ~NtupleMerger();

    bool branchFound(TTree* chain,const string& b,std::vector<std::string>& brList_);
    void setInputBranchAddresses();
    void bookValidationHistograms(const std::string run);

    void filltrigTrackmap();
    void eventLoop();
    //void clearEvent();
    void endJob();
  private :
    TFile* dutF_;
    TFile* telF_;
    TFile* dqmF_;
    TTree *dutchain_; 
    TTree *telchain_;
    TTree *dqmchain_;
    TTree* outTree_;
    TFile* fout_;
    bool telFound_; 
    //input format
    std::vector<unsigned int>* cbcErrorVal_;
    std::vector<unsigned int>* cbcPLAddressVal_;
    unsigned int tdcCounterFromdqm_;  
    tbeam::TelescopeEvent* telEvent_;
    bool periodicityFlag_;
    bool pFlag_;
    bool goodEventFlag_;
    unsigned int l1adqm_;
    unsigned int totalHitsdqm_;
    unsigned int totalStubsdqm_;
    unsigned int evCountcbc_;
    std::vector<unsigned int>* dut0C0chData_;
    std::vector<unsigned int>* dut0C1chData_;
    std::vector<unsigned int>* dut1C0chData_;
    std::vector<unsigned int>* dut1C1chData_;

    //branches to update
    //TBranch* condBranch_;
    TBranch* telBranch_;
    TBranch* periodicityBranch_;
    TBranch* goodEvBranch_;
    
    //TBranch* eventQualityBranch_; 

    //output format
    
    //
    tbeam::TelescopeEvent* telOutputEvent_;
    tbeam::condEvent* condEvent_;
    tbeam::dutEvent* dutEvent_;
    tbeam::dutEvent* outdutEvent_;
    tbeam::condEvent* outcondEvent_;
    //TBranch* condBranch_;

    std::map<Int_t,tbeam::TelescopeEvent>* trigTrackmap_;
    std::map<long int,cbcCond>*  cbcCondmap_;
    long int nTelchainentry_;
    long int nDutchainentry_; 
    long int nDqmchainentry_; 
    long int nEventstoLoop_;
    //validation histograms
    TFile* fval;
    TH1D* tdc1;
    TH1D* tdc2;
    TH1D* tdc3;
    TH1D* tdc4;
    TH1D* tdc5;
    TH1D* tdc6;
    TH1I* eventFilter;
    TH1I* hitmatch;    
    TH1D* hitdqm;
    TH1D* hitedm;
    bool  debug_;
};
#endif
