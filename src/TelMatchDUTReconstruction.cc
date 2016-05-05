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
#include "TelMatchDUTReconstruction.h"
//#include "telescopeBase.h"
using std::vector;
using std::map;
TelMatchDUTReconstruction::TelMatchDUTReconstruction(const string inFilename,const string telFileName, 
const string outFilename,int stubWindow) :
  BeamAnaBase::BeamAnaBase(true),
  outFile_(outFilename),
  stubWindow_(stubWindow)
{
  std::cout << "EDM Tuple Analyzer initialized with the following options\n" 
            << "Infile: " << inFilename
            << "\nOutFile: " << outFile_
            << "\nStubWindow: " << stubWindow_
            << std::endl; 
  setDUTInputFile(inFilename);
  setTelescopeInputFile(telFileName);
  hist_ = new Histogrammer(outFile_);
  beginJob();

}
void TelMatchDUTReconstruction::bookHistograms() {
  hist_->bookEventHistograms();
  hist_->bookDUTHistograms("det0");
  hist_->bookDUTHistograms("det1");
  hist_->bookStubHistograms();
  hist_->bookCorrelationHistograms();
  //For booking addtional hisograms
  hist_->hfile()->cd(); 
  hist_->hfile()->mkdir("telmatch");
  hist_->hfile()->cd("telmatch");
  new TH1I("stubEventsCBCTelmatch","Events with CBC Stubs",4,-0.5,3.5);
  new TH1I("stubEventsRecoTelmatch","Events with Reco Stubs",4,-0.5,3.5);
  new TH1I("stubEventsRecoTelmatch2C0","Events with Reco Stubs",2,-0.5,1.5);
  new TH1I("stubEventsRecoTelmatch2C1","Events with Reco Stubs",2,-0.5,1.5);
  hist_->hfile()->cd(); 
  hist_->hfile()->mkdir("telescopeProp");
  hist_->hfile()->cd("telescopeProp");
  new TH1D("nTrackParams","",30,0,30);
  new TH1D("xpos","",200,-20.,20.);
  new TH1D("ypos","",200,-20.,20.);

}

void TelMatchDUTReconstruction::beginJob() {
  setAddresses();
  bookHistograms();
}
 
