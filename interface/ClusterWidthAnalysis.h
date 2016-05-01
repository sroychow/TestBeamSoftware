#ifndef ClusterWidthAnalysis_h
#define ClusterWidthAnalysis_h

#include "BeamAnaBase.h"

#include "TString.h"
#include "Utility.h"
#include "DataFormats.h"
#include "Histogrammer.h"

class TH1;


class ClusterWidthAnalysis : public BeamAnaBase {
 public:
  ClusterWidthAnalysis(const string inFilename,const string telFileName,const string outFilename,int stubWindow,
                       int clustSize, bool doTelmatch);
  ~ClusterWidthAnalysis();
  void beginJob();
  void eventLoop(); 
  void bookHistograms();
  void clearEvent();
  void endJob();

 private:
  std::string outFile_;
  int stubWindow_;
  int clswSize_;
  bool telMatch_;
  Histogrammer* hist_;
};
#endif
