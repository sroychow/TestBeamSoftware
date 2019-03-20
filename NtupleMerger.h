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
#include "TelescopeEvent.h"
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
  
    void endJob();
    std::vector<tbeam::cluster>  offlineclusterizer(const std::vector<tbeam::hit>& hits, const unsigned int nCbc,
                          const unsigned int nChanPercbc, std::vector<tbeam::cluster>& clusVec);
    void stubSimulator (const std::vector<tbeam::cluster>& seeding, const std::vector<tbeam::cluster>& matching, std::vector<tbeam::stub>& stubVec);

   tbeam::Event ev_;

  private :
    TFile* dutF_;
    TFile* telF_;
    TTree *dutchain_; 
    TTree *telchain_;
    TTree* outTree_;
    TFile* fout_;
    bool telFound_;

    long int nDutchainentry_; 
    long int nEventstoLoop_;
    long int nTelchainentry_;

    //input format
    TelescopeEvent* telEvent_;
    tbeam::Event* dutEvent_;


    //output format    
    tbeam::Event* outdutEvent_;

    //Trigger Track Map
    std::multimap<Int_t,TelescopeEvent>* trigTrackmap_;


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
/*
 public:
    tbeam::hit *_hit0 (0, 0, 0, 0, false);
    tbeam::hit *_hit1 (0, 0, 0, 0, false);
    tbeam::stub *_stub (0, 0, 0.);
    tbeam::cluster *_cls (0, 0, 0, 0, 0, 0);

*/


};
#endif
