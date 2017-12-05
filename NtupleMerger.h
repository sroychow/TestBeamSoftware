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

#include "Event.h"
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



class NtupleMerger {
  public :
    NtupleMerger(const std::string dutTuple, const std::string telTuple, const std::string runNumber);

    virtual ~NtupleMerger();

    bool branchFound(TTree* chain,const string& b,std::vector<std::string>& brList_);
    void setInputBranchAddresses();
    //    void bookValidationHistograms(const std::string run);

    void filltrigTrackmap();
    void eventLoop();
    //void clearEvent();
    void endJob();
  private :
    TFile* dutF_;
    TFile* telF_;
    //    TFile* dqmF_;
    TTree *dutchain_; 
    TTree *telchain_;
    //    TTree *fei4chain_;
    //    TTree *dqmchain_;
    TTree* outTree_;
    TFile* fout_;
    bool telFound_;
    //    bool fei4Found_; 
    //input format
    tbeam::TelescopeEvent* telEvent_;
    tbeam::Event* dutEvent_;


    //output format
    
    //
    tbeam::TelescopeEvent* telOutputEvent_;
    tbeam::Event* outdutEvent_;

    std::map<Int_t,tbeam::TelescopeEvent>* trigTrackmap_;
    //std::map<Int_t,tbeam::FeIFourEvent>* trigFeI4map_;
    long int nTelchainentry_;
    long int nDutchainentry_; 
    long int nEventstoLoop_;
    /*
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
    bool  telOnly_;
    */
};
#endif
