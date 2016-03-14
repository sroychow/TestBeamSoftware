#include "TROOT.h"
#include "TInterpreter.h"
#include "TH1.h"
#include "TH2.h"
#include "TKey.h"
#include "TClass.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include <map>
#include <utility>
#include <vector>
#include <sstream>
#include "Reconstruction.h"
#include "telescopeBase.h"
using std::vector;
using std::map;
//Reconstruction::Reconstruction(const TString& inFilename,const TString&  outFilename) :
Reconstruction::Reconstruction(const string inFilename,const string in_telFilename,const string outFilename,float stubWindow, bool publish) :
  tBeamBase::tBeamBase(TString(inFilename))
{
  outFile = outFilename; 
  stubWindow_ = stubWindow; 
  publishPng_ = publish;
  std::cout << "EDM Tuple Analyzer initialized with the following options\n" 
            << "Infile: " << inFilename
            << "\nOutFile: " << outFile
            << "\nStubWindow: " << stubWindow_
            << "\nPublishPng: " << publishPng_
            << std::endl; 
  beginJob();

  nEvents            = 0;
  nEventsNoHits      = 0;
  nEventsHitInBoth  = 0;
  nEventsHitInDet0  = 0;
  nEventsHitInDet1  = 0;
  telFileName = in_telFilename;
}
void Reconstruction::bookHistogram(TFile* fout_) {
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
  

  fout_->cd();
  fout_->mkdir("det0");
  fout_->cd("det0");
  new TH1I("chsizeC0","dut0 channel occupancy per event col0;#Channels;#Events",51,-0.5,50.5);
  new TH1I("chsizeC1","dut0 channel occupancy per event col1;#Channels;#Events",51,-0.5,50.5);
  new TH2I("hitmapfull","dut0 hitmap;strip no.;#Events",1016,-0.5,1015.5,2,-0.5,1.5);
  new TH1I("hitmapC0","dut0 hitmap col0;strip no.;#Events",1016,-0.5,1015.5);
  new TH1I("hitmapC1","dut0 hitmap col1;strip no.;#Events",1016,-0.5,1015.5);
  new TH1D("nclusterC0","#cluster dut0 col0;#Clusters;#Events",51,-0.5,50.5);
  new TH1D("nclusterC1","#cluster dut0 col1;#Clusters;#Events",51,-0.5,50.5);
  new TH1I("clusterWidthC0","dut0 clusterWidth col0;#ClusterWidth;#Events",20,-0.5,19.5);
  new TH1I("clusterWidthC1","dut0 clusterWidth col1;#ClusterWidth;#Events",20,-0.5,19.5);
  new TH1D("clusterPosC0","dut0 clusterPos col0;Strip Number;#Events",1016,-0.5,1015.5);
  new TH1D("clusterPosC1","dut0 clusterPos col1;Strip Number;#Events",1016,-0.5,1015.5);
  new TProfile("clusterWidthVsPosProfC0","dut0 clusterWidthVsPos col0;Strip Number;Cluster Width",1016,-0.5,1015.5);
  new TProfile("clusterWidthVsPosProfC1","dut0 clusterWidthVsPos col1;Strip Number;Cluster Width",1016,-0.5,1015.5);
  new TH2D("clusterWidthVsPos2DC0","dut0 clusterWidthVsPos col0;Strip Number;Cluster Width",1016,-0.5,1015.5, 20,-0.5,19.5);
  new TH2D("clusterWidthVsPos2DC1","dut0 clusterWidthVsPos col1;Strip Number;Cluster Width",1016,-0.5,1015.5, 20,-0.5,19.5);
  fout_->cd();
  
  fout_->mkdir("det1");
  fout_->cd("det1");
  new TH1I("chsizeC0","dut1 channel occupancy per event col0;#Channels;#Events",51,-0.5,50.5);
  new TH1I("chsizeC1","dut1 channel occupancy per event col1;#Channels;#Events",51,-0.5,50.5);
  new TH2I("hitmapfull","dut1 hitmap;strip no.;#Events",1016,-0.5,1015.5,2,-0.5,1.5);
  new TH1I("hitmapC0","dut1 hitmap col0;strip no.;#Events",1016,-0.5,1015.5);
  new TH1I("hitmapC1","dut1 hitmap col1;strip no.;#Events",1016,-0.5,1015.5);
  new TH1D("nclusterC0","#cluster dut1 col0;#Clusters;#Events",51,-0.5,50.5);
  new TH1D("nclusterC1","#cluster dut1 col1;#Clusters;#Events",51,-0.5,50.5);
  new TH1I("clusterWidthC0","dut1 clusterWidth col0;#ClusterWidth;#Events",20,-0.5,19.5);
  new TH1I("clusterWidthC1","dut1 clusterWidth col1;#ClusterWidth;#Events",20,-0.5,19.5);
  new TH1D("clusterPosC0","dut1 clusterPos col0;Strip Number;#Events",1016,-0.5,1015.5);
  new TH1D("clusterPosC1","dut1 clusterPos col1;Strip Number;#Events",1016,-0.5,1015.5);
  new TProfile("clusterWidthVsPosProfC0","dut1 clusterWidthVsPos col0;Strip Number;Cluster Width",1016,-0.5,1015.5);
  new TProfile("clusterWidthVsPosProfC1","dut1 clusterWidthVsPos col1;Strip Number;Cluster Width",1016,-0.5,1015.5);
  new TH2D("clusterWidthVsPos2DC0","dut1 clusterWidthVsPos col0;Strip Number;Cluster Width",1016,-0.5,1015.5, 20,-0.5,19.5);
  new TH2D("clusterWidthVsPos2DC1","dut1 clusterWidthVsPos col1;Strip Number;Cluster Width",1016,-0.5,1015.5, 20,-0.5,19.5);

  std::stringstream ss;
  ss << stubWindow_;
  TString ssW(ss.str());
  fout_->mkdir("StubInfo");
  fout_->cd("StubInfo");
  new TH1I("cbcStubBit","Stub Bit from CBC",16,-0.5,15.5);
  new TH1I("nclusterdiffC0","Difference in #clusters between dut0 and dut1() col0;#cluster_{det0_col0} - #cluster_  {det1_col0};Events",20,-0.5,19.5);
  new TH1I("nclusterdiffC1","Difference in #clusters between dut0 and dut1() col1;#cluster_{det0_col1} - #cluster_  {det1_col1};Events",20,-0.5,19.5);
  new TH1I("nstubRecoC0","Number of stubs sw=" + ssW +" col0;#stubs;Events",20,-0.5,19.5);
  new TH1I("nstubRecoC1","Number of stubs sw=" + ssW +" col1;#stubs;Events",20,-0.5,19.5);
  new TH1I("nstubCBCC0","Number of stubs from CBC col0;#stubs;Events",20,-0.5,19.5);
  new TH1I("nstubCBCC1","Number of stubs from CBC col1;#stubs;Events",20,-0.5,19.5);
  new TH1I("stubEffRecoC0","StubEfficiency sw=" + ssW + " col0;#stub found if det0 && det1 has cluster;Events",2,-0.5,1.5);
  new TH1I("stubEffRecoC1","StubEfficiency sw=" + ssW + " col1;#stub found if det0 && det1 has cluster;Events",2,-0.5,1.5);
  new TH1I("stubEffCBCC0","StubEfficiency col1;#of stub from CBC;Events",2,-0.5,1.5);
  new TH1I("stubEffCBCC1","StubEfficiency col0;#of stub found CBC;Events",2,-0.5,1.5);
  new TH1I("stubProfileRecoC0","StubProfile sw=" + ssW + " col0;#stub found if det0 && det1 has cluster;Events",8,-0.5,7.5);
  new TH1I("stubProfileRecoC1","StubProfile sw=" + ssW + " col1;#stub found if det0 && det1 has cluster;Events",8,-0.5,7.5);
  new TH1I("stubProfileCBCC0","StubProfile from CBC col0;#stub from CBC in Col0;Events",8,-0.5,7.5);
  new TH1I("stubProfileCBCC1","StubProfile from CBC col1;#stub from CBC in col1;Events",8,-0.5,7.5);
  new TH2D("stubCorrelationC1","StubCorrelation col1;Reconstructed Stubs;CBC Stubs",9,-0.5,8.5,9,-0.5,8.5);
  new TH2D("stubCorrelationC0","StubCorrelation col0;Reconstructed Stubs;CBC Stubs",9,-0.5,8.5,9,-0.5,8.5);
  new TH1I("nstubsFromCBC","Total number of stubs from CBC stub word",20,-.5,19.5);
  new TH1I("nstubsFromReco","Total number of stubs from Reconstruction(stub window " + ssW + ")",20,-.5,19.5); 
  new TH1I("nstubsdiff","#StubsReco - #StubsfromStubWord",41,-19.5,19.5);
  new TH1I("stubMatch","Matching between CBC Stub and Reco Stub",4,0.5,4.5);
  new TH1I("stubEventsCBC","Events with CBC Stubs",2,-0.5,1.5);
  new TH1I("stubEventsReco","Events with Reco Stubs",2,-0.5,1.5);

  fout_->mkdir("Correlation");
  fout_->cd("Correlation");
  new TH1D("cor_eventC0","Sensor Hit Correlation col0",4,0.5,4.5);
  new TH1D("cor_eventC1","Sensor Hit Correlation col1",4,0.5,4.5);
  new TH1D("diffC0","#Hits det0 - #Hits det1 col0",100,-.5,99.5);
  new TH1D("diffC1","#Hits det0 - #Hits det1 col1",100,-.5,99.5);
  new TH2D( "topBottomHitCorrC0", "Hit Correlation between Top Bottom Sensors in Column 0", 
	    1016,-0.5,1015.5,1016,-0.5,1015.5);
  new TH2D( "topBottomHitCorrC1", "Hit Correlation between Top Bottom Sensors in Column 1",
	    1016,-0.5,1015.5,1016,-0.5,1015.5);
  fout_->cd();
}