void TelMatchDUTReconstruction::eventLoop()
{
   //if (fChain == 0) return;
   
   //fill the map of telescope events
   filltrigTrackmap();

   Long64_t nentries = getDutEntries();

   Long64_t nbytes = 0, nb = 0;
   cout << "#Events=" << nentries << endl;
   hist_->fillHist1D("EventInfo","nevents", nentries);
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
   //for (Long64_t jentry=0; jentry<20;jentry++) {
     Long64_t ientry = dutchain()->LoadTree(jentry);
     int nbytes = getDUTEntry(ientry);
     //std::cout << "Chain load status=" << ientry << std::endl;
     if (ientry < 0) break;
     if (jentry%1000 == 0)
       cout << " Events processed. " << std::setw(8) << jentry 
	    << endl;
     Int_t duev = dutEvt()->event;
     //std::cout << duev << " # of tracks " << getNtrack(duev) << std::endl;
     if( trigTrkmap()->find(duev) != trigTrkmap()->end() ) {
       hist_->fillHist1D("telescopeProp","nTrackParams", trigTrkmap()->at(duev).nTrackParams);
       //std::cout << trigTrkmap()->at(duev).xPos->size() << "\t"
       //          << trigTrkmap()->at(duev).yPos->size() << std::endl;
       for(auto& x: *(trigTrkmap()->at(duev).xPos))
         hist_->fillHist1D("telescopeProp","xpos", x);
       for(auto& y: *(trigTrkmap()->at(duev).yPos))
         hist_->fillHist1D("telescopeProp","ypos", y);
     }

     //Telescope ad DUT event matching, select events where there are only 
     //1track in telescope
     if(getNtrack (dutEvt()->event) != 1)    continue;
    
     //put additional code here
     //cout << "Point 2" << endl;
     hist_->fillHist1D("EventInfo","hvSettings", dutEvt()->hvSettings);
     hist_->fillHist1D("EventInfo","dutAngle", dutEvt()->dutAngle);
     hist_->fillHist1D("EventInfo","vcth", dutEvt()->vcth);
     hist_->fillHist1D("EventInfo","offset", dutEvt()->offset);
     hist_->fillHist1D("EventInfo","window", dutEvt()->window);
     hist_->fillHist1D("EventInfo","tilt", dutEvt()->tilt);
     hist_->fillHist1D("EventInfo","condData", dutEvt()->condData);
     hist_->fillHist1D("EventInfo","glibstatus", dutEvt()->glibStatus);
     hist_->fillHist1D("EventInfo","cbc1status", dutEvt()->cbc1Status);
     hist_->fillHist1D("EventInfo","cbc2status", dutEvt()->cbc2Status);

      //cout << "Point 2" << endl;
      setDetChannelVectors();
      doClustering();
      findStub(stubWindow_);
      //cout << "Point 3" << endl;
      //Fill histo for det0
      //std::cout << "Hits det0c0=" << dut0Ch0()->size() << std::endl;
      hist_->fillHist1D("det0","chsizeC0", dut0Ch0()->size());
      hist_->fillHist1D("det0","chsizeC1", dut0Ch1()->size());
      hist_->fillHistofromVec(dut0Ch0(),"det0","hitmapC0");
      hist_->fillHistofromVec(dut0Ch1(),"det0","hitmapC1");
      hist_->fill2DHistofromVec(dut0Ch0(),dut0Ch1(),"det0","hitmapfull");
      hist_->fillClusterHistograms("det0",dutClustermap()->at("det0C0"),"C0");
      hist_->fillClusterHistograms("det0",dutClustermap()->at("det0C1"),"C1");

      //Fill histo for det1
      //std::cout << "Hits det1c0=" << dut1Ch0()->size() << std::endl;
      hist_->fillHist1D("det1","chsizeC0", dut1Ch0()->size());
      hist_->fillHist1D("det1","chsizeC1", dut1Ch1()->size());
      hist_->fillHistofromVec(dut1Ch0(),"det1","hitmapC0");
      hist_->fillHistofromVec(dut1Ch1(),"det1","hitmapC1");
      hist_->fill2DHistofromVec(dut1Ch0(),dut1Ch1(),"det1","hitmapfull");
      hist_->fillClusterHistograms("det1",dutClustermap()->at("det1C0"),"C0");
      hist_->fillClusterHistograms("det1",dutClustermap()->at("det1C1"),"C1");

      //cout << "Point 4" << endl;
      if(dut0Ch0()->size() && !dut1Ch0()->size()) hist_->fillHist1D("Correlation","cor_eventC0", 1);
      if(!dut0Ch0()->size() && dut1Ch0()->size()) hist_->fillHist1D("Correlation","cor_eventC0", 2);
      if(dut0Ch0()->size() && dut1Ch0()->size()) hist_->fillHist1D("Correlation","cor_eventC0", 3);
      if(!dut0Ch0()->size() && !dut1Ch0()->size()) hist_->fillHist1D("Correlation","cor_eventC0", 4);
      //cout << "Point 4a" << endl;
      if(dut0Ch0()->size() && !dut1Ch1()->size()) hist_->fillHist1D("Correlation","cor_eventC1", 1);
      if(!dut0Ch0()->size() && dut1Ch1()->size()) hist_->fillHist1D("Correlation","cor_eventC1", 2);
      if(dut0Ch0()->size() && dut1Ch1()->size()) hist_->fillHist1D("Correlation","cor_eventC1", 3);
      if(!dut0Ch0()->size() && !dut1Ch1()->size()) hist_->fillHist1D("Correlation","cor_eventC1", 4);
      //cout << "Point 4b" << endl;
      hist_->fillHist1D("Correlation","diffC0",std::abs(dut0Ch0()->size()-dut1Ch0()->size())); 
      hist_->fillHist1D("Correlation","diffC1",std::abs(dut0Ch1()->size()-dut1Ch1()->size())); 
      //cout << "Point 4c" << endl;
      if (dut0Ch0()->size() ==1 && dut1Ch0()->size() ==1) 
	hist_->fillHist2D("Correlation","topBottomHitCorrC0",dut0Ch0()->at(0), dut1Ch0()->at(0));
      //cout << "Point 4d" << endl;
      if (dut0Ch1()->size() ==1 && dut1Ch1()->size() ==1) 
	hist_->fillHist2D("Correlation","topBottomHitCorrC1",dut0Ch1()->at(0), dut1Ch1()->at(0));
      //cout << "Point 5" << endl;  
      //Fill cbc stub info
      int totStubCBC  = 0;
      for( auto& col : *dutCbcStubmap() ) {
        if( !dutClustermap()->at("det0" + col.first).empty() && 
            !dutClustermap()->at("det1" + col.first).empty() ) {
          if (col.second.size() > 0 ) hist_->fillHist1D("StubInfo","stubEffCBC"+col.first,1);
          else hist_->fillHist1D("StubInfo","stubEffCBC"+col.first,0); 
        }
        totStubCBC += col.second.size();
        hist_->fillHist1D("StubInfo","nstubCBC"+ col.first, col.second.size());
        for( auto& c : col.second ) {
          hist_->fillHist1D("StubInfo","cbcStubBit",c);
          hist_->fillHist1D("StubInfo","stubProfileCBC" + col.first, c);
        }
      }
      hist_->fillHist1D("StubInfo","nstubsFromCBC",totStubCBC);
      //Fill reco stub info
      //cout << "Point 6" << endl;
      int totStubReco = 0;
      for( auto& col : *dutRecoStubmap() ) {
        if( !dutClustermap()->at("det0" + col.first).empty() && 
            !dutClustermap()->at("det1" + col.first).empty() ) {
          if ( col.second.size() > 0 ) hist_->fillHist1D("StubInfo","stubEffReco"+col.first,1);
          else hist_->fillHist1D("StubInfo","stubEffReco"+col.first,0);
        }
        hist_->fillHist1D("StubInfo","nstubReco"+col.first, col.second.size());
        totStubReco += col.second.size();
        for( auto& c : col.second ) {
          hist_->fillHist1D("StubInfo","stubProfileReco"+col.first, c.cbcid);
        }
      }
      hist_->fillHist1D("StubInfo","nstubsFromReco",totStubReco);

      
      hist_->fillHist1D("StubInfo","nclusterdiffC0", dutClustermap()->at("det0C0").size() - 
                                                     dutClustermap()->at("det1C0").size());
      hist_->fillHist1D("StubInfo","nclusterdiffC1", dutClustermap()->at("det0C1").size() - 
                                                     dutClustermap()->at("det1C1").size());
      
      if (totStubReco > 0)  hist_->fillHist1D("StubInfo","stubEventsReco", 1);
      else  hist_->fillHist1D("StubInfo","stubEventsReco",0 );
      if ( totStubCBC > 0) hist_->fillHist1D("StubInfo","stubEventsCBC",1); 
      else  hist_->fillHist1D("StubInfo","stubEventsCBC",0);  
     
      if (!totStubReco && !totStubCBC) hist_->fillHist1D("StubInfo","stubMatch", 1);
      if (!totStubReco && totStubCBC) hist_->fillHist1D("StubInfo","stubMatch", 2);
      if (totStubReco && !totStubCBC) hist_->fillHist1D("StubInfo","stubMatch", 3);
      if (totStubReco && totStubCBC) hist_->fillHist1D("StubInfo","stubMatch", 4);
      Utility::getHist1D("stubMatch")->GetXaxis()->SetBinLabel(1,"!CBC && !RECO");  
      Utility::getHist1D("stubMatch")->GetXaxis()->SetBinLabel(2,"CBC && !RECO");  
      Utility::getHist1D("stubMatch")->GetXaxis()->SetBinLabel(3,"!CBC && RECO");  
      Utility::getHist1D("stubMatch")->GetXaxis()->SetBinLabel(4,"CBC && RECO");  

      hist_->fillHist1D("StubInfo","nstubsdiff",(totStubReco -totStubCBC));

      hist_->fillHist1D("EventInfo","ntotalHitsReco", dut0Ch0()->size() +dut0Ch1()->size() +
                                                      dut1Ch0()->size() + dut1Ch1()->size());
      //cout << "Point 7" << endl;
      clearEvent();
   }
}

void TelMatchDUTReconstruction::clearEvent() {
  BeamAnaBase::clearEvent();
}
void TelMatchDUTReconstruction::endJob() {
  BeamAnaBase::endJob();
  hist_->closeFile();
}

TelMatchDUTReconstruction::~TelMatchDUTReconstruction(){
  delete hist_;
}
