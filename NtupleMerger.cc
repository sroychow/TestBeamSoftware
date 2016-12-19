#include "NtupleMerger.h"
#include<iomanip>
using std::setw;
NtupleMerger::NtupleMerger(const std::string telTuple, const std::string runNumber) :
  telFound_(true),
  fei4Found_(true),
  dutF_(nullptr),
  dqmF_(nullptr),
  telOutputEvent_(new tbeam::TelescopeEvent()),//for output
  telEvent_(new tbeam::TelescopeEvent()),//for input
  fei4Event_(new tbeam::FeIFourEvent()),
  fei4OutputEvent_(new tbeam::FeIFourEvent()), 
  trigTrackmap_(new std::map<Int_t,tbeam::TelescopeEvent>()),
  trigFeI4map_(new std::map<Int_t,tbeam::FeIFourEvent>()),
  telOnly_(true)
{
  telF_ = TFile::Open(telTuple.c_str());
  if(!telF_) {
    std::cout << telTuple << " could not be onened!!" << std::endl;
    telFound_ = false;
    fei4Found_ = false;
    exit(1);
  } else {
    telchain_ = dynamic_cast<TTree*>(telF_->Get("tracks"));
    fei4chain_ = dynamic_cast<TTree*>(telF_->Get("rawdata"));
    if(telchain_)  {
      nTelchainentry_ = static_cast<long int>(telchain_->GetEntries());
      telFound_ = telFound_ && nTelchainentry_;
    } else telFound_ = false;
    if(fei4chain_) {
      fei4Found_ = fei4Found_ && static_cast<long int>(fei4chain_->GetEntries());
    } else fei4Found_ = false; 
  }  
  
  setInputBranchAddresses();
  std::string outTuple = "TelescopeAnalysisInputTree_" + runNumber + ".root";
  fout_ = TFile::Open(outTuple.c_str(),"recreate");
  outTree_ = new TTree("analysisTree","");
  if(telFound_)  outTree_->Branch("TelescopeEvent",&telOutputEvent_);
  if(fei4Found_) outTree_->Branch("Fei4Event",&fei4OutputEvent_);

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

    for (Long64_t jentry=0; jentry<fei4chain_->GetEntries();jentry++) {
      Long64_t ientry = fei4chain_->GetEntry(jentry);
      if (jentry%1000 == 0)  
        cout << " Events processed. " << std::setw(8) << jentry 
             << "\t loadTree="<< ientry 
             << "\t nPixHits=" << fei4Event_->nPixHits << endl;
      if (ientry < 0) break;
      tbeam::FeIFourEvent fei4Temp(*fei4Event_);
       telOutputEvent_ = &trigTrackmap_->at(fei4Event_->euEvt);
       fei4OutputEvent_ = &fei4Temp;  

      //Fill output tree
      outTree_->Fill();
    }
    endJob();
}
NtupleMerger::NtupleMerger(const std::string dutTuple, const std::string telTuple, const std::string dqmTuple, const std::string runNumber) :
  telFound_(true),
  fei4Found_(true),
  cbcErrorVal_(new std::vector<unsigned int>()),
  cbcPLAddressVal_(new std::vector<unsigned int>()),
  telOutputEvent_(new tbeam::TelescopeEvent()),//for output
  telEvent_(new tbeam::TelescopeEvent()),//for input
  fei4Event_(new tbeam::FeIFourEvent()),
  fei4OutputEvent_(new tbeam::FeIFourEvent()), 
  periodicityFlag_(true),
  pFlag_(true),
  goodEventFlag_(true),
  condEvent_(new tbeam::condEvent()),
  dutEvent_(new tbeam::dutEvent()),
  outdutEvent_(new tbeam::dutEvent()),
  outcondEvent_(new tbeam::condEvent()),
  trigTrackmap_(new std::map<Int_t,tbeam::TelescopeEvent>()),
  trigFeI4map_(new std::map<Int_t,tbeam::FeIFourEvent>()),
  telOnly_(false)
{
  tdcCounterFromdqm_ = 0;
  dut0C0chData_ = new std::vector<unsigned int>();
  dut0C1chData_ = new std::vector<unsigned int>();
  dut1C0chData_ = new std::vector<unsigned int>();
  dut1C1chData_ = new std::vector<unsigned int>();
  cbcCondmap_ = new std::map<long int,cbcCond>();
  debug_ = false;
  
  dutF_ = TFile::Open(dutTuple.c_str());
  if(!dutF_) {
    std::cout << dutTuple << " could not be onened!!" << std::endl;
    exit(1);
  }
  dutchain_ = dynamic_cast<TTree*>(dutF_->Get("treeMaker/tbeamTree"));
  nDutchainentry_ = static_cast<long int>(dutchain_->GetEntries());
  
  telF_ = TFile::Open(telTuple.c_str());
  if(!telF_) {
    std::cout << telTuple << " could not be onened!!" << std::endl;
    telFound_ = false;
    fei4Found_ = false;
    //exit(1);
  } else {
    telchain_ = dynamic_cast<TTree*>(telF_->Get("tracks"));
    fei4chain_ = dynamic_cast<TTree*>(telF_->Get("rawdata"));
    if(telchain_)  {
      nTelchainentry_ = static_cast<long int>(telchain_->GetEntries());
      telFound_ = telFound_ && nTelchainentry_;
    } else telFound_ = false;
    if(fei4chain_) {
      fei4Found_ = fei4Found_ && static_cast<long int>(fei4chain_->GetEntries());
    } else fei4Found_ = false; 
  }  
  
  dqmF_ = TFile::Open(dqmTuple.c_str());
  if(!dqmF_) {
    std::cout << dqmTuple << " could not be onened!!" << std::endl;
    //exit(1);
  }
  dqmchain_ = dynamic_cast<TTree*>(dqmF_->Get("sensorHitTree"));
  nDqmchainentry_ = static_cast<long int>(dqmchain_->GetEntries());
  
  //if(nDutchainentry_ == 0 || nDqmchainentry_ == 0) {
  //  std::cout << "One of the trees have zero entries!!!" << std::endl;
  //  exit(1);
  //}
  //if(nDutchainentry_ > nDqmchainentry_) {
  //  std::cout << "Warning::Total Entries in DUT > DQM Tree!!!Running only upto events processed by DQM" << std::endl;
  //  nEventstoLoop_ = nDqmchainentry_;
  //} else nEventstoLoop_ = nDutchainentry_;
  nEventstoLoop_ = nDutchainentry_;
  std::cout << "DUT Entries=" << nDutchainentry_
            << "\nTelescope Entries=" << nTelchainentry_
            << "\nDQM Entries=" << nDqmchainentry_ 
            << "\nLooping Over " << nEventstoLoop_ 
            << std::endl;

  setInputBranchAddresses();

  if(dutF_) bookValidationHistograms(runNumber); 

  std::string outTuple = "AnalysisTree_" + runNumber + ".root";
  fout_ = TFile::Open(outTuple.c_str(),"recreate");
  outTree_ = new TTree("analysisTree","");
  if(dutF_)  outTree_->Branch("DUT", &outdutEvent_);
  if(dqmF_)  outTree_->Branch("Condition",&outcondEvent_);
  if(telFound_)  outTree_->Branch("TelescopeEvent",&telOutputEvent_);
  if(fei4Found_) outTree_->Branch("Fei4Event",&fei4OutputEvent_);
  if(dutF_ && dqmF_) {
    outTree_->Branch("periodicityFlag",&pFlag_);
    outTree_->Branch("goodEventFlag",&goodEventFlag_);
  }
}

