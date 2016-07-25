#ifndef AlignmentAnalysis_h
#define AlignmentAnalysis_h

#include "BeamAnaBase.h"

#include "TString.h"
//#include "Utility.h"
#include "DataFormats.h"
#include "Histogrammer.h"

class TH1;
class AlignmentAnalysis : public BeamAnaBase {
 public:
  AlignmentAnalysis(const string inFilename,const string outFilename);
  ~AlignmentAnalysis();
  void beginJob();
  void eventLoop(); 
  std::pair<float, float>  GetOffsetVsZ(const char*, float **, float**);
  static double FuncStepGaus(Double_t *, Double_t *);
  void bookHistograms();
  void clearEvent();
  void endJob();

 private:
  std::string runNumber;
  std::string outFile_;
  Histogrammer* hist_;
  unsigned long int nEntries_; 
};
#endif
