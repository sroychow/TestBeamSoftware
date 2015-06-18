#ifndef __TBeamAnalysis_TreeMaker_AnalysisObjects_h
#define __TBeamAnalysis_TreeMaker_AnalysisObjects_h

#include <vector>
#include <map>
#include <string>
#include<stdint.h>
#include "TObject.h"

namespace tbeam {
  class Event :  public TObject {
   public:
    Event();
    virtual ~Event() {}
    unsigned int run;
    unsigned int lumiSection;
    unsigned int event;
    unsigned long long time;
    unsigned long int unixtime;
    unsigned long int dt;
    uint32_t key;
    uint32_t value;
    unsigned int tdcPhase;
    unsigned int HVsettings;
    unsigned int DUTangle;
    
    std::map< std::string,std::vector<int> > dut_channel;
    std::map< std::string,std::vector<int> > dut_row;
    std::map< std::string,std::vector<unsigned short> > dut_adc;
    
    ClassDef(Event,1)
  };
}
#endif
