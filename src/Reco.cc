#include "Reco.h"
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

namespace Reco {
  //Function to swap the ordering of the hits
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

  void getCBCclsuterInfo(const string detName,const std::vector<int>& hmap,
                         std::map<std::string,std::vector<skbeam::Cluster> >& detClustermap) {
    std::vector<skbeam::Cluster> tempV;
    detClustermap[detName]=tempV;
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
	  skbeam::Cluster ctemp0;
	  ctemp0.position = pos;
	  ctemp0.width = width;
	  detClustermap[detName].push_back(ctemp0); 
	  width = 1;
	  pos = hmap[i];
	}
      }
      ch_last = hmap[i];
    }
    skbeam::Cluster ctemp;
    ctemp.position = pos/width;
    ctemp.width = width;
    detClustermap[detName].push_back(ctemp); 
    if (hmap.size() > 0 && detClustermap.size() < 1) {
      std ::cout << " Mismatch " << hmap.size() << " " << detClustermap.size() << std::endl;
      for( unsigned int i=0; i<hmap.size(); i++ ) std::cout << hmap[i] << "  " ;
      std::cout << std::endl;
    }
  
  }
  
  int getRecoStubInfo( const std::map<std::string,std::vector<skbeam::Cluster> >*  detClustermap, 
		       const float stubwindow, std::map<std::string,std::vector<skbeam::Stub> >& recoStubs,
                       const std::string col) {
    std::vector<skbeam::Stub> tempV;
    recoStubs[col]=tempV;
    /*
    //Stub reco when seeding in det0
    for( unsigned int i = 0; i< detClustermap->at("det0" + col).size(); i++ ) {
      float pos0 = detClustermap->at("det0" + col).at(i).position;
      unsigned int CBC0 = pos0/127;  
      if (col.find("C1") != std::string::npos && (CBC0 == 3 || CBC0==5)) continue;
      for( unsigned int j = 0; j< detClustermap->at("det1" + col).size(); j++ ) {
	float pos1 = detClustermap->at("det1" + col).at(j).position;
	unsigned int CBC1 = pos1/127;  
	if (col.find("C1") != std::string::npos && (CBC1 == 3 || CBC1 == 5)) continue;
        if (detClustermap->at("det0" + col).at(i).width <= 3 && 
	    detClustermap->at("det1" + col).at(j).width <= 3 &&         
            std::fabs(pos0 - pos1) <= stubwindow ) {
          skbeam::Stub stemp;
          stemp.det0Cl = detClustermap->at("det0" + col).at(i);
          stemp.det1Cl = detClustermap->at("det1" + col).at(j);
          if (col.find("C0") != std::string::npos) {
            stemp.cbcid = CBC0;
            recoStubs[col].push_back(stemp);
          }
          else {
            stemp.cbcid = CBC1; 
            recoStubs[col].push_back(stemp);
          }
        }
      }
    }
    return recoStubs.size();
    */
    //Stub reco when seeding in det1
    for( unsigned int i = 0; i< detClustermap->at("det1" + col).size(); i++ ) {
      float pos1 = detClustermap->at("det1" + col).at(i).position;
      unsigned int CBC1 = pos1/127;  
      if (col.find("C1") != std::string::npos && (CBC1 == 3 || CBC1==5)) continue;
      for( unsigned int j = 0; j< detClustermap->at("det0" + col).size(); j++ ) {
	float pos0 = detClustermap->at("det0" + col).at(j).position;
	unsigned int CBC0 = pos0/127;  
	if (col.find("C1") != std::string::npos && (CBC0 == 3 || CBC0 == 5)) continue;
        if (detClustermap->at("det1" + col).at(i).width <= 3 && 
	    detClustermap->at("det0" + col).at(j).width <= 3 &&         
            std::fabs(pos0 - pos1) <= stubwindow ) {
          skbeam::Stub stemp;
          stemp.det0Cl = detClustermap->at("det1" + col).at(i);
          stemp.det1Cl = detClustermap->at("det0" + col).at(j);
          if (col.find("C0") != std::string::npos) {
            stemp.cbcid = CBC0;
            recoStubs[col].push_back(stemp);
          }
          else {
            stemp.cbcid = CBC1; 
            recoStubs[col].push_back(stemp);
          }
        }
      }
    }
    return recoStubs.size();
  }
  
  int getCBCStubInfo( std::map<std::string,std::vector<unsigned int> >& cbcStubs, const UInt_t sWord ) {
    std::vector<unsigned int> tempV;
    cbcStubs["C0"]=tempV;
    cbcStubs["C1"]=tempV;
    int ncbcSw = 0;
    if (sWord > 0) {
	for (unsigned int i = 0; i < 16; i++) {
          if (i == 11 || i == 13) continue;
          if ((sWord >> i) & 0x1) {
            ncbcSw++;
	    if (i <= 7) cbcStubs["C0"].push_back(i);
            else cbcStubs["C1"].push_back(i-8);
	  }
	}
      }
    return ncbcSw;  
  }
}