void NtupleMerger::setInputBranchAddresses() {
  //set the address of the input DUT tree
  if(dutF_) {
    dutchain_->SetBranchStatus("*",1);
    dutchain_->SetBranchAddress("DUT", &dutEvent_);
    dutchain_->SetBranchAddress("Condition", &condEvent_);
  }
  if(telFound_) {
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
  }
  if(fei4Found_) {
    fei4chain_->SetBranchAddress("nPixHits", &fei4Event_->nPixHits);
    fei4chain_->SetBranchAddress("euEvt", &fei4Event_->euEvt);
    fei4chain_->SetBranchAddress("col", &fei4Event_->col);
    fei4chain_->SetBranchAddress("row", &fei4Event_->row);
    fei4chain_->SetBranchAddress("tot", &fei4Event_->tot);
    fei4chain_->SetBranchAddress("lv1", &fei4Event_->lv1);
    fei4chain_->SetBranchAddress("iden", &fei4Event_->iden);
    fei4chain_->SetBranchAddress("hitTime", &fei4Event_->hitTime);
    fei4chain_->SetBranchAddress("frameTime", &fei4Event_->frameTime);
  }
  //set branches of the input dqm tree
  if(dqmF_)  {
    dqmchain_->SetBranchStatus("*",1);
    dqmchain_->SetBranchAddress("cbcError", &cbcErrorVal_);
    dqmchain_->SetBranchAddress("cbcPLAddress", &cbcPLAddressVal_);
    dqmchain_->SetBranchAddress("eventFlag", &periodicityFlag_); 
    dqmchain_->SetBranchAddress("tdcCounter",&tdcCounterFromdqm_);  
    dqmchain_->SetBranchAddress("l1Accept", &l1adqm_);
    dqmchain_->SetBranchAddress("eventCbc", &evCountcbc_);
    dqmchain_->SetBranchAddress("totalHits", &totalHitsdqm_);
    dqmchain_->SetBranchAddress("totalStubs", &totalStubsdqm_);
    dqmchain_->SetBranchAddress("dut0Ch0data",  &dut0C0chData_);
    dqmchain_->SetBranchAddress("dut0Ch1data", &dut0C1chData_);
    dqmchain_->SetBranchAddress("dut1Ch0data", &dut1C0chData_);
    dqmchain_->SetBranchAddress("dut1Ch1data", &dut1C1chData_);
  }
}

