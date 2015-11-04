#include "TROOT.h"
#include "TInterpreter.h"
#include "TH1.h"
#include <map>
#include <utility>
#include <vector>
#include <sstream>
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
  new TH1I("chsize","dut0 channel occupancy per event",10,-0.5,9.5);
  new TH1D("hitmap","dut0 hitmap;strip no.,No. of Events",255,-0.5,254.5);
  new TH1D("ncluster","#cluster dut0",20,-0.5,19.5);
  new TH1I("clusterWidth","dut0 clusterWidth",10,-0.5,9.5);
  new TH1D("clusterPos","dut0 clusterPos",508,-0.5,254.5);
  fout_->cd();
  
  fout_->mkdir("det1");
  fout_->cd("det1");
  new TH1I("chsize","dut1 channel occupancy per event",10,-0.5,9.5);
  new TH1D("hitmap","dut1 hitmap;strip no.,No. of Events",255,-0.5,254.5);
  new TH1D("ncluster","#cluster dut1",20,-0.5,19.5);
  new TH1I("clusterWidth","dut1 clusterWidth",10,-0.5,9.5);
  new TH1D("clusterPos","dut1 clusterPos",508,-0.5,254.5);

  fout_->mkdir("StubInfo");
  fout_->cd("StubInfo");
  new TH1D("nclusterdiff","Difference in #clusters between dut0 and dut1();#cluster_{det0} - #cluster_  {det1};Events",20,-0.5,19.5);
  new TH1D("nstub7","Number of stubs(stub Window 7);#stubs;Events",20,-0.5,19.5);
  new TH1D("nstub5","Number of stubs(stub Window 5);#stubs;Events",20,-0.5,19.5);
  new TH1D("nstub3","Number of stubs(stub Window 3);#stubs;Events",20,-0.5,19.5);
  new TH1D("stubEff7","StubEfficiency(stub Window 7);#stub found if det0 && det1 has cluster;Events",2,-0.5,1.5);
  new TH1D("stubEff5","StubEfficiency(stub Window 5);#stub found if det0 && det1 has cluster;Events",2,-0.5,1.5);
  new TH1D("stubEff3","StubEfficiency(stub Window 3);#stub found if det0 && det1 has cluster;has cluster;Events",2,-0.5,1.5);

  fout_->mkdir("Cor_plots");
  fout_->cd("Cor_plots");
  new TH1D("cor_event","",4,0.5,4.5);
  new TH1D("diff","",100,-.5,99.5);
  new TH1D("match_nos",";dut_0 strip no;Events",255,-0.5,254.5);
  new TH1D("nomatch_nos",";dut_0 strip no;Events",255,-0.5,254.5);
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

      fout_->cd("det0");
      vector<int> dut0_chtemp = (*dutEven_channel);
      //Utility::correctHitorder(dut0_chtemp);
      Utility::fillHist1D("chsize", dut0_chtemp.size());
      Utility::fillHistofromVec(dut0_chtemp,"hitmap");
      getCBCclsuterInfo("det0", dut0_chtemp, detClustermap_);

      fout_->cd("det1");
      vector<int> dut1_chtemp = (*dutOdd_channel);
      //Utility::correctHitorder(dut1_chtemp);
      Utility::fillHist1D("chsize", dut1_chtemp.size()); 
      Utility::fillHistofromVec(dut1_chtemp,"hitmap");
      getCBCclsuterInfo("det1", dut1_chtemp, detClustermap_);

      fout_->cd("Cor_plots");
      if(dut0_chtemp.size() && !dut1_chtemp.size()) Utility::fillHist1D("cor_event", 1);
      if(!dut0_chtemp.size() && dut1_chtemp.size()) Utility::fillHist1D("cor_event", 2);
      if(dut0_chtemp.size() && dut1_chtemp.size()) Utility::fillHist1D("cor_event", 3);
      if(!dut0_chtemp.size() && !dut1_chtemp.size()) Utility::fillHist1D("cor_event", 4);
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


      Utility::fillHist1D("diff",abs(dut0_chtemp.size()-dut1_chtemp.size())); 
      Utility::getInfofromClusterVec(detClustermap_["det0"],"det0",fout_);
      Utility::getInfofromClusterVec(detClustermap_["det1"],"det1",fout_);
      Utility::getStubInfo(detClustermap_,3,fout_);
      Utility::getStubInfo(detClustermap_,5,fout_);
      Utility::getStubInfo(detClustermap_,7,fout_);
      detClustermap_.clear();
   }
}

void ReconstructionFromRaw::endJob() {
  fout_->cd();
  fout_->Write();
  fout_->Close();
  delete fout_;
}
