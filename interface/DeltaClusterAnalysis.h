#ifndef DeltaClusterAnalysis_h
#define DeltaClusterAnalysis_h

#include "BeamAnaBase.h"

#include "TString.h"
//#include "Utility.h"
#include "DataFormats.h"
#include "Histogrammer.h"

class TH1;
class DeltaClusterAnalysis : public BeamAnaBase {
 public:
  DeltaClusterAnalysis(const string inFilename,const string outFilename);
  ~DeltaClusterAnalysis();
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
