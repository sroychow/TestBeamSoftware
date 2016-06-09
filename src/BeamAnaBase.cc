#include "BeamAnaBase.h"
#include "Utility.h"
#include "TSystem.h"
#include "TChain.h"

BeamAnaBase::BeamAnaBase() :
  fin_(nullptr),
  analysisTree_(nullptr),
  dutEv_(new tbeam::dutEvent()),
  condEv_(new tbeam::condEvent()),
  telEv_(new  tbeam::TelescopeEvent()),
  periodcictyF_(false),
  isGood_(false),
  sw_(-1),
  offset1_(-1),
  offset2_(-1),
  cwd_(-1),
  dut0_chtempC0_(new std::vector<int>()),
  dut0_chtempC1_(new std::vector<int>()),
  dut1_chtempC0_(new std::vector<int>()),
  dut1_chtempC1_(new std::vector<int>()),
  dutRecoClmap_(new std::map<std::string,std::vector<tbeam::cluster>>),
  dutRecoStubmap_(new std::map<std::string,std::vector<tbeam::stub>>),
  recostubChipids_(new std::map<std::string,std::vector<unsigned int>>()),
  cbcstubChipids_(new std::map<std::string,std::vector<unsigned int>>()),
  nStubsrecoSword_(0),
  nStubscbcSword_(0)
{
  dutRecoClmap_->insert({("det0C0"),std::vector<tbeam::cluster>()});
  dutRecoClmap_->insert({("det0C1"),std::vector<tbeam::cluster>()});
  dutRecoClmap_->insert({("det1C0"),std::vector<tbeam::cluster>()});
  dutRecoClmap_->insert({("det1C1"),std::vector<tbeam::cluster>()});
  dutRecoStubmap_->insert({("C0"),std::vector<tbeam::stub>()});
  dutRecoStubmap_->insert({("C1"),std::vector<tbeam::stub>()});
  recostubChipids_->insert({("C0"),std::vector<unsigned int>()});
  recostubChipids_->insert({("C1"),std::vector<unsigned int>()});
  cbcstubChipids_->insert({("C0"),std::vector<unsigned int>()});
  cbcstubChipids_->insert({("C1"),std::vector<unsigned int>()});
}

bool BeamAnaBase::setInputFile(const std::string& fname) {
  fin_ = TFile::Open(fname.c_str());
  if(!fin_)    {
    std::cout <<  "File " << fname << " could not be opened!!" << std::endl;
    return false;
  }
  analysisTree_ = dynamic_cast<TTree*>(fin_->Get("analysisTree"));
  if(analysisTree_)    return true;
  return false; 
}

void BeamAnaBase::setAddresses() {
  //set the address of the DUT tree
  analysisTree_->SetBranchAddress("DUT", &dutEv_);
  analysisTree_->SetBranchAddress("Condition", &condEv_);
  analysisTree_->SetBranchAddress("TelescopeEvent",&telEv_);
  analysisTree_->SetBranchAddress("periodicityFlag",&periodcictyF_);
  analysisTree_->SetBranchAddress("goodEventFlag",&isGood_);
  analysisTree_->SetBranchStatus("*",1);
}

void BeamAnaBase::setDetChannelVectors() {
  //std::cout << "setP1" << std::endl;
  if( dutEv_->dut_channel.find("det0") != dutEv_->dut_channel.end() ) {
      for( unsigned int j = 0; j<(dutEv_->dut_channel.at("det0")).size(); j++ ) {
        int ch = (dutEv_->dut_channel.at("det0")).at(j);
	if( ch <= 1015 )  dut0_chtempC0_->push_back(ch);
	else dut0_chtempC1_->push_back(ch-1016);
      }
  }
  if( dutEv_->dut_channel.find("det1") != dutEv_->dut_channel.end() ) {
      for( unsigned int j = 0; j<(dutEv_->dut_channel.at("det1")).size(); j++ ) {
        int ch = (dutEv_->dut_channel.at("det1")).at(j);
        if( ch <= 1015 )  dut1_chtempC0_->push_back(ch);
        else  dut1_chtempC1_->push_back(ch-1016);
      }
  }
  //std::cout << "setP2" << std::endl;
  for(auto& cl : (dutEv_->clusters)){
    std::string ckey = cl.first;//keys are det0 and det1

    for(auto& c : cl.second)  {
      if(c->x <= 1015)  dutRecoClmap_->at(ckey +"C0").push_back(*c);
      else {
        auto ctemp = *c;
        ctemp.x -= 1016;//even for column 1 we fill histograms between 0 and 1015 
        dutRecoClmap_->at( ckey + "C1").push_back(ctemp);
      }
    }    
  }
  //std::cout << "setP3" << std::endl;
  for(auto& s : dutEv_->stubs) {
    tbeam::stub st = *s;
    if(st.x <= 1015)   dutRecoStubmap_->at("C0").push_back(st);
    else dutRecoStubmap_->at("C1").push_back(st);
  }
  
  //for(auto& t:*recostubChipids_) std::cout << t.first << ",";
  //for(auto& t:*cbcstubChipids_) std::cout << t.first << ",";
  nStubsrecoSword_ = Utility::readStubWord(*recostubChipids_,dutEv_->stubWordReco);
  nStubscbcSword_ = Utility::readStubWord(*cbcstubChipids_,dutEv_->stubWord);
  //std::cout << "Leaving set" << std::endl;
}


void BeamAnaBase::getCbcConfig(uint32_t cwdWord, uint32_t windowWord){
   sw_ = windowWord >>4;
   offset1_ = (cwdWord)%4;
   if ((cwdWord>>2)%2) offset1_ = -offset1_;
   offset2_ = (cwdWord>>3)%4;
   if ((cwdWord>>5)%2) offset2_ = -offset2_;
   cwd_ = (cwdWord>>6)%4;
}

void BeamAnaBase::endJob() {
  
}
void BeamAnaBase::clearEvent() {
  dut0_chtempC0_->clear();
  dut0_chtempC1_->clear();
  dut1_chtempC0_->clear();
  dut1_chtempC1_->clear();
  for(auto& c: *dutRecoClmap_)
    c.second.clear();
  for(auto& s: *dutRecoStubmap_)
    s.second.clear();
  for(auto& rs : *recostubChipids_)
    rs.second.clear();
  for(auto& rs : *cbcstubChipids_)
    rs.second.clear();
  nStubsrecoSword_ = 0;
  nStubscbcSword_ = 0;
}

BeamAnaBase::~BeamAnaBase() {
}
