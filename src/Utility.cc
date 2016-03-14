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

  void correctHitorder( std::vector<int>& vec ) {
    for( unsigned int i = 0; i<vec.size(); i++ ) {
      if(vec[i] < 127)
        vec[i] = 126 - vec[i];
      else
        vec[i] = 127 + (253 - vec[i]);
    }
  }

  void getChannelMaskedHit( std::vector<int>& vec, const unsigned int chLow, const unsigned int chHigh ) {
    std::vector<int> vtemp = vec;  
    vec.clear();
    for( unsigned int i = 0; i<vtemp.size(); i++ ) {
      if( vtemp[i] >= chLow && vtemp[i] <= chHigh )     continue;
      vec.push_back(vtemp[i]);
    }
  }

  void fillHistofromVec( const std::vector<int>& vec, const char* h) {
    for( unsigned int i = 0; i<vec.size(); i++ ) {
      Utility::fillHist1D(h,vec[i]);
    }
  }

  void fill2DHistofromVec( const std::vector<int>& vecC0, const std::vector<int>& vecC1,const char* h) {
    for( unsigned int i = 0; i<vecC0.size(); i++ ) {
      Utility::fillHist2D(h,vecC0[i],0);
    }
    for( unsigned int i = 0; i<vecC1.size(); i++ ) {
      Utility::fillHist2D(h,1015-vecC1[i],1);
    }
  }

  void getCBCclsuterInfo(const string detName,const std::vector<int>& hmap,
                         std::map<std::string,std::vector<Cluster> >& detClustermap) {
    if (!hmap.size()) return;
    int ch_last = -1;
    int width = 0;
    double pos = -1.;

    for( unsigned int i=0; i<hmap.size(); i++ ) {
      if( i == 0) {
	width  = 1;
	pos = hmap[i];
      } else {
	if( std::abs(hmap[i] - ch_last) == 1) {
	  width++;
	  pos += hmap[i];
	} else {
	  pos /= width;
	  Cluster ctemp0;
	  ctemp0.position = pos;
	  ctemp0.width = width;
	  detClustermap[detName].push_back(ctemp0); 
	  width = 1;
	  pos = hmap[i];
	}
      }
      ch_last = hmap[i];
    }
    Cluster ctemp;
    ctemp.position = pos/width;
    ctemp.width = width;
    detClustermap[detName].push_back(ctemp); 
    if (hmap.size() > 0 && detClustermap.size() < 1) {
      std ::cout << " Mismatch " << hmap.size() << " " << detClustermap.size() << std::endl;
      for( unsigned int i=0; i<hmap.size(); i++ ) std::cout << hmap[i] << "  " ;
      std::cout << std::endl;
    }
  
  }
  /*
  void getInfofromClusterVec(const std::vector<Cluster>& cvec,const std::string det, TFile* fout) {
    fout->cd(TString(det));
    Utility::fillHist1D( "ncluster", cvec.size() );
    for( unsigned int i =0; i<cvec.size(); i++ ) {
      Utility::fillHist1D("clusterWidth",cvec[i].width);
      Utility::fillHist1D("clusterPos",cvec[i].position);
    }
  }
  */
  void getInfofromClusterVec(const std::vector<Cluster>& cvec,const std::string det, TFile* fout, const TString col) {
    fout->cd(TString(det));
    Utility::fillHist1D( "ncluster" + col, cvec.size() );
    for( unsigned int i =0; i<cvec.size(); i++ ) {
      Utility::fillHist1D("clusterWidth" + col,cvec[i].width);
      Utility::fillHist1D("clusterPos" + col,cvec[i].position);
      Utility::fillHistProfile("clusterWidthVsPosProf" + col,cvec[i].position,cvec[i].width);
      Utility::fillHist2D("clusterWidthVsPos2D" + col,cvec[i].position,cvec[i].width);
    }
  }

  int getStubInfo( std::map<std::string,std::vector<Cluster> >&  detClustermap, const float stubwindow, 
                    TFile* fout, const std::string col) {
    std::stringstream stubHname;
    stubHname << "nstub" << col;
    std::stringstream stubEffname;
    stubEffname << "stubEff" << col;
    fout->cd();
    fout->cd("StubInfo");
    //Utility::fillHist1D("stubWord",stubWord);
    Utility::fillHist1D( "nclusterdiff" + col, std::abs( detClustermap["det0" + col].size() - 
                                                   detClustermap["det1" + col].size() ) );
    int nstubs = 0;
    for( unsigned int i = 0; i< detClustermap["det0" + col].size(); i++ ) {
      for( unsigned int j = 0; j< detClustermap["det1" + col].size(); j++ ) {
        if( std::fabs(detClustermap["det0" + col].at(i).position - 
                      detClustermap["det1" + col].at(j).position ) <= stubwindow ) {
          nstubs++;
        }
      }
    }
    Utility::fillHist1D( stubHname.str(), nstubs);
    if( !detClustermap["det0" + col].empty()&& !detClustermap["det1" + col].empty() &&nstubs>0 )
      Utility::fillHist1D(stubEffname.str(),1);
     else if( !detClustermap["det0" + col].empty() && !detClustermap["det1" + col].empty() && nstubs==0 )
       Utility::fillHist1D(stubEffname.str(),0);
    return nstubs;
  }
  int getStubInfoEDM( std::map<std::string,std::vector<Cluster> >&  detClustermap, 
		      std::vector<unsigned int> & cbcStubs, const float stubwindow, 
		      TFile* fout, const std::string col) {
    fout->cd();
    fout->cd("StubInfo");
    Utility::fillHist1D( "nclusterdiff" + col, std::abs( detClustermap["det0" + col].size() - 
                                                   detClustermap["det1" + col].size() ) );
    std::vector<unsigned int> recoStubs;
    for( unsigned int i = 0; i< detClustermap["det0" + col].size(); i++ ) {
      float pos0 = detClustermap["det0" + col].at(i).position;
      unsigned int CBC0 = pos0/127;  
      if (col.find("C1") != std::string::npos && (CBC0 == 3 || CBC0==5)) continue;
      for( unsigned int j = 0; j< detClustermap["det1" + col].size(); j++ ) {
	float pos1 = detClustermap["det1" + col].at(j).position;
	unsigned int CBC1 = pos1/127;  
	if (col.find("C1") != std::string::npos && (CBC1 == 3 || CBC1 == 5)) continue;
        if (detClustermap["det0" + col].at(i).width <= 3 && 
	    detClustermap["det1" + col].at(j).width <= 3 &&         
            std::fabs(pos0 - pos1) <= stubwindow ) {
          if (col.find("C0") != std::string::npos) recoStubs.push_back(CBC0);
          else recoStubs.push_back(CBC1);
        }
      }
    }
    Utility::fillHist1D( "nstubReco"+col, recoStubs.size());
    Utility::fillHist1D( "nstubCBC"+col, cbcStubs.size());
    
    for (unsigned int i = 0; i != cbcStubs.size(); i++) {
      Utility::fillHist1D( "stubProfileCBC"+col, cbcStubs[i]);
    }

    for (unsigned int j = 0; j != recoStubs.size(); j++) {
      Utility::fillHist1D( "stubProfileReco"+col, recoStubs[j]);
    }
    
    for (unsigned int i = 0; i < 8; i++) {
      std::vector<unsigned int>::iterator it_cbc  = find (cbcStubs.begin(),  cbcStubs.end(),  i);
      std::vector<unsigned int>::iterator it_reco = find (recoStubs.begin(), recoStubs.end(), i);
      if (it_cbc != cbcStubs.end() && it_reco != recoStubs.end()) Utility::fillHist2D( "stubCorrelation"+col, i, i);
      else if (it_cbc == cbcStubs.end() && it_reco != recoStubs.end()) Utility::fillHist2D( "stubCorrelation"+col, i, 8);
      else if (it_cbc != cbcStubs.end() && it_reco == recoStubs.end()) Utility::fillHist2D( "stubCorrelation"+col, 8, i);
    }
    
    if( !detClustermap["det0" + col].empty()&& !detClustermap["det1" + col].empty() ) {
      if (recoStubs.size() > 0 ) Utility::fillHist1D("stubEffReco"+col,1);
      else Utility::fillHist1D("stubEffReco"+col,0);   
      
      if (cbcStubs.size() > 0 ) Utility::fillHist1D("stubEffCBC"+col,1);
      else Utility::fillHist1D("stubEffCBC"+col,0);   
    }
    
    return recoStubs.size();
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

}
