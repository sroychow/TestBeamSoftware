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
#include "TH2.h"
#include "TProfile.h"
class TFile;
using std::string;

namespace Utility {
  
  struct Cluster {
   float position;
   int width;
  };
  void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiter);
  void correctHitorder( std::vector<int>& vec );
  void getChannelMaskedHit( std::vector<int>& vec, const unsigned int chLow, const unsigned int chHigh );
  void fillHistofromVec( const std::vector<int>& vec, const char* h);
  void fill2DHistofromVec( const std::vector<int>& vecC0, const std::vector<int>& vecC1,const char* h);
  void getCBCclsuterInfo( const string detName,const std::vector<int>& hmap,
                          std::map<std::string,std::vector<Cluster> >&  detClustermap);
  void getInfofromClusterVec(const std::vector<Cluster>& cvec,const std::string det,
                             TFile* fout, const TString col = "");
  //void getInfofromClusterVec(const std::vector<Cluster>& cvec,const std::string det,TFile* fout,TString col);
  int getStubInfo(std::map<std::string,std::vector<Cluster> >&  detClustermap, const float stubwindow, 
                   TFile* fout,const std::string col = "");
  int getStubInfoEDM(std::map<std::string,std::vector<Cluster> >&  detClustermap, 
		   std::vector<unsigned int> & cbcStubs, const float stubwindow, 
                   TFile* fout,const std::string col = "");

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

  // ---------------------------------------------
  // Convenience routine for filling 2D histograms
  // ---------------------------------------------
  TH2* getHist2D(const char* hname);
  TH2* getHist2D(const std::string& hname);
  template <class T1, class T2>
  bool fillHist2D(const char* hname, T1 xvalue, T2 yvalue, double w=1.0) {
    TH2* h = getHist2D(hname);
    if (!h) return false;
    h->Fill(xvalue, yvalue, w);
    return true;
  }
  template <class T1, class T2>
  bool fillHist2D(const std::string& hname, T1 xvalue, T2 yvalue, double w=1.0) {
    return fillHist2D(hname.c_str(), xvalue, yvalue, w);
  }

  // ---------------------------------------------
  // Convenience routine for filling Profile histograms
  // ---------------------------------------------
  TProfile* getHistProfile(const char* hname);
  TProfile* getHistProfile(const std::string& hname);
  template <class T1, class T2>
  bool fillHistProfile(const char* hname, T1 xvalue, T2 yvalue) {
    TProfile* h = getHistProfile(hname);
    if (!h) return false;
    h->Fill(xvalue, yvalue);
    return true;
  }
  template <class T1, class T2>
  bool fillHistProfile(const std::string& hname, T1 xvalue, T2 yvalue) {
    return fillHistProfile(hname.c_str(), xvalue, yvalue);
  }
}
#endif
