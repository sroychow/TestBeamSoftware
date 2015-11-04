#ifndef __UTILITY__HH
#define __UTILITY__HH

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "TMath.h"
#include "TLorentzVector.h"
#include "TDirectory.h"
#include "TH1.h"
class TFile;
using std::string;

namespace Utility {
  
  struct Cluster {
   float position;
   int width;
  };

  void correctHitorder( std::vector<int>& vec );
  void fillHistofromVec( const std::vector<int>& vec, const char* h);
  void getCBCclsuterInfo( const string detName,const std::vector<int>& hmap,
                          std::map<std::string,std::vector<Cluster> >&  detClustermap);
  void getInfofromClusterVec(const std::vector<Cluster>& cvec,const std::string det,TFile* fout);
  void getStubInfo(std::map<std::string,std::vector<Cluster> >&  detClustermap, const int stubwindow, 
                   TFile* fout);

  TH1* getHist1D(const char* hname);
  TH1* getHist1D(const string& hname);

  template <class T>
  bool fillHist1D(const char* hname, T value, double w=1.0) 
  {
    TH1* h = getHist1D(hname);
    if (!h) return false;
    h->Fill(value, w);
    return true;
  }
  template <class T>
  bool fillHist1D(const string& hname, T value, double w=1.0) {
    return fillHist1D(hname.c_str(), value, w);
  }
}
#endif
