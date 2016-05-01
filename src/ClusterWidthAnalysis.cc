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
#include "ClusterWidthAnalysis.h"
//#include "telescopeBase.h"
using std::vector;
using std::map;
ClusterWidthAnalysis::ClusterWidthAnalysis(const string inFilename,const string telFileName, 
const string outFilename,int stubWindow, int clustSize, bool doTelmatch) :
  BeamAnaBase::BeamAnaBase(doTelmatch)
{
  std::cout << "EDM Tuple Analyzer initialized with the following options\n" 
            << "Infile: " << inFilename
            << "\nOutFile: " << outFile_
            << "\nStubWindow: " << stubWindow_
            << std::endl; 
  
  outFile_ = outFilename;
  stubWindow_ = stubWindow;
  clswSize_ = clustSize;
  telMatch_ = doTelmatch;
  setDUTInputFile(inFilename);
  setTelescopeInputFile(telFileName);
  hist_ = new Histogrammer(outFile_);
  beginJob();

}
void ClusterWidthAnalysis::bookHistograms() {
  hist_->bookEventHistograms();
  /*
  //book baseline histograms if needed.
  hist_->bookDUTHistograms("det0");
  hist_->bookDUTHistograms("det1");
  hist_->bookStubHistograms();
  hist_->bookCorrelationHistograms();
  */
  //For booking addtional hisograms
  hist_->hfile()->cd(); 
  hist_->hfile()->mkdir("highClsw");
  hist_->hfile()->cd("highClsw");
  //book additional histos here
  //e.g, 
  //new TH1D("ncluster", "#cluster dut0 " + c + ";#Clusters;#Events",51,-0.5,50.5);

}

void ClusterWidthAnalysis::beginJob() {
  setAddresses();
  bookHistograms();
}
 
void ClusterWidthAnalysis::eventLoop()
{
   //if (fChain == 0) return;
   
   //fill the map of telescope events
   filltrigTrackmap();

   Long64_t nentries = getDutEntries();

   Long64_t nbytes = 0, nb = 0;
   cout << "#Events=" << nentries << endl;
   hist_->fillHist1D("EventInfo","nevents", nentries);
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     //for (Long64_t jentry=0; jentry<50;jentry++) {
     Long64_t ientry = dutchain()->LoadTree(jentry);
     int nbytes = getDUTEntry(ientry);
     //std::cout << "Chain load status=" << ientry << std::endl;
     if (ientry < 0) break;
     if (jentry%1000 == 0)
       cout << " Events processed. " << std::setw(8) << jentry 
	    << endl;

     //Telescope ad DUT event matching, select events where there are only 
     //1track in telescope
     if(telMatch_ && getNtrack (dutEvt()->event) != 1)    continue;
    
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
      //use stub finding if needed
      //findStub(stubWindow_);
      //put your code for high cluster width here
      //e.g,dutClustermap() has 4 keys, det0C0,det0C1,det1C0,det0C1
      for(auto& c : dutClustermap()->at("det0C0") ) {
        if(c.width > clswSize_) std::cout << "High cluster found" << std::endl;
      }
      

      clearEvent();
   }
}

void ClusterWidthAnalysis::clearEvent() {
  BeamAnaBase::clearEvent();
}
void ClusterWidthAnalysis::endJob() {
  BeamAnaBase::endJob();
  hist_->closeFile();
}

ClusterWidthAnalysis::~ClusterWidthAnalysis(){
  delete hist_;
}
