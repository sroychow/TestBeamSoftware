/*!
        \file                Histogrammer.cc
        \brief               Class provides user to book, fill histograms
        \author              Suvankar Roy Chowdhury
        \date                05/07/16
        Support :            mail to : suvankar.roy.chowdhury@cern.ch
*/
#include "Histogrammer.h"
#include <climits>
#include <cmath>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "TLorentzVector.h"
#include "TFile.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::setw;

Histogrammer::Histogrammer(std::string& outFile) {
  fout_ = new TFile(TString(outFile),"RECREATE");
  isFileopen_ = true;
}

void Histogrammer::bookEventHistograms() {
  fout_->cd();
  fout_->mkdir("EventInfo");
  fout_->cd("EventInfo");
  new TH1I("nevents", "#Events", 10000001, -0.5, 10000000.5);
  new TH1I("dutAngle", "DUT Angle;DUTAngle;#Events", 3100, -0.5, 3099.5);  
  new TH1I("hvSettings", "High Voltage settings;HV;#Events", 1000,-0.5,999.5);
  new TH1I("vcth", "Vcth value;vcth;#Events", 200,-0.5,199.5);
  new TH1I("offset", ";offset;#Events", 200,-0.5,199.5);
  new TH1I("window", ";window;#Events", 200,-0.5,199.5);
  new TH1I("tilt", ";tilt;#Events", 200,-0.5,199.5);
  new TH1I("condData", ";condData;#Events", 20,-0.5,19.5);
  new TH1I("tdcPhase",";tdc;#Events",17,-0.5,16.5);
  new TH1I("isPeriodic",";isPeriodic;#Events",2,-0.5,1.5);
  new TH1I("isGoodFlag",";isGood;#Events",2,-0.5,1.5);
}

void Histogrammer::bookDUTHistograms(std::string det) {
  TString d(det);
  fout_->cd();
  fout_->mkdir(d);
  fout_->cd(d);
  new TH2I("hitmapfull",d + " hitmap;strip no.;#Events",1016,-0.5,1015.5,2,-0.5,1.5);
  bookDUTHistoForColumn(d,"C0");
  //bookDUTHistoForColumn(d,"C1");
}
void Histogrammer::bookDUTHistoForColumn(TString& d, TString c) {
  fout_->cd(d);    
  new TH1I("chsize" + c ,"dut0 channel occupancy per event" + c + ";#Channels;#Events",51,-0.5,50.5);
  new TH1I("hitmap" + c,"dut0 hitmap " + c + ";strip no.;#Events",1016,-0.5,1015.5);
  new TH1D("ncluster" + c,"#cluster dut0 " + c + ";#Clusters;#Events",51,-0.5,50.5);
  new TH1I("clusterWidth" + c,"dut0 clusterWidth " + c +";#ClusterWidth;#Events",128,-0.5,127.5);
  new TH1D("clusterPos" + c,"dut0 clusterPos " + c + ";Strip Number;#Events",1016,-0.5,1015.5);
  new TProfile("clusterWidthVsPosProf" + c,"dut0 clusterWidthVsPos " + c + ";Strip Number;Cluster Width",1016,-0.5,1015.5);
  new TH2D("clusterWidthVsPos2D" + c ,"dut0 clusterWidthVsPos " + c + ";Strip Number;Cluster Width",1016,-0.5,1015.5, 20,-0.5,19.5);
}

void Histogrammer::bookStubHistograms() {
  fout_->cd();
  fout_->mkdir("StubInfo");
  fout_->cd("StubInfo");

  new TH1I("cbcStubWord","Stub Bit from CBC",16,-0.5,15.5);
  new TH1I("recoStubWord","Stub Bit from offline CBC logic emulation",16,-0.5,15.5);
  new TH1I("nstubsFromCBCSword","Total number of stubs from CBC stub word",20,-.5,19.5);
  new TH1I("nstubsFromRecoSword","Total number of stubs from Reco Stub word",20,-.5,19.5); 
  new TH1I("nstubsFromReco","Total number of stubs from Reconstruction",20,-.5,19.5); 
  new TH1I("stubMatch","Matching between CBC Stub and Reco Stub",4,0.5,4.5);
  new TH1I("nstubsdiffSword","#StubsRecoStubword - #StubsfromStubWord",20,-0.5,19.5);
  new TH1I("nstubsdiff","#StubsReco - #StubsfromStubWord",20,-0.5,19.5);
  bookStubHistoForColumn("C0");
  //bookStubHistoForColumn("C1");
}

void Histogrammer::bookStubHistoForColumn(TString c) {
  fout_->cd("StubInfo");
  new TH1I("nstubReco" + c,"Number of stubs for " + c + " from offline reconstruction;#stubs;Events",20,-0.5,19.5);
}

void Histogrammer::bookCorrelationHistograms() {
  fout_->mkdir("Correlation");
  fout_->cd("Correlation");
  bookCorrelationHistoForColumn("C0");
  //bookCorrelationHistoForColumn("C1");
}
    
void Histogrammer::bookCorrelationHistoForColumn(TString c) {
  new TH1D("cor_hit" + c,"Sensor Hit Correlation " + c,4,0.5,4.5);
  new TH1I("nclusterdiff" + c,"Difference in #clusters between dut0 and dut1() for " + c + ";#cluster_{det0} - #cluster_  {det1_};Events",20,-0.5,19.5);
  //new TH1D("nhitdiff" + c,"#Hits det0 - #Hits det1 " + c,100,-.5,99.5);
}


void Histogrammer::bookTelescopeMatchedHistograms() {
  fout_->mkdir("TelescopeMatch");
  fout_->cd("TelescopeMatch");
  new TH1D("nTrackParams","#Tracks Telescope;#tracks;#events",30,0,30);
  new TH1D("xpos","Xpos of track at FEI4 plane",200,-20.,20.);
  new TH1D("xtkatDUT0","Xpos of extrapolated track at DUT0 plane",200,-20.,20.);
  new TH1D("xtkatDUT1","Xpos of extrapolated track at DUT1 plane",200,-20.,20.);
  new TH1D("residualDUT0","Residual at DUT0 plane",20000,-20.,20.);
  new TH1D("residualDUT1","Residual at DUT1 plane",20000,-20.,20.);

}

void Histogrammer::closeFile() { 
  fout_->cd();
  fout_->Write();
  fout_->Close();
  isFileopen_=false;  
}
void Histogrammer::fillClusterHistograms( const char* det, std::vector<tbeam::cluster>& cvec, 
                                          const char* col) {
  fout_->cd(det);
  TString c(col);
  Utility::fillHist1D( "ncluster" + c, cvec.size() );
  for( unsigned int i =0; i<cvec.size(); i++ ) {
    Utility::fillHist1D("clusterWidth" + c,cvec[i].size);
    Utility::fillHist1D("clusterPos" + c,cvec[i].x);
    Utility::fillHistProfile("clusterWidthVsPosProf" + c,cvec[i].x,cvec[i].size);
    Utility::fillHist2D("clusterWidthVsPos2D" + c,cvec[i].x,cvec[i].size);
  } 
}

Histogrammer::~Histogrammer() {
  if(isFileopen_)  {
    std::cout << "You forgot to close the output file!!!Closing it now" << std::endl;  
    closeFile();
   }
  delete fout_; 
}
