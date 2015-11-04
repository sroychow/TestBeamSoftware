#ifndef Reconstruction_h
#define Reconstruction_h

#include "tBeamBase.h"

#include "TString.h"
#include "Utility.h"
class TH1;

using Utility::Cluster;
class Reconstruction : public tBeamBase {
 public:
  Reconstruction(const string inFilename,const string outFilename);
  ~Reconstruction(){}
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
