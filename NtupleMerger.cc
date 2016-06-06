#include "NtupleMerger.h"

NtupleMerger::NtupleMerger(const std::string dutTuple, const std::string telTuple, const std::string dqmTuple) :
  cbcErrorVal_(new std::vector<int>()),
  cbcPLAddressVal_(new std::vector<int>()),
  telOutputEvent_(new tbeam::TelescopeEvent()),//for output
  telEvent_(new tbeam::TelescopeEvent()),//for input
  periodicityFlag_(true),
  pFlag_(true),
  goodEventFlag_(true),
  condEvent_(new tbeam::condEvent()),
  trigTrackmap_(new std::map<Int_t,tbeam::TelescopeEvent>()) 
{
  setDUTInputFile(dutTuple);
  setTelescopeInputFile(telTuple);
  setDQMInputFile(dqmTuple);
  if(nTelchainentry_ == 0 || nDutchainentry_ == 0 || nDqmchainentry_ == 0) {
     std::cout << "One of the trees have zero entries!!!" << std::endl;
     exit(1);
  }
  if(nDutchainentry_ > nDqmchainentry_) {
     std::cout << "Warning::Total Entries in DUT > DQM Tree!!!Running only upto events processed by DQM" << std::endl;
     nEventstoLoop_ = nDqmchainentry_;
  } else nEventstoLoop_ = nDutchainentry_;
  std::cout << "DUT Entries=" << nDutchainentry_
            << "\nTelescope Entries=" << nTelchainentry_
            << "\nDQM Entries=" << nDqmchainentry_ 
            << "\nLooping Over " << nEventstoLoop_ 
            << std::endl;
  setInputBranchAddresses();
}

int NtupleMerger::setDUTInputFile(const std::string& fname) {
  size_t found = fname.find("root:");
  if (found == string::npos && gSystem->AccessPathName(fname.c_str())) {
    std::cerr << ">>> Warning: File <<" << fname << ">> was not found!!" << endl;
    return static_cast<int>(dutchain_->GetEntries()); 
  }
  dutF_ = TFile::Open(fname.c_str(),"update");
  //dutchain_->AddFile(fname.c_str(), -1);
  dutchain_ = dynamic_cast<TTree*>(dutF_->Get("treeMaker/tbeamTree"));
  nDutchainentry_ = static_cast<long int>(dutchain_->GetEntries());
  return nDutchainentry_; 
}

int NtupleMerger::setTelescopeInputFile(const std::string& fname) {
  size_t found = fname.find("root:");
  if (found == string::npos && gSystem->AccessPathName(fname.c_str())) {
    std::cerr << ">>> Warning: File <<" << fname << ">> was not found!!" << endl;
    return static_cast<int>(telchain_->GetEntries()); 
  }
  telF_ = TFile::Open(fname.c_str());
  telchain_ =dynamic_cast<TTree*>(telF_->Get("tracks"));
  nTelchainentry_ = static_cast<long int>(telchain_->GetEntries());
  return nTelchainentry_; 
}

int NtupleMerger::setDQMInputFile(const std::string& fname) {
  size_t found = fname.find("root:");
  if (found == string::npos && gSystem->AccessPathName(fname.c_str())) {
    std::cerr << ">>> Warning: File <<" << fname << ">> was not found!!" << endl;
    return static_cast<int>(dqmchain_->GetEntries()); 
  }
  dqmF_ = TFile::Open(fname.c_str());
  dqmchain_ = dynamic_cast<TTree*>(dqmF_->Get("sensorHitTree"));
  nDqmchainentry_ = static_cast<long int>(dqmchain_->GetEntries());
  return nDqmchainentry_; 
}
bool NtupleMerger::branchFound(TTree* chain,const string& b,std::vector<std::string>& brList_)
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

void NtupleMerger::setInputBranchAddresses() {
  //set the address of the input DUT tree
  if( branchFound(dutchain_,"Condition",dutbrList_) )    dutchain_->SetBranchAddress("Condition", &condEvent_);
  //dutchain_->SetBranchAddress("Condition", &condEvent_);
  //dutbrList_.push_back("Condition");
  condBranch_ = dutchain_->GetBranch("Condition");
  telBranch_ = dutchain_->Branch("TelescopeEvent",&telOutputEvent_);
  periodicityBranch_ = dutchain_->Branch("periodicityFlag",&pFlag_);
  goodEvBranch_ = dutchain_->Branch("goodEventFlag",&goodEventFlag_);
  
  //set the address of the input Telescope tree
  if(branchFound(telchain_,"nTrackParams",telbrList_) )  telchain_->SetBranchAddress("nTrackParams", &telEvent_->nTrackParams);
  if(branchFound(telchain_,"euEvt",telbrList_) )  telchain_->SetBranchAddress("euEvt", &telEvent_->euEvt);
  if(branchFound(telchain_,"xPos",telbrList_) )   telchain_->SetBranchAddress("xPos", &telEvent_->xPos);
  if(branchFound(telchain_,"yPos",telbrList_) )   telchain_->SetBranchAddress("yPos", &telEvent_->yPos);
  if(branchFound(telchain_,"dxdz",telbrList_) )   telchain_->SetBranchAddress("dxdz", &telEvent_->dxdz);
  if(branchFound(telchain_,"dydz",telbrList_) )   telchain_->SetBranchAddress("dydz", &telEvent_->dydz);
  if(branchFound(telchain_,"trackNum",telbrList_) ) telchain_->SetBranchAddress("trackNum", &telEvent_->trackNum);
  if(branchFound(telchain_,"iden",telbrList_) )  telchain_->SetBranchAddress("iden", &telEvent_->iden);
  if(branchFound(telchain_,"chi2",telbrList_) )  telchain_->SetBranchAddress("chi2", &telEvent_->chi2);
  if(branchFound(telchain_,"ndof",telbrList_) )  telchain_->SetBranchAddress("ndof", &telEvent_->ndof);
  //set branches of the input dqm tree
  if(branchFound(dqmchain_,"cbcError",dqmbrList_) )  dqmchain_->SetBranchAddress("cbcError", &cbcErrorVal_);
  if(branchFound(dqmchain_,"cbcPLAddress",dqmbrList_) )  dqmchain_->SetBranchAddress("cbcPLAddress", &cbcPLAddressVal_);
  if(branchFound(dqmchain_,"eventFlag",dqmbrList_) )  dqmchain_->SetBranchAddress("eventFlag", &periodicityFlag_);   
}

