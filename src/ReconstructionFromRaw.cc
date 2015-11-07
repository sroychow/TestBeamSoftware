#include "TROOT.h"
#include "TInterpreter.h"
#include "TH1.h"
#include "TH2.h"
#include <map>
#include <utility>
#include <vector>
#include <sstream>
#include <iomanip>
#include "ReconstructionFromRaw.h"

using std::vector;
using std::map;
//ReconstructionFromRaw::ReconstructionFromRaw(const TString& inFilename,const TString&  outFilename) :
ReconstructionFromRaw::ReconstructionFromRaw(const string inFilename,const string outFilename) :
  trawTupleBase::trawTupleBase(TString(inFilename))
{
  outFile = outFilename;  
  beginJob();
}
void ReconstructionFromRaw::bookHistogram(TFile* fout_) {
  fout_->cd();
  fout_->mkdir("EventInfo");
  fout_->cd("EventInfo");
  new TH1I("nevents", "#Events", 10000001, -0.5, 10000000.5);

  fout_->cd();
  fout_->mkdir("det0");
  fout_->cd("det0");
  new TH1I("chsizeC0","dut0 channel occupancy per event col0",30,-0.5,29.5);
  new TH1I("chsizeC1","dut0 channel occupancy per event col1",30,-0.5,29.5);
  new TH2I("hitmapfull","dut0 hitmap;strip no.,No. of Events",1016,0.5,1016.5,2,-0.5,1.5);
  new TH1I("hitmapC0","dut0 hitmap col0;strip no.,No. of Events",1016,0.5,1016.5);
  new TH1I("hitmapC1","dut0 hitmap col1;strip no.,No. of Events",1016,0.5,1016.5);
  new TH1D("nclusterC0","#cluster dut0 col0",40,-0.5,39.5);
  new TH1D("nclusterC1","#cluster dut0 col1",40,-0.5,29.5);
  new TH1I("clusterWidthC0","dut0 clusterWidth col0",20,-0.5,19.5);
  new TH1I("clusterWidthC1","dut0 clusterWidth col1",20,-0.5,19.5);
  new TH1D("clusterPosC0","dut0 clusterPos col0",1016,0.5,1016.5);
  new TH1D("clusterPosC1","dut0 clusterPos col1",1016,0.5,1016.5);
  fout_->cd();
  
  fout_->mkdir("det1");
  fout_->cd("det1");
  new TH1I("chsizeC0","dut1 channel occupancy per event col0",30,-0.5,29.5);
  new TH1I("chsizeC1","dut1 channel occupancy per event col1",30,-0.5,29.5);
  new TH2I("hitmapfull","dut1 hitmap;strip no.,No. of Events",1016,0.5,1016.5,2,-0.5,1.5);
  new TH1I("hitmapC0","dut1 hitmap col0;strip no.,No. of Events",1016,0.5,1016.5);
  new TH1I("hitmapC1","dut1 hitmap col1;strip no.,No. of Events",1016,0.5,1016.5);
  new TH1D("nclusterC0","#cluster dut1 col0",40,-0.5,39.5);
  new TH1D("nclusterC1","#cluster dut1 col1",40,-0.5,29.5);
  new TH1I("clusterWidthC0","dut1 clusterWidth col0",20,-0.5,19.5);
  new TH1I("clusterWidthC1","dut1 clusterWidth col1",20,-0.5,19.5);
  new TH1D("clusterPosC0","dut1 clusterPos col0",1016,0.5,1016.5);
  new TH1D("clusterPosC1","dut1 clusterPos col1",1016,0.5,1016.5);

  fout_->mkdir("StubInfo");
  fout_->cd("StubInfo");
  new TH1D("nclusterdiffC0","Difference in #clusters between dut0 and dut1() col0;#cluster_{det0_col0} - #cluster_  {det1_col0};Events",20,-0.5,19.5);
  new TH1D("nclusterdiffC1","Difference in #clusters between dut0 and dut1() col1;#cluster_{det0_col1} - #cluster_  {det1_col1};Events",20,-0.5,19.5);
  new TH1D("nstub7C0","Number of stubs(stub Window 7) col0;#stubs;Events",20,-0.5,19.5);
  new TH1D("nstub7C1","Number of stubs(stub Window 7) col1;#stubs;Events",20,-0.5,19.5);
  new TH1D("nstub5C0","Number of stubs(stub Window 5) col0;#stubs;Events",20,-0.5,19.5);
  new TH1D("nstub5C1","Number of stubs(stub Window 5) col1;#stubs;Events",20,-0.5,19.5);
  new TH1D("nstub3C0","Number of stubs(stub Window 3) col0;#stubs;Events",20,-0.5,19.5);
  new TH1D("nstub3C1","Number of stubs(stub Window 3) col1;#stubs;Events",20,-0.5,19.5);
  new TH1D("stubEff7C0","StubEfficiency(stub Window 7) col0;#stub found if det0 && det1 has cluster;Events",2,-0.5,1.5);
  new TH1D("stubEff7C1","StubEfficiency(stub Window 7) col1;#stub found if det0 && det1 has cluster;Events",2,-0.5,1.5);
  new TH1D("stubEff5C0","StubEfficiency(stub Window 5) col0;#stub found if det0 && det1 has cluster;Events",2,-0.5,1.5);
  new TH1D("stubEff5C1","StubEfficiency(stub Window 5) col1;#stub found if det0 && det1 has cluster;Events",2,-0.5,1.5);
  new TH1D("stubEff3C0","StubEfficiency(stub Window 3) col0;#stub found if det0 && det1 has cluster;has cluster;Events",2,-0.5,1.5);
  new TH1D("stubEff3C1","StubEfficiency(stub Window 3) col1;#stub found if det0 && det1 has cluster;has cluster;Events",2,-0.5,1.5);
  new TH1D("nstubsFromRaw","Total number of stubs from CBC stub word",20,-.5,19.5);
  new TH1D("nstubsFromReco","Total number of stubs from Reconstruction",20,-.5,19.5);
  new TH1D("nstubsdiff","#StubsReco - #StubsfromStubWord",20,-.5,19.5);

  fout_->mkdir("Correlation");
  fout_->cd("Correlation");
  new TH1D("cor_eventC0","Sensor Hit Correlation col0",4,0.5,4.5);
  new TH1D("cor_eventC1","Sensor Hit Correlation col1",4,0.5,4.5);
  new TH1D("diffC0","#Hits det0 - #Hits det1 col0",100,-.5,99.5);
  new TH1D("diffC1","#Hits det0 - #Hits det1 col1",100,-.5,99.5);
  //new TH1D("match_nos_C0",";dut_0 strip no;Events",255,-0.5,254.5);
  //new TH1D("nomatch_nos",";dut_0 strip no;Events",255,-0.5,254.5);
  fout_->cd();
}

