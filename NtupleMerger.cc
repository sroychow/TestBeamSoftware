#include "NtupleMerger.h"

NtupleMerger::NtupleMerger(const std::string dutTuple, const std::string telTuple, const std::string dqmTuple, const std::string outTuple) :
  cbcErrorVal_(new std::vector<int>()),
  cbcPLAddressVal_(new std::vector<int>()),
  telOutputEvent_(new tbeam::TelescopeEvent()),//for output
  telEvent_(new tbeam::TelescopeEvent()),//for input
  periodicityFlag_(true),
  pFlag_(true),
  goodEventFlag_(true),
  condEvent_(new tbeam::condEvent()),
  dutEvent_(new tbeam::dutEvent()),
  outdutEvent_(new tbeam::dutEvent()),
  trigTrackmap_(new std::map<Int_t,tbeam::TelescopeEvent>()) 
{
  dutF_ = TFile::Open(dutTuple.c_str());
  dutchain_ = dynamic_cast<TTree*>(dutF_->Get("treeMaker/tbeamTree"));
  nDutchainentry_ = static_cast<long int>(dutchain_->GetEntries());

  telF_ = TFile::Open(telTuple.c_str());
  telchain_ =dynamic_cast<TTree*>(telF_->Get("tracks"));
  nTelchainentry_ = static_cast<long int>(telchain_->GetEntries());

  dqmF_ = TFile::Open(dqmTuple.c_str());
  dqmchain_ = dynamic_cast<TTree*>(dqmF_->Get("sensorHitTree"));
  nDqmchainentry_ = static_cast<long int>(dqmchain_->GetEntries());

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
  //outTree_ = dutchain_->CloneTree(0);
  fout_ = TFile::Open(outTuple.c_str(),"recreate");
  outTree_ = new TTree("analysisTree","");
  outTree_->Branch("DUT", &outdutEvent_);
  outTree_->Branch("Condition",&condEvent_);
  outTree_->Branch("TelescopeEvent",&telOutputEvent_);
  outTree_->Branch("periodicityFlag",&pFlag_);
  outTree_->Branch("goodEventFlag",&goodEventFlag_);
}

void NtupleMerger::setInputBranchAddresses() {
  //set the address of the input DUT tree
  dutchain_->SetBranchStatus("*",1);
  dutchain_->SetBranchAddress("DUT", &dutEvent_);
  dutchain_->SetBranchAddress("Condition", &condEvent_);
  //set the address of the input Telescope tree
  telchain_->SetBranchAddress("nTrackParams", &telEvent_->nTrackParams);
  telchain_->SetBranchAddress("euEvt", &telEvent_->euEvt);
  telchain_->SetBranchAddress("xPos", &telEvent_->xPos);
  telchain_->SetBranchAddress("yPos", &telEvent_->yPos);
  telchain_->SetBranchAddress("dxdz", &telEvent_->dxdz);
  telchain_->SetBranchAddress("dydz", &telEvent_->dydz);
  telchain_->SetBranchAddress("trackNum", &telEvent_->trackNum);
  telchain_->SetBranchAddress("iden", &telEvent_->iden);
  telchain_->SetBranchAddress("chi2", &telEvent_->chi2);
  telchain_->SetBranchAddress("ndof", &telEvent_->ndof);
  //set branches of the input dqm tree
  dqmchain_->SetBranchAddress("cbcError", &cbcErrorVal_);
  dqmchain_->SetBranchAddress("cbcPLAddress", &cbcPLAddressVal_);
  dqmchain_->SetBranchAddress("eventFlag", &periodicityFlag_);   
}

