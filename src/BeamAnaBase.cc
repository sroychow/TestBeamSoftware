#include "BeamAnaBase.h"
#include "TSystem.h"
#include "TChain.h"

BeamAnaBase::BeamAnaBase() :
  dutchain_(new TChain("treeMaker/tbeamTree")),
  telchain_(new TChain("tracks")),
  dutEvent_(new skbeam::tBeamBase()),
  telEvent_(new skbeam::telescopeBase()),
  dut0_chtempC0(new std::vector<int>()),
  dut0_chtempC1(new std::vector<int>()),
  dut1_chtempC0(new std::vector<int>()),
  dut1_chtempC1(new std::vector<int>())
{
  nTelchainentry = -1;
  nDutchainentry = -1;
  nEventsNoHits = 0;
  nEventsHitInBoth = 0;
  nEventsHitInDet0 = 0;
  nEventsHitInDet1 = 0;
}

int BeamAnaBase::setDUTInputFile(const std::string& fname) {
  size_t found = fname.find("root:");
  if (found == string::npos && gSystem->AccessPathName(fname.c_str())) {
    cerr << ">>> Warning: File <<" << fname << ">> was not found!!" << endl;
    return static_cast<int>(dutchain_->GetEntries()); 
  }
  dutchain_->AddFile(fname.c_str(), -1);
  nDutchainentry = static_cast<long int>(dutchain_->GetEntries());
  return nDutchainentry; 
}

int BeamAnaBase::setTelescopeInputFile(const std::string& fname) {
  size_t found = fname.find("root:");
  if (found == string::npos && gSystem->AccessPathName(fname.c_str())) {
    cerr << ">>> Warning: File <<" << fname << ">> was not found!!" << endl;
    return static_cast<int>(telchain_->GetEntries()); 
  }
  telchain_->AddFile(fname.c_str(), -1);
  nTelchainentry = static_cast<long int>(telchain_->GetEntries());
  return nTelchainentry; 
}

bool BeamAnaBase::branchFound(TChain* chain,const string& b)
{
  TBranch* branch = chain->GetBranch(b.c_str());
  if (!branch) {
    cout << ">>> SetBranchAddress: <" << b << "> not found!" << endl;
    return false;
  }
  cout << ">>> SetBranchAddress: <" << b << "> found!" << endl;
  return true;
}

void BeamAnaBase::setAddresses() {
  //set the address of the DUT tree
  if(branchFound(dutchain_,"run") ) 
    dutchain_->SetBranchAddress("run", &dutEvent_->run);
  if(branchFound(dutchain_,"lumiSection") ) 
    dutchain_->SetBranchAddress("lumiSection", &dutEvent_->lumiSection);
  if(branchFound(dutchain_,"event") )
    dutchain_->SetBranchAddress("event", &dutEvent_->event);
  if(branchFound(dutchain_,"time") )
    dutchain_->SetBranchAddress("time", &dutEvent_->time);
  if(branchFound(dutchain_,"unixtime") )
    dutchain_->SetBranchAddress("unixtime", &dutEvent_->unixtime);
  if(branchFound(dutchain_,"tdcPhase") )
    dutchain_->SetBranchAddress("tdcPhase", &dutEvent_->tdcPhase);
  if(branchFound(dutchain_,"hvSettings") )
    dutchain_->SetBranchAddress("hvSettings", &dutEvent_->hvSettings);
  if(branchFound(dutchain_,"dutAngle") )
    dutchain_->SetBranchAddress("dutAngle", &dutEvent_->dutAngle);
  if(branchFound(dutchain_,"stubWord") )
    dutchain_->SetBranchAddress("stubWord", &dutEvent_->stubWord);
  if(branchFound(dutchain_,"vcth") )
    dutchain_->SetBranchAddress("vcth", &dutEvent_->vcth);
  if(branchFound(dutchain_,"offset") )
    dutchain_->SetBranchAddress("offset", &dutEvent_->offset);
  if(branchFound(dutchain_,"window") )
    dutchain_->SetBranchAddress("window", &dutEvent_->window);
  if(branchFound(dutchain_,"cwd") )
    dutchain_->SetBranchAddress("cwd", &dutEvent_->cwd);
  if(branchFound(dutchain_,"tilt") )
    dutchain_->SetBranchAddress("tilt", &dutEvent_->tilt);
  if(branchFound(dutchain_,"condData") )
    dutchain_->SetBranchAddress("condData", &dutEvent_->condData);
  if(branchFound(dutchain_,"glibStatus") )
    dutchain_->SetBranchAddress("glibStatus", &dutEvent_->glibStatus);
  if(branchFound(dutchain_,"cbc1Status") )
    dutchain_->SetBranchAddress("cbc1Status", &dutEvent_->cbc1Status);
  if(branchFound(dutchain_,"cbc2Status") )
    dutchain_->SetBranchAddress("cbc2Status", &dutEvent_->cbc2Status);
  if(branchFound(dutchain_,"dut_channel") )
    dutchain_->SetBranchAddress("dut_channel", &dutEvent_->dut_channel);
  if(branchFound(dutchain_,"dut_row") )
    dutchain_->SetBranchAddress("dut_row", &dutEvent_->dut_row);
  //set telescope branch address
  if(branchFound(telchain_,"nTrackParams") )  
  telchain_->SetBranchAddress("nTrackParams", &telEvent_->nTrackParams);
  if(branchFound(telchain_,"euEvt") )  
    telchain_->SetBranchAddress("euEvt", &telEvent_->euEvt);
  if(branchFound(telchain_,"xPos") )  
    telchain_->SetBranchAddress("xPos", &telEvent_->xPos);
  if(branchFound(telchain_,"yPos") )  
    telchain_->SetBranchAddress("yPos", &telEvent_->yPos);
  if(branchFound(telchain_,"dxdz") )  
    telchain_->SetBranchAddress("dxdz", &telEvent_->dxdz);
  if(branchFound(telchain_,"dydz") )  
    telchain_->SetBranchAddress("dydz", &telEvent_->dydz);
  if(branchFound(telchain_,"trackNum") )  
    telchain_->SetBranchAddress("trackNum", &telEvent_->trackNum);
  if(branchFound(telchain_,"iden") )  
    telchain_->SetBranchAddress("iden", &telEvent_->iden);
  if(branchFound(telchain_,"chi2") )  
    telchain_->SetBranchAddress("chi2", &telEvent_->chi2);
  if(branchFound(telchain_,"ndof") )  
    telchain_->SetBranchAddress("ndof", &telEvent_->ndof);
}

