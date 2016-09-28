#ifndef TelescopeAnalysis_h
#define TelescopeAnalysis_h

#include "BeamAnaBase.h"

#include "TString.h"
//#include "Utility.h"
#include "DataFormats.h"
#include "Histogrammer.h"

class TH1;
class TelescopeAnalysis : public BeamAnaBase {
 public:
  TelescopeAnalysis(const string inFilename,const string outFilename);
  ~TelescopeAnalysis();
  void beginJob();
  void eventLoop(); 
  void bookHistograms();
  void clearEvent();
  void endJob();

 private:
  std::string outFile_;
  Histogrammer* hist_;
  unsigned long int nEntries_; 
};
#endif
