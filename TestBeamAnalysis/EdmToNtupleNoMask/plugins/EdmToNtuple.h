#ifndef __TBeamAnalysis_TreeMaker_EdmToNtupleNoMask_h
#define __TBeamAnalysis_TreeMaker_EdmToNtupleNoMask_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TBeamAnalysis/TreeMaker/interface/AnalysisObjects.h"

#include "TFile.h"
#include "TTree.h"
#include<stdint.h>

class EdmToNtupleNoMask : public edm::EDAnalyzer
{
 public:
  explicit EdmToNtupleNoMask(const edm::ParameterSet& iConfig);
  ~EdmToNtupleNoMask(){}
 private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  static bool sortEvent( const tbeam::Event& a,  const tbeam::Event& b);
  const int verbosity_;
  
  TTree* tree_;
  std::vector<tbeam::Event> v_evtInfo_;
  tbeam::Event ev;
  
  std::map<int,std::string> detIdNamemap_;
  uint32_t tdcAdd_;
  uint32_t hvAdd_;
  uint32_t DUTangAdd_;      
};

#endif
