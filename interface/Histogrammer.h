#ifndef __Histogrammer__HH
#define __Histogrammer__HH

#include "TDirectory.h"
#include "TProfile.h"
class TFile;
using std::string;

class Histogrammer {
  public:
    Histogrammer();
    ~Histogrammer();
  private:
    TFile* fout_;  
};
#endif
