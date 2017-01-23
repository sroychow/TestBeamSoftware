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
#include "TF1.h"
#include "TProfile.h"
#include "stdint.h"
#include "DataFormats.h"
class TFile;
using std::string;

namespace Utility {
  
  void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiter);
  void getChannelMaskedHits( std::vector<int>& vec, const std::vector<int>& mch );
  void getChannelMaskedClusters( std::vector<tbeam::cluster*>& vec, const std::vector<int>& mch );
  void getChannelMaskedStubs( std::vector<tbeam::stub*>& vec, const std::vector<int>& mch );
 
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
  
  //  ------------------------------------------ //
  //  Convenience routines for fitting residuals //
  //  ------------------------------------------ //

  static double FuncPol1Gaus(Double_t * x, Double_t * par){
    double xx = x[0];
    double mean = par[1];
    double sigma = par[2];
    double norm = par[0];
    double cte = par[3];
    double slope = par[4];
    double f = norm*exp(-0.5*((xx-mean)/sigma)*((xx-mean)/sigma));
    f = f + cte + slope * xx;
    return f;
  }

  static double funcStepGaus(Double_t * x, Double_t * par){
    double xx = x[0];
    double pitch = par[0];
    double sigma = par[1];
    double norm = par[2];
    double cte = par[3];
    double f =0;
    if (xx<0) f = norm*(1+TMath::Erf((xx+pitch/2.)/(sqrt(2)*sigma)));
    if (xx>0) f = norm*(1-TMath::Erf((xx-pitch/2.)/(sqrt(2)*sigma)));
    f += cte;
    return f;
  }

  static double FuncStepGausShift(Double_t * x, Double_t * par){
    double xx = x[0];
    double pitch = par[0];
    double sigma = par[1];
    double norm = par[2];
    double cte = par[3];
    double shift = par[4];
    double f =0;
    if (xx<shift) f = norm*(1+TMath::Erf((xx+pitch/2.-shift)/(sqrt(2)*sigma)));
    if (xx>shift) f = norm*(1-TMath::Erf((xx-pitch/2.-shift)/(sqrt(2)*sigma)));
    f += cte;
    return f;
  }


  void removeTrackDuplicates(std::vector<double> *xTk, std::vector<double> *yTk, std::vector<double> *xTkNoOverlap, std::vector<double> *yTkNoOverlap);
  void removeTrackDuplicates(std::vector<double> *xTk, std::vector<double> *yTk, std::vector<double> *slopeTk, std::vector<double> *xTkNoOverlap, std::vector<double> *yTkNoOverlap, std::vector<double> *slopeTkNoOverlap);
  void removeTrackDuplicates(const tbeam::TelescopeEvent *telEv, std::vector<tbeam::Track>& tkNoOverlap);
  
  void cutTrackFei4Residuals(std::vector<double> *xTk, std::vector<double> *yTk, std::vector<int> *colFei4, std::vector<int> *rowFei4, std::vector<double> *xSelectedTk, std::vector<double> *ySelectedTk, double xResMean, double yResMean, double xResPitch, double yResPitch);
  void cutTrackFei4Residuals(std::vector<double> *xTk, std::vector<double> *yTk, std::vector<double> *slopeTk, std::vector<int> *colFei4, std::vector<int> *rowFei4, std::vector<double> *xSelectedTk, std::vector<double> *ySelectedTk, std::vector<double> *slopeSelectedTk, double xResMean, double yResMean, double xResPitch, double yResPitch);
  
  void cutTrackFei4Residuals(const tbeam::FeIFourEvent* fei4ev ,const std::vector<tbeam::Track>& tkNoOverlap, std::vector<tbeam::Track>& selectedTk, double xResMean, double yResMean, double xResPitch, double yResPitch, bool doClosestTrack);

  double extrapolateTrackAtDUTwithAngles(const tbeam::Track& track, double FEI4_z, double offset, double zPlane, double theta);
  std::pair<double, double> extrapolateTrackAtDUTwithAngles(const tbeam::Track& track, double FEI4_z, double offset_d0, double zDUT_d0, double deltaZ, double theta);

}
#endif
