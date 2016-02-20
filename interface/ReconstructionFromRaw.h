#ifndef ReconstructionFromRaw_h
#define ReconstructionFromRaw_h

#include "trawTupleBase.h"

#include "TString.h"
#include "Utility.h"
class TH1;

using Utility::Cluster;
class ReconstructionFromRaw : public trawTupleBase {
 public:
  ReconstructionFromRaw(const string inFilename,const string outFilename,float stubWindow, bool publish);
  ~ReconstructionFromRaw(){}
  void publishPlots(TString dirName);
  void bookHistogram(TFile* fout);
  void setStubWindow(float stubWindow) { stubWindow_ = stubWindow; }
  void beginJob();
  void Loop();
  void endJob();

 private:
  TString outFile;
  TFile* fout_;
  float stubWindow_;
  bool publishPng_;
  std::map<std::string,std::vector<Cluster> >  detClustermap_;
};
#endif