void NtupleMerger::filltrigTrackmap() {
  std::cout << "Filling Trigger Track Map" << std::endl;
  for (Long64_t jentry=0; jentry<nTelchainentry_;jentry++) {
    Long64_t ientry = telchain_->GetEntry(jentry);
    if (jentry%1000 == 0)  
      cout << " Events processed. " << std::setw(8) << jentry 
           << "\t loadTree="<< ientry 
           << "\t ntracks=" << telEvent_->nTrackParams << endl;
    if (ientry < 0) break;
    tbeam::TelescopeEvent telTemp(*telEvent_);
    trigTrackmap_->insert({telEvent_->euEvt,telTemp});
  }
}

void NtupleMerger::eventLoop() {
  filltrigTrackmap();
  
  std::cout << "Trigtrackmap size="<< trigTrackmap_->size() << std::endl;
  for (Long64_t jentry=0; jentry<nEventstoLoop_;jentry++) {//nEventstoLoop_
     Long64_t dutentry = dutchain_->GetEntry(jentry);
     Long64_t dqmentry = dqmchain_->GetEntry(jentry);
     if (jentry%1000 == 0)  {
       cout << " Events processed. " << std::setw(8) << jentry 
            << "\t Load DUT=" << dutentry << "\t Load Dqm=" << dqmentry << endl;
       //std::cout << "Telescope Event=" << trigTrackmap_->at(condEvent_->event).euEvt 
       //          << "\tntracks=" << trigTrackmap_->at(condEvent_->event).nTrackParams << std::endl;
       //std::cout << "DQM Event::ErrorValSize=" << cbcErrorVal_->size() 
       //          << "\tPLAddressValSize" << cbcPLAddressVal_->size() << std::endl; 
       //std::cout << "condEvent pointer=" << condEvent_ 
       //          << "\teuEvt=" << condEvent_->event
       //          << std::endl;
     }
      goodEventFlag_ = true;
      for(unsigned int i = 0; i<condEvent_->cbcs.size(); i++) 
         std::cout << static_cast<unsigned int>(condEvent_->cbcs[i].error) << ",";  
       std::cout << std::endl;
     
     //if(!periodicityFlag_) std::cout << "Event no>>>" << jentry << std::endl;
     bool cbcErrflag = true;
     for(unsigned int i = 0; i<cbcErrorVal_->size(); i++) {
       condEvent_->cbcs[i].error = cbcErrorVal_->at(i);
       if(cbcErrflag && condEvent_->cbcs[i].error!=0)    cbcErrflag = false; 
     }
     for(unsigned int i = 0; i<cbcPLAddressVal_->size(); i++)
       condEvent_->cbcs[i].pipelineAdd = cbcPLAddressVal_->at(i);
      
    for(unsigned int i = 0; i<condEvent_->cbcs.size(); i++) 
         std::cout << static_cast<unsigned int>(condEvent_->cbcs[i].error) << ",";  
       std::cout << "\n End ev" << std::endl;

     outdutEvent_ = dutEvent_;
     telOutputEvent_ = &trigTrackmap_->at(condEvent_->event);
     pFlag_ = periodicityFlag_;
     if( !cbcErrorVal_ || !pFlag_)      goodEventFlag_ = false;
     outTree_->Fill();
  }
  endJob();
}


void NtupleMerger::endJob() {
   telF_->Close();
   dqmF_->Close();
   dutF_->Close();
   outTree_->AutoSave();
   fout_->Close();
   
}

NtupleMerger::~NtupleMerger() {
  //if(dutchain_) delete dutchain_;
  //if(telchain_) delete telchain_;
  //if(dqmchain_) delete dqmchain_;
  //if(telEvent_) delete telEvent_;
  //if(condEvent_) delete condEvent_;
}

int main(int argc, char** argv) {
  if(argc<5)   {
    std::cout << "Wrong Usage!!" << std::endl;
    std::cout << "Usage: ./ntuplemerger <EDMTupleName> <TelescopeTupleName> <DQMTupleName> <OutPutTupleName>" << std::endl;
    exit(1);
  }
  
  NtupleMerger m(argv[1],argv[2],argv[3],argv[4]);
  m.eventLoop(); 
  return 0;
}
