#ifndef __Reco__HH
#define __Reco__HH

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
#include "DataFormats.h"
class TFile;
using std::string;
namespace Reco {
  void correctHitorder( std::vector<int>& vec );
  void getChannelMaskedHit( std::vector<int>& vec, const unsigned int chLow, const unsigned int chHigh );

  void getCBCclsuterInfo( const string detName,const std::vector<int>& hmap,
                          std::map<std::string,std::vector<skbeam::Cluster> >&  detClustermap); 
  int getRecoStubInfo( const std::map<std::string,std::vector<skbeam::Cluster> >*  detClustermap, 
		       const float stubwindow, std::map<std::string,std::vector<skbeam::Stub> >& recoStubs,
                       const std::string col );
  int getCBCStubInfo( std::map<std::string,std::vector<unsigned int> >& cbcStubs, const UInt_t sWord );
}
#endif
