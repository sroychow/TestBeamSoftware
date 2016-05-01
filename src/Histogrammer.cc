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
  new TH1I("window", ";window;#Events", 20,-0.5,19.5);
  new TH1I("tilt", ";tilt;#Events", 200,-0.5,199.5);
  new TH1I("condData", ";condData;#Events", 20,-0.5,19.5);
  new TH1I("glibstatus", "GLIB status;GlibStatus;#Events", 4, -0.5, 3);
  new TH1I("cbc1status", "CBC1 status;CBC1Status;#Events", 4, -0.5, 3);
  new TH1I("cbc2status", "CBC2 status;CBC1Status;#Events", 4, -0.5, 3);
  new TH1D("ntotalHitsReco","Total number of hits reco;#totalHitsReco;#Events",100,-.5,99.5);
}

void Histogrammer::bookDUTHistograms(std::string det) {
  TString d(det);
  fout_->cd();
  fout_->mkdir(d);
  fout_->cd(d);
  new TH2I("hitmapfull",d + " hitmap;strip no.;#Events",1016,-0.5,1015.5,2,-0.5,1.5);
  bookDUTHistoForColumn(d,"C0");
  bookDUTHistoForColumn(d,"C1");
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

  new TH1I("cbcStubBit","Stub Bit from CBC",16,-0.5,15.5);

  new TH1I("nstubsFromCBC","Total number of stubs from CBC stub word",20,-.5,19.5);
  new TH1I("nstubsFromReco","Total number of stubs from Reconstruction",20,-.5,19.5); 
  new TH1I("nstubsdiff","#StubsReco - #StubsfromStubWord",41,-19.5,19.5);
  new TH1I("stubMatch","Matching between CBC Stub and Reco Stub",4,0.5,4.5);
  new TH2I("stubMatchVsCBC#","Matching between CBC Stub and Reco Stub Vs CBC Id; ;CBC ID",4,0.5,4.5, 8,-0.5,7.5);
  new TH1I("stubEventsCBC","Events with CBC Stubs",2,-0.5,1.5);
  new TH1I("stubEventsReco","Events with Reco Stubs",2,-0.5,1.5);
  bookStubHistoForColumn("C0");
  bookStubHistoForColumn("C1");
}

void Histogrammer::bookStubHistoForColumn(TString c) {
  fout_->cd("StubInfo");
  new TH1I("nclusterdiff" + c,"Difference in #clusters between dut0 and dut1() for " + c + ";#cluster_{det0} - #cluster_  {det1_};Events",20,-0.5,19.5);
  new TH1I("nstubReco" + c,"Number of stubs for " + c + ";#stubs;Events",20,-0.5,19.5);
  new TH1I("nstubCBC" + c,"Number of stubs from CBC for " + c +";#stubs;Events",20,-0.5,19.5);
  new TH1I("stubEffReco" + c,"StubEfficiency for " + c + ";#stub found if det0 && det1 has cluster;Events",2,-0.5,1.5);
  new TH1I("stubEffCBC" + c,"StubEfficiency for " + c +";#of stub from CBC;Events",2,-0.5,1.5);
  new TH1I("stubProfileReco" + c,"StubProfile for " + c + ";#stub found if det0 && det1 has cluster;Events",8,-0.5,7.5);
  new TH1I("stubProfileCBC" + c,"StubProfile from CBC for " + c + ";#stub from CBC;Events",8,-0.5,7.5);
  new TH2D("stubCorrelation" + c,"StubCorrelation for " + c + ";Reconstructed Stubs;CBC Stubs",9,-0.5,8.5,9,-0.5,8.5);
}

void Histogrammer::bookCorrelationHistograms() {
  fout_->mkdir("Correlation");
  fout_->cd("Correlation");
  bookCorrelationHistoForColumn("C0");
  bookCorrelationHistoForColumn("C1");
}
    
void Histogrammer::bookCorrelationHistoForColumn(TString c) {
  new TH1D("cor_event" + c,"Sensor Hit Correlation " + c,4,0.5,4.5);
  new TH1D("diff" + c,"#Hits det0 - #Hits det1 " + c,100,-.5,99.5);
  new TH2D( "topBottomHitCorr" + c, "Hit Correlation between Top Bottom Sensors in " + c, 
            1016,-0.5,1015.5,1016,-0.5,1015.5);
  new TH2D("clusterWidthVsPos2D" + c,"dut1 clusterWidthVsPos " + c + ";Strip Number;Cluster Width",1016,-0.5,1015.5, 128,-0.5,127.5);
  new TH2D( "clusterWidthCorr" + c , "Cluster Width Correlation between Top Bottom Sensors in " + c,128,-0.5,127.5,128,-0.5,127.5);
  new TH2D( "clusterWidthCorr" + c + "_matched", "Cluster Width Correlation between Top Bottom Sensors in " + c,128,-0.5,127.5,128,-0.5,127.5);
}

void Histogrammer::closeFile() { 
  fout_->cd();
  fout_->Write();
  fout_->Close();
  isFileopen_=false;  
}
void Histogrammer::fillClusterHistograms( const char* det, std::vector<skbeam::Cluster>& cvec, 
                                          const char* col) {
  fout_->cd(det);
  TString c(col);
  Utility::fillHist1D( "ncluster" + c, cvec.size() );
  for( unsigned int i =0; i<cvec.size(); i++ ) {
    Utility::fillHist1D("clusterWidth" + c,cvec[i].width);
    Utility::fillHist1D("clusterPos" + c,cvec[i].position);
    Utility::fillHistProfile("clusterWidthVsPosProf" + c,cvec[i].position,cvec[i].width);
    Utility::fillHist2D("clusterWidthVsPos2D" + c,cvec[i].position,cvec[i].width);
  } 
}

Histogrammer::~Histogrammer() {
  if(isFileopen_)  {
    std::cout << "You forgot to close the output file!!!Closing it now" << std::endl;  
    closeFile();
   }
  delete fout_; 
}