void BeamAnaBase::filltrigTrackmap() {
  
  for (Long64_t jentry=0; jentry<nTelchainentry;jentry++) {
    Long64_t ientry = telchain_->LoadTree(jentry);
    if (ientry < 0) break;
    trigTrackmap_[telEvent_->euEvt] = telEvent_;
  }
}

int BeamAnaBase::getNtrack (const long int dutEvt) const {
  int nt = -1;
  if( trigTrackmap_.find(dutEvt) != trigTrackmap_.end() )
    nt = trigTrackmap_.at(dutEvt)->nTrackParams;
  return nt;
}

void BeamAnaBase::setDetChannelVectors() {
  bool hExistD0 = false;
  bool hExistD1 = false;
  if( dutEvent_->dut_channel->find("det0") != dutEvent_->dut_channel->end() ) {
    if ((dut_channel->at("det0"))->size()) hExistD0 = true;
      for( unsigned int j = 0; j<(dutEvent_->dut_channel->at("det0")).size(); j++ ) {
        int ch = (dutEvent_->dut_channel->at("det0")).at(j);
	if( ch <= 1015 )  dut0_chtempC0->push_back(ch);
	else dut0_chtempC1->push_back(ch-1016);
      }
  }
  if( dutEvent_->dut_channel->find("det1") != dutEvent_->dut_channel->end() ) {
      if ((dutEvent_->dut_channel->at("det1")).size()) hExistD1 = true;
      for( unsigned int j = 0; j<(dutEvent_->dut_channel->at("det1")).size(); j++ ) {
        int ch = (dutEvent_->dut_channel->at("det1")).at(j);
        if( ch <= 1015 )  dut1_chtempC0->push_back(ch);
        else  dut1_chtempC1->push_back(ch-1016);
      }
  }
  if (!hExistD0 && !hExistD1) nEventsNoHits++;
  if ( hExistD0 && hExistD1)  nEventsHitInBoth++;
  if ( hExistD0 && !hExistD1) nEventsHitInDet0++;
  if (!hExistD0 && hExistD1)  nEventsHitInDet1++;
}

void BeamAnaBase::clearEvent() {
  dut0_chtempC0->clear();
  dut0_chtempC1->clear();
  dut1_chtempC0->clear();
  dut1_chtempC1->clear();
}

BeamAnaBase::~BeamAnaBase() {
  delete dutchain_;
  delete telchain_;
  delete dutEvent_;
  delete telEvent_;
}
