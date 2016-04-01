#include "BeamAnaBase.h"
#include "TSystem.h"
#include "TChain.h"
#include "Reco.h"

BeamAnaBase::BeamAnaBase(const bool doTelescopeAnalysis) :
  dutchain_(new TChain("treeMaker/tbeamTree")),
  telchain_(new TChain("tracks")),
  dutEvent_(new skbeam::tBeamBase()),
  telEvent_(new skbeam::telescopeBase()),
  dut0_chtempC0_(new std::vector<int>()),
  dut0_chtempC1_(new std::vector<int>()),
  dut1_chtempC0_(new std::vector<int>()),
  dut1_chtempC1_(new std::vector<int>()),
  dutClustermap_(new std::map<std::string,std::vector<skbeam::Cluster>>()),
  dutRecoStubmap_(new std::map<std::string,std::vector<skbeam::Stub> >()),
  dutCbcStubmap_(new std::map<std::string,std::vector<unsigned int>>()),
  requireTelescope_(doTelescopeAnalysis) 
{
  nTelchainentry = -1;
  nDutchainentry = -1;
  nEventsNoHits  = 0;
  nEventsHitInBoth = 0;
  nEventsHitInDet0 = 0;
  nEventsHitInDet1 = 0;
  dutbrList_.clear();
  telbrList_.clear();
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

bool BeamAnaBase::branchFound(TChain* chain,const string& b,std::vector<std::string>& brList_)
{
  TBranch* branch = chain->GetBranch(b.c_str());
  if (!branch) {
    cout << ">>> SetBranchAddress: <" << b << "> not found!" << endl;
    return false;
  }
  cout << ">>> SetBranchAddress: <" << b << "> found!" << endl;
  brList_.push_back(b);
  return true;
}

void BeamAnaBase::setAddresses() {
  //set the address of the DUT tree
  if(branchFound(dutchain_,"run",dutbrList_) ) 
    dutchain_->SetBranchAddress("run", &dutEvent_->run);
  if(branchFound(dutchain_,"lumiSection",dutbrList_) ) 
    dutchain_->SetBranchAddress("lumiSection", &dutEvent_->lumiSection);
  if(branchFound(dutchain_,"event",dutbrList_) )
    dutchain_->SetBranchAddress("event", &dutEvent_->event);
  if(branchFound(dutchain_,"time",dutbrList_) )
    dutchain_->SetBranchAddress("time", &dutEvent_->time);
  if(branchFound(dutchain_,"unixtime",dutbrList_) )
    dutchain_->SetBranchAddress("unixtime", &dutEvent_->unixtime);
  if(branchFound(dutchain_,"tdcPhase",dutbrList_) )
    dutchain_->SetBranchAddress("tdcPhase", &dutEvent_->tdcPhase);
  if(branchFound(dutchain_,"hvSettings",dutbrList_) )
    dutchain_->SetBranchAddress("hvSettings", &dutEvent_->hvSettings);
  if(branchFound(dutchain_,"dutAngle",dutbrList_) )
    dutchain_->SetBranchAddress("dutAngle", &dutEvent_->dutAngle);
  if(branchFound(dutchain_,"stubWord",dutbrList_) )
    dutchain_->SetBranchAddress("stubWord", &dutEvent_->stubWord);
  if(branchFound(dutchain_,"vcth",dutbrList_) )
    dutchain_->SetBranchAddress("vcth", &dutEvent_->vcth);
  if(branchFound(dutchain_,"offset",dutbrList_) )
    dutchain_->SetBranchAddress("offset", &dutEvent_->offset);
  if(branchFound(dutchain_,"window",dutbrList_) )
    dutchain_->SetBranchAddress("window", &dutEvent_->window);
  if(branchFound(dutchain_,"cwd",dutbrList_) )
    dutchain_->SetBranchAddress("cwd", &dutEvent_->cwd);
  if(branchFound(dutchain_,"tilt",dutbrList_) )
    dutchain_->SetBranchAddress("tilt", &dutEvent_->tilt);
  if(branchFound(dutchain_,"condData",dutbrList_) )
    dutchain_->SetBranchAddress("condData", &dutEvent_->condData);
  if(branchFound(dutchain_,"glibStatus",dutbrList_) )
    dutchain_->SetBranchAddress("glibStatus", &dutEvent_->glibStatus);
  if(branchFound(dutchain_,"cbc1Status",dutbrList_) )
    dutchain_->SetBranchAddress("cbc1Status", &dutEvent_->cbc1Status);
  if(branchFound(dutchain_,"cbc2Status",dutbrList_) )
    dutchain_->SetBranchAddress("cbc2Status", &dutEvent_->cbc2Status);
  if(branchFound(dutchain_,"dut_channel",dutbrList_) )
    dutchain_->SetBranchAddress("dut_channel", &dutEvent_->dut_channel);
  if(branchFound(dutchain_,"dut_row",dutbrList_) )
    dutchain_->SetBranchAddress("dut_row", &dutEvent_->dut_row);
  //set telescope branch address
  if(requireTelescope_) {
    if(branchFound(telchain_,"nTrackParams",dutbrList_) )  
      telchain_->SetBranchAddress("nTrackParams", &telEvent_->nTrackParams);
    if(branchFound(telchain_,"euEvt",dutbrList_) )  
      telchain_->SetBranchAddress("euEvt", &telEvent_->euEvt);
    if(branchFound(telchain_,"xPos",dutbrList_) )  
      telchain_->SetBranchAddress("xPos", &telEvent_->xPos);
    if(branchFound(telchain_,"yPos",dutbrList_) )  
      telchain_->SetBranchAddress("yPos", &telEvent_->yPos);
    if(branchFound(telchain_,"dxdz",dutbrList_) )  
      telchain_->SetBranchAddress("dxdz", &telEvent_->dxdz);
    if(branchFound(telchain_,"dydz",dutbrList_) )  
      telchain_->SetBranchAddress("dydz", &telEvent_->dydz);
    if(branchFound(telchain_,"trackNum",dutbrList_) )  
      telchain_->SetBranchAddress("trackNum", &telEvent_->trackNum);
    if(branchFound(telchain_,"iden",dutbrList_) )  
      telchain_->SetBranchAddress("iden", &telEvent_->iden);
    if(branchFound(telchain_,"chi2",dutbrList_) )  
      telchain_->SetBranchAddress("chi2", &telEvent_->chi2);
    if(branchFound(telchain_,"ndof",dutbrList_) )  
      telchain_->SetBranchAddress("ndof", &telEvent_->ndof);
  }
}

int BeamAnaBase::getDUTEntry(int lflag) const
{
  int nbytes = 0;
  for (auto it  = dutbrList_.begin(); it != dutbrList_.end(); ++it) {
    TBranch* branch = dutchain_->GetBranch((*it).c_str());
    if (!branch) {
      cout << ">>> Branch: " << (*it) << " not found!" << endl;
      continue;
    }
    nbytes += branch->GetEntry(lflag);
  }
  return nbytes;
}

int BeamAnaBase::getTelEntry(int lflag) const
{
  int nbytes = 0;
  for (auto it  = telbrList_.begin(); it != telbrList_.end(); ++it) {
    TBranch* branch = telchain_->GetBranch((*it).c_str());
    if (!branch) {
      cout << ">>> Branch: " << (*it) << " not found!" << endl;
      continue;
    }
    nbytes += branch->GetEntry(lflag);
  }
  return nbytes;
}

void BeamAnaBase::filltrigTrackmap() {
  for (Long64_t jentry=0; jentry<nTelchainentry;jentry++) {
    Long64_t ientry = telchain_->LoadTree(jentry);
    int nb = getTelEntry(ientry);
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
    if ((dutEvent_->dut_channel->at("det0")).size()) hExistD0 = true;
      for( unsigned int j = 0; j<(dutEvent_->dut_channel->at("det0")).size(); j++ ) {
        int ch = (dutEvent_->dut_channel->at("det0")).at(j);
	if( ch <= 1015 )  dut0_chtempC0_->push_back(ch);
	else dut0_chtempC1_->push_back(ch-1016);
      }
  }
  if( dutEvent_->dut_channel->find("det1") != dutEvent_->dut_channel->end() ) {
      if ((dutEvent_->dut_channel->at("det1")).size()) hExistD1 = true;
      for( unsigned int j = 0; j<(dutEvent_->dut_channel->at("det1")).size(); j++ ) {
        int ch = (dutEvent_->dut_channel->at("det1")).at(j);
        if( ch <= 1015 )  dut1_chtempC0_->push_back(ch);
        else  dut1_chtempC1_->push_back(ch-1016);
      }
  }
  if (!hExistD0 && !hExistD1) nEventsNoHits++;
  if ( hExistD0 && hExistD1)  nEventsHitInBoth++;
  if ( hExistD0 && !hExistD1) nEventsHitInDet0++;
  if (!hExistD0 && hExistD1)  nEventsHitInDet1++;
}

void BeamAnaBase::doClustering() {
  Reco::getCBCclsuterInfo("det0C0", *dut0_chtempC0_, *dutClustermap_);
  Reco::getCBCclsuterInfo("det0C1", *dut0_chtempC1_, *dutClustermap_);
  Reco::getCBCclsuterInfo("det1C0", *dut1_chtempC0_, *dutClustermap_);
  Reco::getCBCclsuterInfo("det1C1", *dut1_chtempC1_, *dutClustermap_);
}

void BeamAnaBase::findStub(const int stubWindow) { 
 Reco::getRecoStubInfo(dutClustermap_, stubWindow,*dutRecoStubmap_,"C0");
 Reco::getRecoStubInfo(dutClustermap_, stubWindow,*dutRecoStubmap_,"C1");
 Reco::getCBCStubInfo(*dutCbcStubmap_,dutEvent_->stubWord);
}
void BeamAnaBase::clearEvent() {
  dut0_chtempC0_->clear();
  dut0_chtempC1_->clear();
  dut1_chtempC0_->clear();
  dut1_chtempC1_->clear();
  dutClustermap_->clear();
  dutRecoStubmap_->clear();
  dutCbcStubmap_->clear();
}

BeamAnaBase::~BeamAnaBase() {
  delete dutchain_;
  delete telchain_;
  delete dutEvent_;
  delete telEvent_;
}
