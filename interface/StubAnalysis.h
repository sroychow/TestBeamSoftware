#ifndef StubAnalysis_h
#define StubAnalysis_h

#include "BeamAnaBase.h"

#include "TString.h"
//#include "Utility.h"
#include "Histogrammer.h"

class TH1;
class StubAnalysis : public BeamAnaBase {
 public:
  StubAnalysis();
  ~StubAnalysis();
  void beginJob();
  void eventLoop();
  void bookHistograms();
  void printEfficiency();
  void clearEvent();
  void endJob();

 private:
  //std::string outFile_;
  Histogrammer* hist_;
  unsigned long int nEntries_;
  unsigned long int maxEvent_;
};
#endif