int NtupleMerger::getDUTEntry(int lflag) const
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

int NtupleMerger::getTelEntry(int lflag) const
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

int NtupleMerger::getDQMEntry(int lflag) const
{
  int nbytes = 0;
  for (auto it  = dqmbrList_.begin(); it != dqmbrList_.end(); ++it) {
    TBranch* branch = dqmchain_->GetBranch((*it).c_str());
    if (!branch) {
      cout << ">>> Branch: " << (*it) << " not found!" << endl;
      continue;
    }
    nbytes += branch->GetEntry(lflag);
  }
  return nbytes;
}


void NtupleMerger::filltrigTrackmap() {
  std::cout << "Filling Trigger Track Map" << std::endl;
  for (Long64_t jentry=0; jentry<nTelchainentry_;jentry++) {
    Long64_t ientry = telchain_->LoadTree(jentry);
    int nb = getTelEntry(ientry);
    if (jentry%1000 == 0)  cout << " Events processed. " << std::setw(8) << jentry << "\t loadTree="<< ientry << endl;
    if (ientry < 0) break;
    tbeam::TelescopeEvent telTemp(*telEvent_);
    trigTrackmap_->insert({telEvent_->euEvt,telTemp});
  }
}

void NtupleMerger::eventLoop() {
  filltrigTrackmap();
  std::cout << "Trigtrackmap size="<< trigTrackmap_->size() << std::endl;
  for (Long64_t jentry=0; jentry<nEventstoLoop_;jentry++) {//nEventstoLoop_
     Long64_t dutentry = dutchain_->LoadTree(jentry);
     Long64_t dqmentry = dqmchain_->LoadTree(jentry);
     int nbytes = getDUTEntry(dutentry);
     int mbytes = getDQMEntry(dqmentry);
     if (jentry%1000 == 0)  {
       cout << " Events processed. " << std::setw(8) << jentry << endl;
       std::cout << "Telescope Event=" << trigTrackmap_->at(condEvent_->event).euEvt << "\tntracks=" 
                 << trigTrackmap_->at(condEvent_->event).nTrackParams << std::endl;
       std::cout << "DQM Event::ErrorValSize=" << cbcErrorVal_->size() << "\tPLAddressValSize" << cbcPLAddressVal_->size() << std::endl; 
     }
     
     //if(!periodicityFlag_) std::cout << "Event no>>>" << jentry << std::endl;
     for(unsigned int i = 0; i<cbcErrorVal_->size(); i++)
       condEvent_->cbcs[i].error = cbcErrorVal_->at(i);
     for(unsigned int i = 0; i<cbcPLAddressVal_->size(); i++)
       condEvent_->cbcs[i].pipelineAdd = cbcPLAddressVal_->at(i);
     telOutputEvent_ = &trigTrackmap_->at(condEvent_->event);
     pFlag_ = periodicityFlag_;
     periodicityBranch_->Fill();
     goodEvBranch_->Fill();
     condBranch_->Fill();
     telBranch_->Fill();
  }
  endJob();
}


void NtupleMerger::endJob() {
   telF_->Close();
   dqmF_->Close();
   dutF_->cd("treeMaker");
   dutchain_->Write("", TObject::kOverwrite);
   //"", TObject::kOverwrite);
   dutF_->Close();
}

NtupleMerger::~NtupleMerger() {
  //if(dutchain_) delete dutchain_;
  //if(telchain_) delete telchain_;
  //if(dqmchain_) delete dqmchain_;
  //if(telEvent_) delete telEvent_;
  //if(condEvent_) delete condEvent_;
}

int main(int argc, char** argv) {
  if(argc<3)   {
    std::cout << "Wrong Usage!!" << std::endl;
    std::cout << "Usage: ./ntuplemerger <EDMTupleName> <TelescopeTupleName> <DQMTupleName>" << std::endl;
    exit(1);
  }
  
  NtupleMerger m(argv[1],argv[2],argv[3]);
  m.eventLoop(); 
  //m.endJob();
  return 0;
}
