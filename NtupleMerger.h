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

#include <map>
#include <string>

#include "DataFormats.h"
//#include "AnalysisObjects.h"
#include "TSystem.h"
#include "TChain.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::cerr;

namespace inputFormat {
  
}
class NtupleMerger {
  public :
    NtupleMerger(const std::string dutTuple, const std::string telTuple, const std::string dqmTuple);
    virtual ~NtupleMerger();
    int setDUTInputFile(const std::string& fname);
    int setTelescopeInputFile(const std::string& fname);
    int setDQMInputFile(const std::string& fname);

    bool branchFound(TChain* chain,const string& b,std::vector<std::string>& brList_);
    void setInputBranchAddresses();

    int getDUTEntry(int lflag) const;
    int getTelEntry(int lflag) const;
    int getDQMEntry(int lflag) const;

    void filltrigTrackmap();
    void eventLoop();
    //void clearEvent();
    void endJob();
  private :
    TChain *dutchain_; 
    TChain *telchain_;
    TChain *dqmchain_;
    TTree* outTuple_;
     
    //input format
    std::vector<int>* cbcErrorVal_;
    std::vector<int>* cbcPLAddressVal_;  
    tbeam::TelescopeEvent* telEvent_;
    //input branchlist
    std::vector<std::string> dutbrList_;
    std::vector<std::string> telbrList_;
    std::vector<std::string> dqmbrList_;

    //output format
    tbeam::DutEvent* dutEvent_;
    tbeam::TelescopeEvent* telOutputEvent_;
    tbeam::CondEvent* condEvent_;

    std::map<Int_t,tbeam::TelescopeEvent>* trigTrackmap_;
    long int nTelchainentry_;
    long int nDutchainentry_; 
    long int nDqmchainentry_; 
    long int nEventstoLoop_;
};
#endif
