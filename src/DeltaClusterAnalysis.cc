/*!
        \file                DeltaClusterAnalysis.cc
        \brief               Analysis to study the delta position between clusters in 2 sensors
        \author              Jarne De Clercq, Jelena Luetic
        \date                05/07/16
        Support :            mail to :
*/

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
#include "DeltaClusterAnalysis.h"
using std::vector;
using std::map;
DeltaClusterAnalysis::DeltaClusterAnalysis(const std::string inFilename,const std::string outFilename) :
  BeamAnaBase::BeamAnaBase(),
  outFile_(outFilename)
{
  std::cout << "EDM Tuple Analyzer initialized with the following options\n"
            << "Infile: " << inFilename
            << "\nOutFile: " << outFile_
            << std::endl;
  if( setInputFile(inFilename) == 0 ) {
    std::cout << "Empty Chain!!";
    exit(1);
  }
  nEntries_ = analysisTree()->GetEntries();
  hist_ = new Histogrammer(outFile_);
  beginJob();

}
void DeltaClusterAnalysis::bookHistograms() {
  hist_->bookEventHistograms();
  hist_->bookDUTHistograms("det0");
  hist_->bookDUTHistograms("det1");
  hist_->hfile()->mkdir("DeltaCluster");
  hist_->hfile()->cd("DeltaCluster");
  new TH2D("nclsCorrelation","#cluster correlation between det0 and det1;#Cluster_{det0};#Cluster_{det1}", 50, -0.5, 49.5, 50, -0.5, 49.5);
  new TH1D("evtsW1cls","Events with 1 cluster", 2, -0.5, 1.5);
  new TH1I("nclusterdiffC0","Difference in #clusters between dut0 and dut1() for C0;#cluster_{det0} - #cluster_{det1};Events",20,-0.5,19.5);
}

void DeltaClusterAnalysis::beginJob() {
  setAddresses();
  bookHistograms();
  analysisTree()->GetEntry(0);
  getCbcConfig(condEv()->cwd, condEv()->window);
}

void DeltaClusterAnalysis::eventLoop()
{
   //if (fChain == 0) return;

   Long64_t nbytes = 0, nb = 0;
   cout << "#Events=" << nEntries_ << endl;
   hist_->fillHist1D("EventInfo","nevents", nEntries_);

   std::cout << "CBC configuration:: SW=" << stubWindow()
             << "\tCWD=" << cbcClusterWidth()
             << "\tOffset1="<< cbcOffset1()
             << "\tOffset2" << cbcOffset2()
   << std::endl;
   for (Long64_t jentry=0; jentry<nEntries_;jentry++) {
     clearEvent();
     Long64_t ientry = analysisTree()->GetEntry(jentry);
     if (ientry < 0) break;
     if (jentry%1000 == 0) {
       cout << " Events processed. " << std::setw(8) << jentry
	    << endl;
     }
     //cout << "Point 1" << endl;
     if(jentry==0) {
       hist_->fillHist1D("EventInfo","hvSettings", condEv()->HVsettings);
       hist_->fillHist1D("EventInfo","dutAngle", condEv()->DUTangle);
       hist_->fillHist1D("EventInfo","vcth", condEv()->vcth);
       hist_->fillHist1D("EventInfo","offset", cbcOffset1());
       hist_->fillHist1D("EventInfo","offset", cbcOffset2());
       hist_->fillHist1D("EventInfo","window", stubWindow());
       hist_->fillHist1D("EventInfo","tilt", static_cast<unsigned long int>(condEv()->tilt));
     }
     hist_->fillHist1D("EventInfo","isPeriodic",isPeriodic());
     hist_->fillHist1D("EventInfo","isGoodFlag",isGoodEvent());

     if(!isGoodEvent())   continue;
     hist_->fillHist1D("EventInfo","condData", condEv()->condData);
     hist_->fillHist1D("EventInfo","tdcPhase", static_cast<unsigned int>(condEv()->tdcPhase));

      //cout << "Point 2" << endl;
      //All input vectors are set here. You can use the hit,cluster, stub maps only after calling this method
      setDetChannelVectors();

      bool cls1 = dutRecoClmap()->at("det0C0").size() == 1 && dutRecoClmap()->at("det1C0").size() == 1;
      hist_->fillHist2D("DeltaCluster", "nclsCorrelation", dutRecoClmap()->at("det0C0").size(), dutRecoClmap()->at("det1C0").size());
      hist_->fillHist1D("DeltaCluster", "evtsW1cls",cls1);
      hist_->fillHist1D("DeltaCluster","nclusterdiffC0", std::fabs(dutRecoClmap()->at("det0C0").size() - 
                                                         dutRecoClmap()->at("det1C0").size()));

      if(!cls1)  continue;//single cluster on both sensors
      //Follow up all other study for single cluster events here
      const auto& d0c0 = *det0C0();
      const auto& d0c1 = *det0C1();
      const auto& d1c0 = *det1C0();
      const auto& d1c1 = *det1C1();
      //cout << "Point 3" << endl;
      //Fill histo for det0
      hist_->fillHist1D("det0","chsizeC0", d0c0.size());
      //hist_->fillHist1D("det0","chsizeC1", det0C1()->size());
      hist_->fillHistofromVec(d0c0,"det0","hitmapC0");
      //hist_->fillHistofromVec(d0c1,"det0","hitmapC1");
      hist_->fill2DHistofromVec(d0c0,d0c1,"det0","hitmapfull");
      hist_->fillClusterHistograms("det0",dutRecoClmap()->at("det0C0"),"C0");
      //hist_->fillClusterHistograms("det0",dutRecoClmap()->at("det0C1"),"C1");

      //Fill histo for det1
      //std::cout << "Hits det1c0=" << dut1Ch0()->size() << std::endl;
      hist_->fillHist1D("det1","chsizeC0", d1c0.size());
      //hist_->fillHist1D("det1","chsizeC1", d1c1.size());
      hist_->fillHistofromVec(d1c0,"det1","hitmapC0");
      //hist_->fillHistofromVec(d1c1,"det1","hitmapC1");
      hist_->fill2DHistofromVec(d1c0,d1c1,"det1","hitmapfull");
      hist_->fillClusterHistograms("det1",dutRecoClmap()->at("det1C0"),"C0");
      //hist_->fillClusterHistograms("det1",dutRecoClmap()->at("det1C1"),"C1");
   }
}

void DeltaClusterAnalysis::clearEvent() {
  BeamAnaBase::clearEvent();
}
void DeltaClusterAnalysis::endJob() {
  BeamAnaBase::endJob();
  hist_->closeFile();
}

DeltaClusterAnalysis::~DeltaClusterAnalysis(){
  delete hist_;
}
