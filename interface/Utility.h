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
#include "stdint.h"
class TFile;
using std::string;

namespace Utility {
  
  void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiter);
  //void getChannelMaskedHit( std::vector<int>& vec, const unsigned int chLow, const unsigned int chHigh );
 
  int readStubWord( std::map<std::string,std::vector<unsigned int> >& stubids, const uint32_t sWord );
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
   template <class T>
  void fillHistofromVec( const std::vector<T>& vec, const char* h) {
    for( unsigned int i = 0; i<vec.size(); i++ ) {
      fillHist1D(h,vec.at(i));
    }
  }
  void fill2DHistofromVec( const std::vector<int>& vecC0, const std::vector<int>& vecC1,const char* h);

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
