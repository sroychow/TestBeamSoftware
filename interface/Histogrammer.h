#ifndef __Histogrammer__HH
#define __Histogrammer__HH

#include "TDirectory.h"
#include "TProfile.h"
#include "TFile.h"
#include "Utility.h"
//#include "DataFormats.h"
#include "Event.h"
#include<string>
#include<vector>
class Histogrammer {
  public:
    Histogrammer(std::string& outFile);
    virtual ~Histogrammer();
    void bookEventHistograms();
    void bookDUTHistograms(std::string det);
    void bookDUTHistoForColumn(TString& d, TString c);
    void bookStubHistograms(std::string& d) {
      TString td(d);
      bookStubHistograms(td);
    }
    void bookStubHistograms(TString& d);
    void bookStubHistoForColumn(TString c);
    void bookCorrelationHistograms(TString& modId);
    void bookCorrelationHistograms(std::string& modId) {
      TString td(modId);
      bookCorrelationHistograms(td);
    }
    void bookCorrelationHistoForColumn(TString c);

    //histograms for common track propoerties
    void bookTrackCommonHistograms();

    //Histograms to be used by alignment analysis
    void bookTrackFitHistograms(TString& detId, float zMin = 0., float zStep = 0., int zNsteps = 0);
    void FillAlignmentOffsetVsZ(const char*, const char*, int, float, float, float);

    //Hisograms used by analysis with stub match
    void bookTrackMatchHistograms(TString& detId);
    void bookTrackMatchHistograms(std::string& d) {
      TString td(d);
      std::cout << "Entering bookTrackMatchHistograms with dnmae" << d << std::endl;
      bookTrackMatchHistograms(d);
    }

    void bookCBCHistograms(std::string cbc);
    //common histogram utility functions
    TH1* GetHistoByName(const char*, const char* );
    TH1* GetHistoByName(const std::string& dir, const std::string& hname);

    TH2* Get2DHistoByName(const char*, const char* );
    TH2* Get2DHistoByName(const std::string& dir, const std::string& hname);

    TProfile* GetProfileByName(const char*, const char* );
    TProfile* GetProfileByName(const std::string& dir, const std::string& hname);

    template <class T>
    void fillHist1D(const char* dir, const char* histo, T val, double w=1.0) {
      fout_->cd(dir);
      Utility::fillHist1D(histo, val);
    }
    template <class T>
    void fillHist1D(const std::string& dir, const std::string& histo, T val, double w=1.0) {
      fillHist1D(dir.c_str(), histo.c_str(), val);
    }
    template <class T1, class T2>
    void fillHist2D(const char* dir, const char* histo, T1 xval, T2 yval) {
      fout_->cd(dir);
      Utility::fillHist2D(histo, xval, yval);
    }
    template <class T1, class T2>
    void fillHist2D(const std::string& dir, const std::string& histo, T1 xval, T2 yval) {
      fillHist2D(dir.c_str(), histo.c_str(), xval, yval);
    }

    template <class T>
    void fillHistofromVec( const std::vector<T>& vec, const char* dir, const char* h) {
      fout_->cd(dir);
      Utility::fillHistofromVec( vec, h);
    }
    template <class T1, class T2>
    void fill2DHistofromVec( const std::vector<T1>& vecC0, const std::vector<T2>& vecC1,
                             const char* dir, const char* h) {
      fout_->cd(dir);
      Utility::fill2DHistofromVec( vecC0, vecC1,h);
    }

    template <class T1, class T2>
    bool fillHistProfile(const char* dir, const char* hname, T1 xvalue, T2 yvalue) {
      fout_->cd(dir);
      Utility::fillHistProfile(hname,xvalue,yvalue);
    }
    template <class T1, class T2>
    bool fillHistProfile(const std::string& dir, const std::string& hname, T1 xvalue, T2 yvalue) {
      fillHistProfile(dir.c_str(),hname.c_str(), xvalue, yvalue);
    }
    void fillClusterHistograms( const string& det, const std::vector<tbeam::cluster>& cvec, const char* col) {
      fillClusterHistograms(det.c_str(), cvec, col);
    }
    void closeFile();

    TFile* hfile() const { return fout_;}
  private:
    TFile* fout_;
    bool isFileopen_;
};
#endif
