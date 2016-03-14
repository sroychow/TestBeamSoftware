#ifndef Reconstruction_h
#define Reconstruction_h

#include "tBeamBase.h"

#include "TString.h"
#include "Utility.h"
class TH1;

using Utility::Cluster;
class Reconstruction : public tBeamBase {
 public:
  Reconstruction(const string inFilename,const string in_telFilename,const string outFilename,float stubWindow, bool publish);
  ~Reconstruction(){}
  void bookHistogram(TFile* fout);
  void publishPlots(TString dirName);
  void setStubWindow(float stubWindow) { stubWindow_ = stubWindow; }
  void beginJob();
  void Loop();
  void endJob();
  bool tel_ok(Long64_t jentry);

 private:
  TString outFile, telFileName;
  TFile* fout_;
  float stubWindow_;
  bool publishPng_;
  std::map<std::string,std::vector<Cluster> >  detClustermap_;
  int nEvents;
  int nEventsNoHits;
  int nEventsHitInBoth;
  int nEventsHitInDet0;
  int nEventsHitInDet1;
};
#endif