void Reconstruction::beginJob() {
  fout_ = TFile::Open(outFile,"RECREATE");
  bookHistogram(fout_);
}
 
void Reconstruction::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();

   Long64_t nbytes = 0, nb = 0;
   cout << "#Events=" << nentries << endl;
   fout_->cd("EventInfo");
   Utility::fillHist1D("nevents", nentries);
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     //for (Long64_t jentry=0; jentry<50;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     if (jentry%1000 == 0)
       cout << " Events processed. " << std::setw(8) << jentry 
	    << endl;
     // if (Cut(ientry) < 0) continue;
     
     if(jentry==0) {
       std::cout << "Entry>>" << jentry << "\n";
       std::cout << "Is channel map empty>>" << dut_channel->empty() << "\n" << endl;
       for ( std::map<string,std::vector<int> >::const_iterator it = dut_channel->begin(); it != dut_channel->end(); ++it) 
	 std::cout << it->first << ":" << (it->second).size() << '\n' ;
       std::cout << "HV=" << hvSettings
		 << "\ndutAngle=" << dutAngle
                 << "\nvcth=" << vcth
                 << "\noffset=" << offset
                 << "\nwindow=" << window
                 << "\ntilt=" << tilt
                 << "\ncondData=" << condData
                 << std::endl;
     }
     nEvents++;
     fout_->cd("EventInfo");
     Utility::fillHist1D("hvSettings", hvSettings);
     Utility::fillHist1D("dutAngle", dutAngle);
     Utility::fillHist1D("vcth", vcth);
     Utility::fillHist1D("offset", offset);
     Utility::fillHist1D("window", window);
     Utility::fillHist1D("tilt", tilt);
     Utility::fillHist1D("condData", condData);
     Utility::fillHist1D("glibstatus", glibStatus);
     Utility::fillHist1D("cbc1status", cbc1Status);
     Utility::fillHist1D("cbc2status", cbc2Status);

     // getting telescope information
     telescopeBase tel(telFileName);
     bool hasChain = tel.isValidChain();
     bool hasTk = hasChain && tel.hasTrk(jentry);
     if(!hasTk) continue;
     
     
     //cout << "Point 1" << endl;
     vector<int> dut0_chtempC0;
     vector<int> dut0_chtempC1;
     vector<int> dut1_chtempC0;
     vector<int> dut1_chtempC1;
     bool hExistD0 = false;
     bool hExistD1 = false;
     if( dut_channel->find("det0") != dut_channel->end() ) {
       if ((dut_channel->at("det0")).size()) hExistD0 = true;
       for( unsigned int j = 0; j<(dut_channel->at("det0")).size(); j++ ) {
	 int ch = (dut_channel->at("det0")).at(j);
	 if( ch <= 1015 )  dut0_chtempC0.push_back(ch);
	 else dut0_chtempC1.push_back(ch-1016);
       }
     }
     
      if( dut_channel->find("det1") != dut_channel->end() ) {
       if ((dut_channel->at("det1")).size()) hExistD1 = true;
       for( unsigned int j = 0; j<(dut_channel->at("det1")).size(); j++ ) {
         int ch = (dut_channel->at("det1")).at(j);
         if( ch <= 1015 )  dut1_chtempC0.push_back(ch);
         else  dut1_chtempC1.push_back(ch-1016);
       }
      }
      //cout << "Point 2" << endl;
     if (!hExistD0 && !hExistD1) nEventsNoHits++;
     if ( hExistD0 && hExistD1)  nEventsHitInBoth++;
     if ( hExistD0 && !hExistD1) nEventsHitInDet0++;
     if (!hExistD0 && hExistD1)  nEventsHitInDet1++;

      fout_->cd("det0");
      //      Utility::getChannelMaskedHit( dut0_chtempC1, 379, 510 );      
      //      Utility::getChannelMaskedHit( dut0_chtempC1, 633, 763 );
      Utility::fillHist1D("chsizeC0", dut0_chtempC0.size());
      Utility::fillHist1D("chsizeC1", dut0_chtempC1.size());
      Utility::fillHistofromVec(dut0_chtempC0,"hitmapC0");
      Utility::fillHistofromVec(dut0_chtempC1,"hitmapC1");
      Utility::fill2DHistofromVec(dut0_chtempC0,dut0_chtempC1,"hitmapfull");
      getCBCclsuterInfo("det0C0", dut0_chtempC0, detClustermap_);
      getCBCclsuterInfo("det0C1", dut0_chtempC1, detClustermap_);
      //cout << "Point 3" << endl;

      fout_->cd("det1");
      //Utility::correctHitorder(dut0_chtemp);
      //      Utility::getChannelMaskedHit( dut1_chtempC1, 379, 510 );      
      //      Utility::getChannelMaskedHit( dut1_chtempC1, 633, 763 );
      Utility::fillHist1D("chsizeC0", dut1_chtempC0.size());
      Utility::fillHist1D("chsizeC1", dut1_chtempC1.size());
      Utility::fillHistofromVec(dut1_chtempC0,"hitmapC0");
      Utility::fillHistofromVec(dut1_chtempC1,"hitmapC1");
      Utility::fill2DHistofromVec(dut1_chtempC0,dut1_chtempC1,"hitmapfull");
      getCBCclsuterInfo("det1C0", dut1_chtempC0, detClustermap_);
      getCBCclsuterInfo("det1C1", dut1_chtempC1, detClustermap_);
      //cout << "Point 4" << endl;  
     
      fout_->cd("Correlation");

      if(dut0_chtempC0.size() && !dut1_chtempC0.size()) Utility::fillHist1D("cor_eventC0", 1);
      if(!dut0_chtempC0.size() && dut1_chtempC0.size()) Utility::fillHist1D("cor_eventC0", 2);
      if(dut0_chtempC0.size() && dut1_chtempC0.size()) Utility::fillHist1D("cor_eventC0", 3);
      if(!dut0_chtempC0.size() && !dut1_chtempC0.size()) Utility::fillHist1D("cor_eventC0", 4);

      if(dut0_chtempC1.size() && !dut1_chtempC1.size()) Utility::fillHist1D("cor_eventC1", 1);
      if(!dut0_chtempC1.size() && dut1_chtempC1.size()) Utility::fillHist1D("cor_eventC1", 2);
      if(dut0_chtempC1.size() && dut1_chtempC1.size()) Utility::fillHist1D("cor_eventC1", 3);
      if(!dut0_chtempC1.size() && !dut1_chtempC1.size()) Utility::fillHist1D("cor_eventC1", 4);

      Utility::fillHist1D("diffC0",std::abs(dut0_chtempC0.size()-dut1_chtempC0.size())); 
      Utility::fillHist1D("diffC1",std::abs(dut0_chtempC1.size()-dut1_chtempC1.size())); 
      if (dut0_chtempC0.size() ==1 && dut1_chtempC0.size() ==1) 
	Utility::fillHist2D("topBottomHitCorrC0",dut0_chtempC0[0], dut1_chtempC0[0]);
      if (dut0_chtempC1.size() ==1 && dut1_chtempC1.size() ==1) 
	Utility::fillHist2D("topBottomHitCorrC1",dut0_chtempC1[0], dut1_chtempC1[0]);

      Utility::getInfofromClusterVec(detClustermap_["det0C0"],"det0",fout_,"C0");
      Utility::getInfofromClusterVec(detClustermap_["det0C1"],"det0",fout_,"C1");
      Utility::getInfofromClusterVec(detClustermap_["det1C0"],"det1",fout_,"C0");
      Utility::getInfofromClusterVec(detClustermap_["det1C1"],"det1",fout_,"C1");

      int totalStubsRecoC0=0; 
      int totalStubsRecoC1=0; 

      std::vector<unsigned int> stubBitsC0;
      std::vector<unsigned int> stubBitsC1;
      if (stubWord > 0) {
	fout_->cd();
	fout_->cd("StubInfo");
	for (unsigned int i = 0; i < 16; i++) {
          if ((stubWord >> i) & 0x1) Utility::fillHist1D("cbcStubBit",i);
          if (i == 11 || i == 13) continue;
          if ((stubWord >> i) & 0x1) {
	    if (i <= 7) stubBitsC0.push_back(i);
            else stubBitsC1.push_back(i-8);
	  }
	}
      }

      totalStubsRecoC0 = Utility::getStubInfoEDM(detClustermap_,stubBitsC0,stubWindow_,fout_,"C0");
      totalStubsRecoC1 = Utility::getStubInfoEDM(detClustermap_,stubBitsC1,stubWindow_,fout_,"C1");
      //if( totalStubsRecoC0 )  Utility::fillHist1D("stubEffC0",1);
      //else                    Utility::fillHist1D("stubEffC0",0);
      //if( totalStubsRecoC1 )  Utility::fillHist1D("stubEffC1",1);
      //else                    Utility::fillHist1D("stubEffC1",0);

      int totStubReco = totalStubsRecoC0 + totalStubsRecoC1;
      int totStubCBC  = stubBitsC0.size()+stubBitsC1.size();
      Utility::fillHist1D("nstubsFromCBC",totStubCBC);
      Utility::fillHist1D("nstubsFromReco",totStubReco);

      if (totStubReco > 0)  Utility::fillHist1D("stubEventsReco", 1);
      else  Utility::fillHist1D("stubEventsReco",0 );
      if ( totStubCBC > 0) Utility::fillHist1D("stubEventsCBC",1); 
      else  Utility::fillHist1D("stubEventsCBC",0);  
     
      if (!totStubReco && !totStubCBC) Utility::fillHist1D("stubMatch", 1);
      if (!totStubReco && totStubCBC) Utility::fillHist1D("stubMatch", 2);
      if (totStubReco && !totStubCBC) Utility::fillHist1D("stubMatch", 3);
      if (totStubReco && totStubCBC) Utility::fillHist1D("stubMatch", 4);
      Utility::getHist1D("stubMatch")->GetXaxis()->SetBinLabel(1,"!CBC && !RECO");  
      Utility::getHist1D("stubMatch")->GetXaxis()->SetBinLabel(2,"CBC && !RECO");  
      Utility::getHist1D("stubMatch")->GetXaxis()->SetBinLabel(3,"!CBC && RECO");  
      Utility::getHist1D("stubMatch")->GetXaxis()->SetBinLabel(4,"CBC && RECO");  

      Utility::fillHist1D("nstubsdiff",(totStubReco -totStubCBC));


      detClustermap_.clear();
      fout_->cd("EventInfo");
      //Utility::fillHist1D("ntotalHitsRaw", totalHits);
      Utility::fillHist1D("ntotalHitsReco", dut0_chtempC0.size()+dut0_chtempC1.size() +
                          dut1_chtempC0.size() + dut1_chtempC1.size());
   }
}
void Reconstruction::publishPlots(TString dirName) {
  
  if( dirName == "StubInfo") {
  std::stringstream ss;
  ss << stubWindow_;
  TString sw(ss.str());
  TCanvas* cCanvas = new TCanvas( "c1", "c1", 800, 600);
  fout_->cd();
  fout_->cd("StubInfo");
    Utility::getHist1D("nstubC0")->Draw();
  cCanvas->SaveAs("nstubsC0.png");
  cCanvas->Clear();
  
  Utility::getHist1D("nstubC1")->Draw();
  cCanvas->SaveAs("nstubsC1.png");
  cCanvas->Clear();

  std::stringstream sb1,sb2;
  TH1* hseffc0 = Utility::getHist1D("stubEffC0");
  sb1 << hseffc0->GetBinContent(1)/hseffc0->Integral();
  sb2 << hseffc0->GetBinContent(2)/hseffc0->Integral();
  hseffc0->GetXaxis()->SetBinLabel(1,sb1.str().c_str());  
  hseffc0->GetXaxis()->SetBinLabel(2,sb2.str().c_str());  
  hseffc0->Draw();
  cCanvas->SaveAs("stubEffC0.png");
  cCanvas->Clear();
  sb1.str("");
  sb2.str("");
   
  TH1* hseffc1 = Utility::getHist1D("stubEffC1");
  sb1 << hseffc1->GetBinContent(1)/hseffc1->Integral();
  sb2 << hseffc1->GetBinContent(2)/hseffc1->Integral();
  hseffc1->GetXaxis()->SetBinLabel(1,sb1.str().c_str());  
  hseffc1->GetXaxis()->SetBinLabel(2,sb2.str().c_str());  
  hseffc1->Draw();
  cCanvas->SaveAs("stubEffC1.png");
  cCanvas->Clear();

  Utility::getHist1D("nstubsFromRaw")->Draw();
  cCanvas->SaveAs("nstubsFromRaw.png");
  cCanvas->Clear();  

  Utility::getHist1D("nstubsFromReco" )->Draw();
  cCanvas->SaveAs("nstubsFromReco.png");
  cCanvas->Clear();

  Utility::getHist1D("nstubsdiff")->Draw();
  cCanvas->SaveAs("nstubsdiff.png");
  cCanvas->Clear(); 
  
  fout_->cd();
  }
  else {
  TDirectory* dir = dynamic_cast<TDirectory*>(fout_->Get(dirName));
  std::cout << "Dir=" << dir->GetName() << std::endl; 
  //dir->GetListOfKeys()->Print();
  TIter next(dir->GetListOfKeys());
  TKey *hkey;
  while ((hkey = (TKey*)next())) {
    //std::cout << "Entered Here" << std::endl;
    TClass *cl = gROOT->GetClass(hkey->GetClassName());  
    if ( cl->InheritsFrom("TH1") ) {
        TH1* h = dynamic_cast<TH1*>(hkey->ReadObj());
	h->SetDirectory(0);
        std::cout << h->GetName() << std::endl;
	TString canvasName("c" + std::string(hkey->GetName()));
	TCanvas* myCanvas = new TCanvas( canvasName, "c1", 800, 600);
	myCanvas->cd();
	h->Draw();  
        myCanvas->SaveAs( dir->GetName() + canvasName + TString(".png"));
    } else if( cl->InheritsFrom("TH2") ) {
        TH2* h = dynamic_cast<TH2*>(hkey->ReadObj());
	h->SetDirectory(0);
        std::cout << h->GetName() << std::endl;
	TString canvasName("c" + std::string(hkey->GetName()));
	TCanvas* myCanvas = new TCanvas( canvasName, "c1", 800, 800);
	myCanvas->cd();
	h->Draw();  
        myCanvas->SaveAs( dir->GetName() + canvasName + TString(".png"));
    }
  }
   } 
}

void Reconstruction::endJob() {
  fout_->cd();
  fout_->Write();
  if( publishPng_ ) {
    publishPlots("det0");
    publishPlots("det1");
    publishPlots("StubInfo");
  }
  std::cout << " Total Number Of Events " << nEvents << std::endl;
  std::cout << " Total Number Of Events w/o Hits " << nEventsNoHits << std::endl;
  std::cout << " Total Number Of Events with Hits " << nEvents - nEventsNoHits << std::endl;
  std::cout << " Total Number Of Events with Hits in Det0 Only " << nEventsHitInDet0 << std::endl;
  std::cout << " Total Number Of Events with Hits in Det1 Only " << nEventsHitInDet1 << std::endl;
  std::cout << " Total Number Of Events with Hits in both Detectors " << nEventsHitInBoth << std::endl;
  fout_->Close();
  delete fout_;
}