void NtupleMerger::bookValidationHistograms(const std::string run) {
  std::string fname = "validation_" + run +".root";
  //book validation histograms
  fval = TFile::Open(fname.c_str(),"recreate");
  tdc1 = new TH1D("tdc1","TDC phase before filter(edm)",16,-0.5,15.5);
  tdc2 = new TH1D("tdc2","TDC phase after periodicity filter(edm)",16,-0.5,15.5);
  tdc3 = new TH1D("tdc3","TDC phase after good event filter(edm)",16,-0.5,15.5);
  
  tdc4 = new TH1D("tdc4","TDC phase before filter(dqm)",16,-0.5,15.5);
  tdc5 = new TH1D("tdc5","TDC phase after periodicity filter(dqm)",16,-0.5,15.5);
  tdc6 = new TH1D("tdc6","TDC phase after good event filter(dqm)",16,-0.5,15.5);
  
  eventFilter = new TH1I("goodEventCounter","Good flag",5,-0.5,4.5);
  hitmatch = new TH1I("hitmatch","#Hits match raw vs edm",2,-0.5,1.5);
  hitdqm = new TH1D("hitdqm","Total Hits dqm",50,-0.5,49.5);
  hitedm = new TH1D("hitedm","Total Hits edm",50,-0.5,49.5);
}

