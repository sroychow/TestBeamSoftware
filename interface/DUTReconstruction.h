#ifndef DUTReconstruction_h
#define DUTReconstruction_h

#include "BeamAnaBase.h"

#include "TString.h"
#include "Utility.h"
#include "DataFormats.h"
#include "Histogrammer.h"

class TH1;


class DUTReconstruction : public BeamAnaBase {
 public:
  DUTReconstruction(const string inFilename,const string outFilename,int stubWindow);
  ~DUTReconstruction();
  void beginJob();
  void eventLoop(); 
  void bookHistograms();
  void clearEvent();
  void endJob();

 private:
  std::string outFile_;
  int stubWindow_;
  Histogrammer* hist_;
};
#endif
