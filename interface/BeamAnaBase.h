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

static const double z_DUT0 = 435.0;
static const double z_DUT1 = 438.1;  
static const double z_FEI4 = 724.0;

class BeamAnaBase {
  public :
    BeamAnaBase();
    virtual ~BeamAnaBase();
    bool setInputFile(const std::string& fname);
    bool branchFound(const string& b);
    void setAddresses();
    void setDetChannelVectors();
    TTree* analysisTree() const{ return analysisTree_; } 
    tbeam::dutEvent* dutEv() const { return dutEv_; }
    tbeam::condEvent* condEv() const {return condEv_;}
    tbeam::TelescopeEvent* telEv() const { return telEv_; }
    tbeam::FeIFourEvent* fei4Ev() const { return fei4Ev_; }
    bool isPeriodic() const { return periodcictyF_;}
    bool isGoodEvent() const { return isGood_;}
    int stubWindow()  const { return sw_;}
    int cbcClusterWidth()  const { return cwd_;}
    int cbcOffset1() const {return offset1_;}
    int cbcOffset2() const {return offset2_;}
    vector<int>* det0C0() const { return dut0_chtempC0_;}
    vector<int>* det0C1() const { return dut0_chtempC1_;}
    vector<int>* det1C0() const { return dut1_chtempC0_;}
    vector<int>* det1C1() const { return dut1_chtempC1_;}
    std::map<std::string,std::vector<tbeam::cluster>>* dutRecoClmap() const {return dutRecoClmap_;} 
    std::map<std::string,std::vector<tbeam::stub> >* dutRecoStubmap() const {return dutRecoStubmap_;};
    std::map<std::string,std::vector<unsigned int>>* recostubChipids() const { return recostubChipids_;}
    std::map<std::string,std::vector<unsigned int>>* cbcstubChipids() const { return cbcstubChipids_;}
    int nStubsrecoSword() const { return nStubsrecoSword_;}
    int nStubscbcSword() const { return nStubscbcSword_;}
    bool hasTelescope() const { return hasTelescope_;}
    virtual void beginJob(){}
    virtual void endJob();
    virtual void eventLoop() = 0; 
    virtual void bookHistograms() = 0;
    virtual void clearEvent();
    void getCbcConfig(uint32_t cwdWord, uint32_t windowWord);
    void getExtrapolatedTracks(std::vector<double>& xTkdut0, std::vector<double>& xTkdut1);
    void readChannelMaskData(const std::string cmaskF);
    void setTelMatching(const bool mtel);
    void setChannelMasking(const bool mch, const std::string cFile);
    bool doTelMatching() const { return doTelMatching_;}
    bool doChannelMasking() const { return doChannelMasking_;}
    std::map<std::string,std::vector<int> >* getMaskedChannelMap() const {return dut_maskedChannels_;}
    void readAlignmentConstant(const std::string& aFname);
    tbeam::alignmentPars aLparameteres() const { return alPars_; }
    bool isTrkfiducial(const double xtrkPos, int& xtkdutStrip, const std::string det);
  private :
    TFile* fin_;
    TTree *analysisTree_; 
    tbeam::dutEvent* dutEv_;
    tbeam::condEvent* condEv_;
    tbeam::TelescopeEvent* telEv_;
    tbeam::FeIFourEvent* fei4Ev_;
    bool periodcictyF_;
    bool isGood_;
    bool hasTelescope_;
    bool doTelMatching_;
    bool doChannelMasking_;
    int sw_;
    int offset1_;
    int offset2_;
    int cwd_;
    vector<int>* dut0_chtempC0_;
    vector<int>* dut0_chtempC1_;
    vector<int>* dut1_chtempC0_;
    vector<int>* dut1_chtempC1_;
    std::map<std::string,std::vector<tbeam::cluster> >* dutRecoClmap_;
    std::map<std::string,std::vector<tbeam::stub> >* dutRecoStubmap_;
    std::map<std::string,std::vector<unsigned int>>* recostubChipids_;
    std::map<std::string,std::vector<unsigned int>>* cbcstubChipids_;
    std::map<int,std::vector<int>>  cbcMaskedChannelsMap_;
    std::map<std::string,std::vector<int> >* dut_maskedChannels_;
    int nStubsrecoSword_;
    int nStubscbcSword_;
    tbeam::alignmentPars  alPars_;
    //std::map<std::string,std::vector<unsigned int>>* dutCbcStubmap_;
};
#endif