void ReconstructionFromRaw::beginJob() {
  fout_ = TFile::Open(outFile,"RECREATE");
  bookHistogram(fout_);
}
 
void ReconstructionFromRaw::Loop()
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
      // if (Cut(ientry) < 0) continue;
      if (jentry%10000 == 0)
            cout << " Events processed. " << std::setw(8) << jentry 
            << endl;
     
      fout_->cd("det0");
      vector<int> dut0_chtempC0 = (*dut0Ch0data);
      vector<int> dut0_chtempC1 = (*dut0Ch1data);
      //Utility::correctHitorder(dut0_chtemp);
      Utility::fillHist1D("chsizeC0", dut0_chtempC0.size());
      Utility::fillHist1D("chsizeC1", dut0_chtempC1.size());
      Utility::fillHistofromVec(dut0_chtempC0,"hitmapC0");
      Utility::fillHistofromVec(dut0_chtempC1,"hitmapC1");
      Utility::fill2DHistofromVec(dut0_chtempC0,dut0_chtempC1,"hitmapfull");
      getCBCclsuterInfo("det0C0", dut0_chtempC0, detClustermap_);
      getCBCclsuterInfo("det0C1", dut0_chtempC1, detClustermap_);
      fout_->cd("det1");
      vector<int> dut1_chtempC0 = (*dut1Ch0data);
      vector<int> dut1_chtempC1 = (*dut1Ch1data);
      //Utility::correctHitorder(dut0_chtemp);
      Utility::fillHist1D("chsizeC0", dut1_chtempC0.size());
      Utility::fillHist1D("chsizeC1", dut1_chtempC1.size());
      Utility::fillHistofromVec(dut1_chtempC0,"hitmapC0");
      Utility::fillHistofromVec(dut1_chtempC1,"hitmapC1");
      Utility::fill2DHistofromVec(dut1_chtempC0,dut1_chtempC1,"hitmapfull");
      getCBCclsuterInfo("det1C0", dut1_chtempC0, detClustermap_);
      getCBCclsuterInfo("det1C1", dut1_chtempC1, detClustermap_);

      fout_->cd("Correlation");

      if(dut0_chtempC0.size() && !dut1_chtempC0.size()) Utility::fillHist1D("cor_eventC0", 1);
      if(!dut0_chtempC0.size() && dut1_chtempC0.size()) Utility::fillHist1D("cor_eventC0", 2);
      if(dut0_chtempC0.size() && dut1_chtempC0.size()) Utility::fillHist1D("cor_eventC0", 3);
      if(!dut0_chtempC0.size() && !dut1_chtempC0.size()) Utility::fillHist1D("cor_eventC0", 4);

      if(dut0_chtempC1.size() && !dut1_chtempC1.size()) Utility::fillHist1D("cor_eventC1", 1);
      if(!dut0_chtempC1.size() && dut1_chtempC1.size()) Utility::fillHist1D("cor_eventC1", 2);
      if(dut0_chtempC1.size() && dut1_chtempC1.size()) Utility::fillHist1D("cor_eventC1", 3);
      if(!dut0_chtempC1.size() && !dut1_chtempC1.size()) Utility::fillHist1D("cor_eventC1", 4);

      /*
      for(int i=0;i<dut0_chtemp.size();i++){
         int hit_position_0 = dut0_chtemp.at(i);
         bool match=false;
         for(int j=0;j<dut1_chtemp.size();j++){
           int hit_position_1 = dut1_chtemp.at(j);
           if(abs(hit_position_0-hit_position_1)<=7){
             match=true;
             break;
           }  
         }
         if(match==true) Utility::fillHist1D("match_nos",hit_position_0); 
	 else  Utility::fillHist1D("nomatch_nos",hit_position_0); 
      }
      */

      Utility::fillHist1D("diffC0",std::abs(dut0_chtempC0.size()-dut1_chtempC0.size())); 
      Utility::fillHist1D("diffC1",std::abs(dut0_chtempC1.size()-dut1_chtempC1.size())); 
      Utility::getInfofromClusterVec(detClustermap_["det0C0"],"det0",fout_,"C0");
      Utility::getInfofromClusterVec(detClustermap_["det0C1"],"det0",fout_,"C1");
      Utility::getInfofromClusterVec(detClustermap_["det1C0"],"det1",fout_,"C0");
      Utility::getInfofromClusterVec(detClustermap_["det1C1"],"det1",fout_,"C1");
      Utility::getStubInfo(detClustermap_,3,fout_,"C0");
      Utility::getStubInfo(detClustermap_,3,fout_,"C1");
      Utility::getStubInfo(detClustermap_,5,fout_,"C0");
      Utility::getStubInfo(detClustermap_,5,fout_,"C1");
      int totalStubsReco=0;
      totalStubsReco += Utility::getStubInfo(detClustermap_,7,fout_,"C0");
      totalStubsReco += Utility::getStubInfo(detClustermap_,7,fout_,"C1");
      Utility::fillHist1D("nstubsFromRaw",totalStubs);
      Utility::fillHist1D("nstubsFromReco",totalStubsReco);
      Utility::fillHist1D("nstubsdiff",std::abs(totalStubsReco - totalStubs));
      detClustermap_.clear();
   }
}

void ReconstructionFromRaw::endJob() {
  fout_->cd();
  fout_->Write();
  fout_->Close();
  delete fout_;
}
