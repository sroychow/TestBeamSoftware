#ifndef __Event_h
#define __Event_h
#include <vector>
#include <map>
#include <string>
#include<stdint.h>
#include<iostream>
#include<iomanip>
#include "TObject.h"
#include "Cluster.h"
#include "Hit.h"
#include "Cbc.h"
#include "Stub.h"
#include "Track.h"
namespace tbeam {
  class OfflineEvent :  public TObject {
  public:
    OfflineEvent();
    virtual ~OfflineEvent(){}
    void reset();
    void dumpOfflineEvent(std::ostream& os = std::cout);
    //header part
    unsigned int run;//
    unsigned int lumiSection;//
    unsigned int event;//
    unsigned long long time;//
    unsigned long int unixtime;//
    bool isSparisified;//
    
    uint8_t dataFormatVersion;//
    uint8_t condData;//
    uint8_t debugMode;//
    uint8_t readoutMode;//
    uint8_t dataType;//
    uint64_t glibStatusCode;//
    uint16_t numberOfCBC;//
    //condition data
    unsigned int tdcPhase;//
    unsigned int HVsettings;//
    unsigned int DUTangle;//
    
    uint32_t stubLatency;
    uint32_t triggerLatency;
    
    uint8_t offset1Reg;
    uint8_t offset2Reg;
    uint8_t hipRegReg;
    uint8_t vcth1Reg;
    uint8_t vcth2Reg;
    uint8_t compReg;
    uint8_t logicReg;
    uint8_t triggerLatencyReg;


    std::map<uint32_t,uint32_t > conddatamap;//this is kept for now.If not used in offline analysis, it can be dropped
    std::map<unsigned int,std::vector<tbeam::cbc> > cbcs;//
    std::map< std::string,std::vector<tbeam::hit> > dutHits;//
    std::map< std::string, std::vector<tbeam::cluster> > cbcClusters;//only in sparsified mode
    std::map<std::string, std::vector<tbeam::stub>> cbcStubs;//
    std::map< std::string, std::vector<tbeam::cluster> > offlineClusters;//only in unsparsified mode
    std::map< std::string, std::vector<tbeam::stub> > offlineStubs;//created from offlineClusters or cbcClusters
    //At the ntuplizer stage this is a empty
    //This vector of tracks is defined here to avoid redifining OfflineEvent class in offline analysis
    //This vector will be filled in Merge step of Telescope Ntuple + EdmNtuple
    std::vector<tbeam::OfflineTrack>  tracks;
    ClassDef(OfflineEvent,1)
      };
}
#endif
