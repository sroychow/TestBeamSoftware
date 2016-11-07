#ifndef TelescopeAnalysis_h
#define TelescopeAnalysis_h

#include "BeamAnaBase.h"

#include "TString.h"
//#include "Utility.h"
#include "DataFormats.h"
#include "Histogrammer.h"

class TH1;
class TelescopeAnalysis : public BeamAnaBase {
 public:
  TelescopeAnalysis();
  ~TelescopeAnalysis();
  void beginJob();
  void eventLoop(); 
  void bookHistograms();
  void clearEvent();
  void endJob();
/*
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
*/
 private:
  Histogrammer* hist_;
  unsigned long int nEntries_; 
};
#endif
