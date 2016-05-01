#ifndef TelMatchDUTReconstruction_h
#define TelMatchDUTReconstruction_h

#include "BeamAnaBase.h"

#include "TString.h"
#include "Utility.h"
#include "DataFormats.h"
#include "Histogrammer.h"

class TH1;


class TelMatchDUTReconstruction : public BeamAnaBase {
 public:
  TelMatchDUTReconstruction(const string inFilename,const string telFileName,const string outFilename,int stubWindow);
  ~TelMatchDUTReconstruction();
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
