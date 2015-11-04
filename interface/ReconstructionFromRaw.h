#ifndef ReconstructionFromRaw_h
#define ReconstructionFromRaw_h

#include "trawTupleBase.h"

#include "TString.h"
#include "Utility.h"
class TH1;

using Utility::Cluster;
class ReconstructionFromRaw : public trawTupleBase {
 public:
  ReconstructionFromRaw(const string inFilename,const string outFilename);
  ~ReconstructionFromRaw(){}
  void bookHistogram(TFile* fout);
  void beginJob();
  void Loop();
  void endJob();

 private:
  TString outFile;
  TFile* fout_;
  std::map<std::string,std::vector<Cluster> >  detClustermap_;
};
#endif