void NtupleMerger::filltrigTrackmap() {
  if(telFound_) {
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
  if(fei4Found_) {
    std::cout << "Filling Trigger Track Map" << std::endl;
    for (Long64_t jentry=0; jentry<fei4chain_->GetEntries();jentry++) {
      Long64_t ientry = fei4chain_->GetEntry(jentry);
      if (jentry%1000 == 0)  
        cout << " Events processed. " << std::setw(8) << jentry 
             << "\t loadTree="<< ientry 
             << "\t nPixHits=" << fei4Event_->nPixHits << endl;
      if (ientry < 0) break;
      tbeam::FeIFourEvent fei4Temp(*fei4Event_);
      trigFeI4map_->insert({fei4Event_->euEvt,fei4Temp});
    }
  }

  if(dqmF_) {
  for (Long64_t jentry=0; jentry<nDqmchainentry_;jentry++) {
    Long64_t dqmentry = dqmchain_->GetEntry(jentry);
    if (jentry%1000 == 0)  
      cout << " Events processed. " << std::setw(8) << jentry  
           << "\t loadDQMTree="<< dqmentry 
           << "\t L1A=" << l1adqm_
           << std::endl;
    cbcCond ctemp;
    ctemp.l1Accept_ = l1adqm_;
    ctemp.tdcCounter_ = tdcCounterFromdqm_;
    ctemp.totalHits_ = totalHitsdqm_;  
    ctemp.totalStubs_ = totalStubsdqm_;  
    ctemp.eventFlag_ = periodicityFlag_;
    ctemp.eventCountCBC_ = evCountcbc_;
    ctemp.cbcErrorVal_ = *cbcErrorVal_;
    ctemp.cbcPLAddressVal_ = *cbcPLAddressVal_;
    ctemp.dut0C0chData_ = *dut0C0chData_;
    ctemp.dut0C1chData_ = *dut0C1chData_;
    ctemp.dut1C0chData_ = *dut1C0chData_;
    ctemp.dut1C1chData_ = *dut1C1chData_;
    cbcCondmap_->insert({l1adqm_,ctemp});
  }
  }
}

void NtupleMerger::eventLoop() {
  //manual calculation ov evflag
  
  filltrigTrackmap();
  std::cout << "Trigtrackmap size="<< trigTrackmap_->size() << std::endl;
  std::cout << "CBCCondmap size=" << cbcCondmap_->size() << std::endl;
  int counter[5] = {0,0,0,0,0};  
  for (Long64_t jentry=0; jentry<nDutchainentry_;jentry++) {//nEventstoLoop_
    Long64_t dutentry = dutchain_->GetEntry(jentry);
    //Long64_t dqmentry = dqmchain_->GetEntry(jentry);
    if (jentry%1000 == 0)  {
      cout << " Events processed. " << std::setw(8) << jentry 
	   << "\t Load DUT=" << dutentry 
           << "\t EDM Event = " << condEvent_->event 
           << "\t TelescopeFound=" << telFound_ 
           << endl;
    }
    counter[0]++;
    eventFilter->Fill(0);//all
    //std::cout << "EDM Event = " << condEvent_->event << std::endl; 
    //l1A match
    if(cbcCondmap_->find(condEvent_->event) == cbcCondmap_->end())                       continue;
    if(telFound_ && trigTrackmap_->find(condEvent_->event) == trigTrackmap_->end())      continue;
    //if(fei4Found_ && trigFeI4map_->find(condEvent_->event) == trigFeI4map_->end())     continue;
    eventFilter->Fill(1);//l1A matched
    counter[1]++;

    auto& cbctemp = cbcCondmap_->at(condEvent_->event);
    auto& dtemp = *dutEvent_;
    auto& ctemp = *condEvent_;
    
    bool cbcErrflag = true;
    for(unsigned int i = 0; i<cbctemp.cbcErrorVal_.size(); i++) {
      ctemp.cbcs[i].error = cbctemp.cbcErrorVal_.at(i);
      if(cbcErrflag && ctemp.cbcs[i].error!=0)    cbcErrflag = false; 
    }
    for(unsigned int i = 0; i<cbctemp.cbcPLAddressVal_.size(); i++)
      ctemp.cbcs[i].pipelineAdd = cbctemp.cbcPLAddressVal_.at(i);
    
    outdutEvent_ = &dtemp;
    outcondEvent_ = &ctemp;
    if(telFound_) telOutputEvent_ = &trigTrackmap_->at(ctemp.event);
    if(fei4Found_) fei4OutputEvent_ = &trigFeI4map_->at(ctemp.event);  
    pFlag_ = cbctemp.eventFlag_;
    bool tdcMatch = (cbctemp.tdcCounter_ == ctemp.tdcPhase);
    goodEventFlag_ = cbcErrflag && pFlag_ && tdcMatch;

    //Fill output tree
    outTree_->Fill();

    //Fill validation histograms
    tdc1->Fill(ctemp.tdcPhase);
    tdc4->Fill(cbctemp.tdcCounter_);
    if(pFlag_)   {
      tdc2->Fill(ctemp.tdcPhase);
      tdc5->Fill(cbctemp.tdcCounter_);
    }
    if(goodEventFlag_) {
      tdc3->Fill(ctemp.tdcPhase);
      tdc6->Fill(cbctemp.tdcCounter_);
    }
    if(pFlag_)   {
      eventFilter->Fill(2);
      counter[2]++;  
    }
    if(cbcErrflag && pFlag_)  {
      eventFilter->Fill(3);
      counter[3]++;
    }
    if(goodEventFlag_)  {
      eventFilter->Fill(4);
      counter[4]++;
    }

    int totalHitsDQMch = cbctemp.dut0C0chData_.size() + cbctemp.dut0C1chData_.size() + 
                         cbctemp.dut1C0chData_.size() + cbctemp.dut1C1chData_.size();
    int totalHitsEDM = 0;
    for(auto& dc : dtemp.dut_channel) 
      totalHitsEDM += dc.second.size();
    if(totalHitsEDM == totalHitsDQMch)  hitmatch->Fill(1);
    else hitmatch->Fill(0);
    hitdqm->Fill(totalHitsDQMch);
    hitedm->Fill(totalHitsEDM);
    if(debug_) {
      std::cout << "DQM Hits>>>det0C0>>("; 
      for(auto& h : cbctemp.dut0C0chData_) std::cout << h << ",";
      std::cout << ")\t>>>det1C0>>("; 
      for(auto& h : cbctemp.dut1C0chData_) std::cout << h << ",";
      std::cout << endl;
      std::cout << "EDM Hits>>>";
      for(auto& dc : dtemp.dut_channel) {
        std::cout << dc.first << ">>(";
        for(auto& h : dc.second) std::cout << h << ",";
        std::cout << ")\t>>";
      }
      std::cout << endl;
      std::cout << "L1A=" << condEvent_->event << "\tHitsEDM=" << totalHitsEDM 
              << "\tHitsDQM=" <<  int(cbctemp.totalHits_) 
              << "\tHHdqm=" <<  totalHitsDQMch << std::endl;
    }
  }
  
  TString  binS[] = {"All DUT", "L1Match", "Periodicity", "CBCError", "TDC Match"};
  for(unsigned int i = 0; i<5; i++) {
    eventFilter->GetXaxis()->SetBinLabel(i+1, binS[i]);
    eventFilter->SetBinContent(i+1,eventFilter->GetBinContent(i+1)/eventFilter->GetBinContent(1));
    std::cout << binS[i] << "\t" << counter[i] << "\t" << float(counter[i])/float(counter[0]) << std::endl;
  }
  endJob();
}


void NtupleMerger::endJob() {
  outTree_->AutoSave();
  if(fout_) fout_->Close();
  if(telFound_) telF_->Close();
  if(telOnly_)  return;
  if(dqmF_) dqmF_->Close();
  if(dutF_) dutF_->Close();
  if(fval) {
    fval->Write();
    fval->Close();
  }
  
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
    std::cout << "Wrong Usage!!Use one of the following" << std::endl;
    std::cout << "Usage: ./ntuplemerger <EDMTupleName> <TelescopeTupleName> <DQMTupleName> <RunNumber>" << std::endl;
    std::cout << "Usage: ./ntuplemerger <TelescopeTupleName> <RunNumber>" << std::endl;
    exit(1);
  }
  if(argc == 3) NtupleMerger m(argv[1],argv[2]);
  else {
    NtupleMerger m(argv[1],argv[2],argv[3],argv[4]);
    m.eventLoop();
  } 
  return 0;
}
