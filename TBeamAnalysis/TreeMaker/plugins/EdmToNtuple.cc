// -*- C++ -*-
//
// Package:    EdmToNtupleNoMask
// Class:      EdmToNtupleNoMask
// 
/**\class EdmToNtupleNoMask EdmToNtupleNoMask.cc TestBeamAnalysis/TreeMaker/plugins/EdmToNtupleNoMask.cc
 Description: [one line class summary]
 Implementation:
     [Notes on implementation]
*/
//
// Author:  Ali Harb , Suvankar Roy Chowdhury
// 
//

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigiCollection.h"
#include "Phase2TrackerDAQ/SiStripDigi/interface/SiStripCommissioningDigi.h"

#include "TBeamAnalysis/TreeMaker/plugins/EdmToNtuple.h"
#include<vector>
#include<algorithm>
#include<iomanip>

EdmToNtupleNoMask::EdmToNtupleNoMask(const edm::ParameterSet& iConfig) :
  verbosity_(iConfig.getUntrackedParameter<int>("verbosity", 0))
{
  std::vector<int> detId(iConfig.getParameter< std::vector<int> >("detIdVec"));
  std::vector<std::string> detNames(iConfig.getParameter< std::vector<std::string> >("detNamesVec"));
  if( detId.empty() || detId.size() != detNames.size() ) {
    std::cout << "detId information error!! Please check PSet" << std::endl;
    exit(EXIT_FAILURE);
  } 
  else {
    for( unsigned int i = 0; i<detId.size(); i++ ) {
      detIdNamemap_[detId[i]] = detNames[i];    
    }
  }
  if(verbosity_)
    for( auto& e: detIdNamemap_ )
      std::cout << e.first << "=" << e.second << std::endl;
  std::string stemp = iConfig.getParameter<std::string>("tdcAddress");
  tdcAdd_= std::stoul(stemp, nullptr, 16);
  stemp = iConfig.getParameter<std::string>("hvAddress");
  hvAdd_ = std::stoul(stemp, nullptr, 16);
  stemp = iConfig.getParameter<std::string>("dutAngAddress");
  DUTangAdd_ = std::stoul(stemp, nullptr, 16);
  
  if(verbosity_) {
    std::cout << "tdcPhase Address=" << tdcAdd_ << std::endl;
    std::cout << "HVsettings Address=" << hvAdd_ << std::endl;
    std::cout << "DUT angle Address=" << DUTangAdd_ << std::endl;
  }
}

void EdmToNtupleNoMask::beginJob()
{
  edm::Service<TFileService> fs;
  fs->file().cd("/");
  tree_ = fs->make<TTree>("tbeamTree", "AnalysisTree no mask");
  //ev is a dummy variable of type tbeam::Event
  tree_->Branch("run", &ev.run);
  tree_->Branch("lumiSection" , &ev.lumiSection );
  tree_->Branch("event" , &ev.event );
  tree_->Branch("time" , &ev.time, "time/L" );
  tree_->Branch("unixtime" , &ev.unixtime, "unixtime/L" );
  tree_->Branch("tdcPhase", &ev.tdcPhase);
  tree_->Branch("hvSettings", &ev.HVsettings);
  tree_->Branch("dutAngle", &ev.DUTangle);
  tree_->Branch("dut_channel", "std::map< std::string,std::vector<int> >", &ev.dut_channel);
  tree_->Branch("dut_row", "std::map< std::string,std::vector<int> >", &ev.dut_row);
  tree_->Branch("dut_adc", "std::map< std::string,std::vector<unsigned short> >", &ev.dut_adc);
}

void EdmToNtupleNoMask::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  tbeam::Event evtInfo;
  // event info
  evtInfo.run = iEvent.id().run();
  evtInfo.event = iEvent.id().event();  
  evtInfo.lumiSection = iEvent.luminosityBlock();
  edm::Timestamp itime = iEvent.time();
  evtInfo.time = itime.value();
  evtInfo.unixtime = itime.unixTime();
  
  edm::Handle < edm::DetSet<SiStripCommissioningDigi> > conditions;
  iEvent.getByLabel("SiStripDigiCondDataproducer", "ConditionData",  conditions);
  if( conditions.isValid() ) {
    edm::DetSet<SiStripCommissioningDigi>::const_iterator i_detSet;
    for( edm::DetSet<SiStripCommissioningDigi>::const_iterator i_detSet = conditions->begin(); 
	 i_detSet != conditions->end(); ++i_detSet){
      uint32_t key = i_detSet->getKey();
      int32_t value = i_detSet->getValue();
      if(key == tdcAdd_) evtInfo.tdcPhase = value;
      else if(key == hvAdd_) evtInfo.HVsettings = value;
      else if(key == DUTangAdd_) evtInfo.DUTangle = value;
    }
  }
  else 
    std::cerr << "SiStripCommissioningDigi not found!!" << std::endl;
  
  edm::Handle< edm::DetSetVector<PixelDigi> > input;
  iEvent.getByLabel( "SiStripDigitestproducer", "ProcessedRaw", input);
  if( input.isValid() ) {
    // loop over modules
    for(edm::DetSetVector<PixelDigi>::const_iterator it = input->begin() ; it != input->end(); ++it)
      {
	int detId = it->id;
	// loop over hits in the module
	for(edm::DetSet<PixelDigi>::const_iterator hit = it->begin(); hit!=it->end(); hit++ )
	  {
            evtInfo.dut_channel[detIdNamemap_[detId]].push_back(hit->channel());
            evtInfo.dut_row[detIdNamemap_[detId]].push_back(hit->row());
            evtInfo.dut_adc[detIdNamemap_[detId]].push_back(hit->adc());
	  }
      }
  }
  else 
    std::cerr << "PixelDigi not found!!" << std::endl;
  
  
  v_evtInfo_.push_back(evtInfo);
}

void EdmToNtupleNoMask::endJob()
{
  std::sort(v_evtInfo_.begin(),v_evtInfo_.end(),sortEvent);
  
  for( auto& e: v_evtInfo_ ) {
    ev = e;
    tree_->Fill();
  } 
}

bool EdmToNtupleNoMask::sortEvent( const tbeam::Event& a,  const tbeam::Event& b) {
  return a.time < b.time;
}
//define this as a plug-in
DEFINE_FWK_MODULE(EdmToNtupleNoMask);

