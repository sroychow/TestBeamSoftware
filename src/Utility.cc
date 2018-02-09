#include "Utility.h"
#include <climits>
#include <cmath>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>
#include "TLorentzVector.h"
#include "TFile.h"
using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::setw;

using std::string;

namespace Utility {
  void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters) {
  
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  
    // Find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);
  
    while (std::string::npos != pos || string::npos != lastPos)  {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
    
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
    
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
    }
  }

  void getChannelMaskedHits( std::vector<int>& vec, const std::vector<int>& mch ) {
    std::vector<int> vtemp = vec;
    vec.clear();
    for(auto& ch : vtemp) {
      //std::cout << ch;
      if( std::find(mch.begin(), mch.end(), ch) != mch.end() )   continue;
      vec.push_back(ch);

    }
  }

  void getChannelMaskedClusters( std::vector<tbeam::cluster*>& vec, const std::vector<int>& mch ) {
    //std::vector<tbeam::cluster*> vtemp = vec;
    //vec.clear();
    //for(auto& clus : vtemp) {
    //  //std::cout << ch;
    //  if( std::find(mch.begin(), mch.end(), clus->x) != mch.end() )   continue;
    //  vec.push_back(clus);
    //}
  }

  void getChannelMaskedStubs( std::vector<tbeam::stub*>& vec, const std::vector<int>& mch ) {
    //std::vector<tbeam::stub*> vtemp = vec;
    //vec.clear();
    //for(auto& stub : vtemp) {
      //std::cout << ch;
    //  if( std::find(mch.begin(), mch.end(), stub->x) != mch.end() )   continue;
    //  vec.push_back(stub);
    //}
  }

  void fill2DHistofromVec( const std::vector<int>& vecC0, const std::vector<int>& vecC1,const char* h) {
    for( unsigned int i = 0; i<vecC0.size(); i++ ) {
      Utility::fillHist2D(h,vecC0.at(i),0);
    }
    for( unsigned int i = 0; i<vecC1.size(); i++ ) {
      Utility::fillHist2D(h,1015-vecC1.at(i),1);
    }
  }

  int readStubWord( std::map<std::string,std::vector<unsigned int> >& stubids, const uint32_t sWord ) {
    int ncbcSw = 0;
    if (sWord > 0) {
	for (unsigned int i = 0; i < 16; i++) {
#ifdef NOV_15
          if (i == 11 || i == 13) continue;//only for nov15
#endif
          if ((sWord >> i) & 0x1) {
            ncbcSw++;
	    if (i <= 7) stubids.at("C0").push_back(i);
            else stubids.at("C1").push_back(i-8);
	  }
	}
      }
    return ncbcSw;  
  }

  // ------------------------------------------------------------------------
  // Convenience routine for filling 1D histograms. We rely on root to keep 
  // track of all the histograms that are booked all over so that we do not 
  // have to use any global variables to save the histogram pointers. Instead, 
  // we use the name of the histograms and gROOT to retrieve them from the 
  // Root object pool whenever necessary. This is the closest one can go to 
  // hbook and ID based histogramming
  // -------------------------------------------------------------------------
  
  TH1* getHist1D(const char* hname) {
    TObject *obj = gDirectory->GetList()->FindObject(hname); 
    if (!obj) {
      std::cerr << "**** getHist1D: Histogram for <" << hname << "> not found!" << std::endl;
      return 0;
    }
    TH1 *h = 0;
    if (obj->InheritsFrom("TH1D"))
      h = dynamic_cast<TH1D*>(obj);
    else if (obj->InheritsFrom("TH1C"))
      h = dynamic_cast<TH1C*>(obj);
    //else if (obj->InheritsFrom("TH1K"))
    //  h = dynamic_cast<TH1K*>(obj);
    else if (obj->InheritsFrom("TH1S"))
      h = dynamic_cast<TH1S*>(obj);
    else if (obj->InheritsFrom("TH1I"))
      h = dynamic_cast<TH1I*>(obj);
    else
      h = dynamic_cast<TH1F*>(obj);
    
    if (!h) {
      std::cerr << "**** getHist1D: <" << hname << "> may not be a 1D Histogram" << std::endl;
      return 0;
    }
    return h;
  }
  
  TH1* getHist1D(const string& hname) {
    return getHist1D(hname.c_str());
  }
  // ---------------------------------------------
  // Convenience routine for filling 2D histograms
  // ---------------------------------------------
  TH2* getHist2D(const char* hname) {
    TObject *obj = gDirectory->GetList()->FindObject(hname); 
    if (!obj) {
      cerr << "**** getHist2D: Histogram for <" << hname << "> not found!" << endl;
      return nullptr;
    }
    
    TH2 *h = nullptr;
    if (obj->InheritsFrom("TH2D"))
      h = dynamic_cast<TH2D*>(obj);
    else if (obj->InheritsFrom("TH2C"))
      h = dynamic_cast<TH2C*>(obj);
    else if (obj->InheritsFrom("TH2S"))
      h = dynamic_cast<TH2S*>(obj);
    else if (obj->InheritsFrom("TH2I"))
      h = dynamic_cast<TH2I*>(obj);
    else
      h = dynamic_cast<TH2F*>(obj);
    
    if (!h) {
      cerr << "**** getHist2D: <<" << hname << ">> may not be a 2D Histogram" << endl;
      return nullptr;
    }
    return h;
  }
  TH2* getHist2D(const string& hname) {
    return getHist2D(hname.c_str());
  }

  TProfile* getHistProfile(const char* hname) {
    TObject *obj = gDirectory->GetList()->FindObject(hname);
    if (!obj) {
      cerr << "**** getHistProfile: Histogram for <" << hname << "> not found!" << endl;
      return nullptr;
    }
    TProfile *h = nullptr;
    if (obj->InheritsFrom("TProfile"))
      h = dynamic_cast<TProfile*>(obj);

    if (!h) {
      cerr << "**** getHistProfile: <<" << hname << ">> may not be a 2Profile Histogram" << endl;
      return nullptr;
    }
    return h;
  }
  TProfile* getHistProfile(const string& hname) {
    return getHistProfile(hname.c_str());
  }

  // ---------------------------------------------
  // Convenience routine for track cleaning
  // ---------------------------------------------

  double extrapolateTrackAtDUTwithAngles(const tbeam::OfflineTrack& track, double FEI4_z, double offset, double zDUT, double theta){

    float xInit = track.xPosPrevHit();
    //float xInit = track.yPosPrevHit();
    //float slope = track.dydz();
    float slope = track.dxdz();
    float zTkIP = FEI4_z;
    //float zTkIP = FEI4_z - track.xPosPrevHit()*sin(24*TMath::Pi()/180.);
    //float zTkIP = FEI4_z + track.xPosPrevHit()*sin(24*TMath::Pi()/180.);

    //float xInit = track.xPos();
    //float zTkIP = FEI4_z;

    //Compute distance between DUT center and track impact at DUT along X 
    float xTkAtDUT = xInit + (zDUT - zTkIP)*slope;
    xTkAtDUT = (xTkAtDUT + offset)/ (cos(theta)*(1.-slope*tan(theta)));
    return xTkAtDUT; 
  }

  std::pair<double, double> extrapolateTrackAtDUTwithAngles(const tbeam::OfflineTrack& track, double FEI4_z, double offset_d0, double zDUT_d0, double deltaZ, double theta){

    float xInit = track.xPosPrevHit();
    //float xInit = track.yPosPrevHit();
    float slope = track.dxdz();
    //float slope = track.dydz();
    float zTkIP = FEI4_z;
    //float zTkIP = FEI4_z - track.xPosPrevHit()*sin(24*TMath::Pi()/180.);
    //float zTkIP = FEI4_z + track.xPosPrevHit()*sin(24*TMath::Pi()/180.);

    //float xInit = track.xPos();
    //float zTkIP = FEI4_z;

    //Compute distance between DUT center and track impact at DUT along X 
    //float xTkAtDUT_d0 = xInit + (zDUT_d0 - zTkIP)*track.dxdz();
    float xTkAtDUT_d0 = xInit + (zDUT_d0 - zTkIP)*slope;
    xTkAtDUT_d0 = (xTkAtDUT_d0 + offset_d0)/ (cos(theta)*(1.-slope*tan(theta)));

    double zDUT_d1 = zDUT_d0 + deltaZ*cos(theta);
    float xTkAtDUT_d1 = xInit + (zDUT_d1 - zTkIP)*slope;
    double offset_d1 = offset_d0 + sin(theta)*deltaZ;
    xTkAtDUT_d1 = (xTkAtDUT_d1 + offset_d1)/ (cos(theta)*(1.-slope*tan(theta)));

    std::pair<double, double> xTkAtDUT;
    xTkAtDUT.first = xTkAtDUT_d0;
    xTkAtDUT.second = xTkAtDUT_d1;
    return xTkAtDUT;
  }

  std::pair<double, double> extrapolateTrackAtDUTwithAngles(const tbeam::OfflineTrack& track, double FEI4_z, double offset_d0, double zDUT_d0, double deltaZ, double theta, double offsetPlanes){

    //float xInit = track.yPosPrevHit();
    float xInit = track.xPosPrevHit();
    float slope = track.dxdz();
    //float slope = track.dydz();
    float zTkIP = FEI4_z;
    //float zTkIP = FEI4_z - track.xPosPrevHit()*sin(24*TMath::Pi()/180.);
    //float zTkIP = FEI4_z + track.xPosPrevHit()*sin(24*TMath::Pi()/180.);

    //float xInit = track.xPos();
    //float zTkIP = FEI4_z;

    //Compute distance between DUT center and track impact at DUT along X 
    float xTkAtDUT_d0 = xInit + (zDUT_d0 - zTkIP)*slope;
    xTkAtDUT_d0 = (xTkAtDUT_d0 + offset_d0)/ (cos(theta)*(1.-slope*tan(theta)));

    double zDUT_d1 = zDUT_d0 + deltaZ*cos(theta) + offsetPlanes*sin(theta);
    float xTkAtDUT_d1 = xInit + (zDUT_d1 - zTkIP)*slope;
    double offset_d1 = offset_d0 + sin(theta)*deltaZ - offsetPlanes*cos(theta);
    xTkAtDUT_d1 = (xTkAtDUT_d1 + offset_d1)/ (cos(theta)*(1.-slope*tan(theta)));

    std::pair<double, double> xTkAtDUT;
    xTkAtDUT.first = xTkAtDUT_d0;
    xTkAtDUT.second = xTkAtDUT_d1;
    return xTkAtDUT;
  }

  std::pair<double, double> extrapolateTrackAtDUTwithAngles(const tbeam::OfflineTrack& track, double FEI4_z, double offset_d0, double zDUT_d0, double deltaZ, double theta, double offsetPlanes, double phi_d0, double phi_d1){

    float xInit = track.xPosPrevHit();
    float yInit = track.yPosPrevHit();
    float slopeX = track.dxdz();
    float slopeY = track.dydz();
    float zTkIP = FEI4_z;

    float xTkAtDUT_d0_init = xInit + (zDUT_d0 - zTkIP)*slopeX;
    float yTkAtDUT_d0_init = yInit + (zDUT_d0 - zTkIP)*slopeY;

    float xTkAtDUT_d0 = cos(phi_d0)*xTkAtDUT_d0_init + sin(phi_d0)*yTkAtDUT_d0_init;
    //float yTkAtDUT_d0 = -sin(phi_d0)*xTkAtDUT_d0_init + cos(phi_d0)*yTkAtDUT_d0_init;

    xTkAtDUT_d0 = (xTkAtDUT_d0 + offset_d0)/ (cos(theta)*(1.-slopeX*tan(theta)));
    
    double zDUT_d1 = zDUT_d0 + deltaZ*cos(theta) + offsetPlanes*sin(theta);
    float xTkAtDUT_d1_init = xInit + (zDUT_d1 - zTkIP)*slopeX;
    float yTkAtDUT_d1_init = yInit + (zDUT_d1 - zTkIP)*slopeY;

     float xTkAtDUT_d1 = cos(phi_d1)*xTkAtDUT_d1_init + sin(phi_d1)*yTkAtDUT_d1_init;
     //yTkAtDUT_d1 = -sin(phi_d1)*xTkAtDUT_d1_init + cos(phi_d1)*yTkAtDUT_d1_init;

    double offset_d1 = offset_d0 + sin(theta)*deltaZ - offsetPlanes*cos(theta);
    xTkAtDUT_d1 = (xTkAtDUT_d1 + offset_d1)/ (cos(theta)*(1.-slopeX*tan(theta)));

    std::pair<double, double> xTkAtDUT;
    xTkAtDUT.first = xTkAtDUT_d0;
    xTkAtDUT.second = xTkAtDUT_d1;
    return xTkAtDUT;

  }

}
