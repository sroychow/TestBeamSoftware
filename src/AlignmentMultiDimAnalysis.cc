/*!
 *         \file                AlignmentAnalysis.cc
 *         \brief               Alignment analysis, written starting from BaselineAnalysis
 *         \author              Nicolas Chanon
 *         \date                20/07/16
 *         Support :            mail to : nicolas.pierre.chanon@cern.ch
 * */

//#include "TROOT.h"
#include "TInterpreter.h"
#include "TF1.h"
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
#include <iostream>
#include <fstream>

#include "Math/Functor.h"
#include "Minuit2/Minuit2Minimizer.h"


#include "AlignmentMultiDimAnalysis.h"

using std::vector;
using std::map;
using namespace std;

AlignmentMultiDimAnalysis::AlignmentMultiDimAnalysis() :
  BeamAnaBase::BeamAnaBase()
{
  alignparFile_ = workDir() + "/data/alignmentParameters.txt";

}

void AlignmentMultiDimAnalysis::beginJob() {
  BeamAnaBase::beginJob();
  hist_ = outFile();
  setAddresses();
  nEntries_ = analysisTree()->GetEntries();

  zMin = 300.;

  zStep = 20.;
  zNsteps = 50;

  bookHistograms();
  //analysisTree()->GetEntry(0);
  //getCbcConfig(condEv()->cwd, condEv()->window);

  //if(jobCardmap().find("isProductionmode") != jobCardmap().end())
  //  isProduction_ = (atoi(jobCardmap().at("isProductionmode").c_str()) > 0) ? true : false;
  if(jobCardmap().find("alignmentOutputFile") != jobCardmap().end())
    alignparFile_ = workDir() + "/data/" + jobCardmap().at("alignmentOutputFile");
  if(jobCardmap().find("Run") != jobCardmap().end())
    runNumber_ = jobCardmap().at("Run");

  std::cout << "Additional Parameter specific to AlignmentReco>>"
            //<< "\nisProductionMode:" << isProduction_
            << "\nalignparameterOutputFile:" << alignparFile_
            << std::endl;
  //If Production mode, read the existing alignment json into a json array.
  //Later, the alignment from this code will be added to this json array
  alignmentDump_ = json::array();
  //if(isProduction_) {
  //  std::ifstream fileAlignment(alignparFile_.c_str());
   // fileAlignment >> alignmentDump_;
  //  fileAlignment.close();
 //}

  dnamebottom = (*modVec())[0].hdirbottom_ + "/TrackFit";
  dnametop = (*modVec())[0].hdirtop_ + "/TrackFit";

  refPlaneZ = telPlaneprev_.z;
  //refPlaneZ = (*modVec())[0].z;
  cout << "refPlaneZ="<<refPlaneZ<<endl;

}

void AlignmentMultiDimAnalysis::bookHistograms() {
  BeamAnaBase::bookHistograms();

  for(auto& m : *modVec()){
    TString s = TString(m.hdirbottom_);
    bookTrackFitHistograms(s,zMin, zStep, zNsteps);
    s = TString(m.hdirtop_);
    bookTrackFitHistograms(s,zMin, zStep, zNsteps);
  }
}

void AlignmentMultiDimAnalysis::dumpAlignment(const double* a) {
  json o;
  o.emplace("offset_d0", a[0]);
  o.emplace("zDUT_d0"  , a[1]);
  o.emplace("deltaZ"   , a[2]);
  //o.emplace("theta"    , a[3]*180./TMath::Pi());
  o.emplace("theta"    , a[3]);
  o.emplace("shiftPlanes", a[4]);
  //o.emplace("phi_d0"   , a[5]*180./TMath::Pi());
  //o.emplace("phi_d1"   , a[6]*180./TMath::Pi());
  o.emplace("phi_d0"   , a[5]);
  o.emplace("phi_d1"   , a[6]);


  std::cout << "Alignment File=" << alignparFile_ << std::endl;
  std::cout << std::setw(4) << o << std::endl;
  std::ofstream fileAlignment(alignparFile_.c_str());
  fileAlignment << std::setw(4) << o << std::endl;
  fileAlignment.close();

/*
offset_d0=resultBothPlanesConstraintShiftPhi[0]
zDUT_d0= resultBothPlanesConstraintShiftPhi[1]
deltaZ= resultBothPlanesConstraintShiftPhi[2]
theta =resultBothPlanesConstraintShiftPhi[3]*180./TMath::Pi()
shiftPlanes=resultBothPlanesConstraintShiftPhi[4]
phi_d0  = resultBothPlanesConstraintShiftPhi[5]*180./TMath::Pi()
phi_d1= resultBothPlanesConstraintShiftPhi[6]*180./TMath::Pi() 
*/

}

void AlignmentMultiDimAnalysis::eventLoop()
{

  selectedTk_d0_1Hit.clear();
  d0_DutXpos.clear();
  selectedTk_d1_1Hit.clear();
  d1_DutXpos.clear();

  selectedTk_bothPlanes_1Cls.clear();
  bothPlanes_DutXposD0.clear();
  bothPlanes_DutXposD1.clear();

  toMinimize = new ROOT::Math::Functor(this, &AlignmentMultiDimAnalysis::ComputeChi2, 3);
  minimizer = new ROOT::Minuit2::Minuit2Minimizer( ROOT::Minuit2::kMigrad );
  minimizer->SetPrintLevel(0);
  minimizer->SetFunction(*toMinimize);

  toMinimizeBothPlanes = new ROOT::Math::Functor(this, &AlignmentMultiDimAnalysis::ComputeChi2BothPlanes, 5);
  minimizerBothPlanes = new ROOT::Minuit2::Minuit2Minimizer( ROOT::Minuit2::kMigrad );
  minimizerBothPlanes->SetPrintLevel(0);
  minimizerBothPlanes->SetFunction(*toMinimizeBothPlanes);

  toMinimizeBothPlanesConstraint = new ROOT::Math::Functor(this, &AlignmentMultiDimAnalysis::ComputeChi2BothPlanes, 4);
  minimizerBothPlanesConstraint = new ROOT::Minuit2::Minuit2Minimizer( ROOT::Minuit2::kMigrad );
  minimizerBothPlanesConstraint->SetPrintLevel(0);
  minimizerBothPlanesConstraint->SetFunction(*toMinimizeBothPlanesConstraint);

  toMinimizeBothPlanesConstraintShift = new ROOT::Math::Functor(this, &AlignmentMultiDimAnalysis::ComputeChi2BothPlanes, 5);
  minimizerBothPlanesConstraintShift = new ROOT::Minuit2::Minuit2Minimizer( ROOT::Minuit2::kMigrad ); //kMigrad
  minimizerBothPlanesConstraintShift->SetPrintLevel(0);
  minimizerBothPlanesConstraintShift->SetFunction(*toMinimizeBothPlanesConstraintShift);

  toMinimizeBothPlanesConstraintShiftPhi = new ROOT::Math::Functor(this, &AlignmentMultiDimAnalysis::ComputeChi2BothPlanes, 7);
  minimizerBothPlanesConstraintShiftPhi = new ROOT::Minuit2::Minuit2Minimizer( ROOT::Minuit2::kMigrad ); //kMigrad
  minimizerBothPlanesConstraintShiftPhi->SetPrintLevel(0);
  minimizerBothPlanesConstraintShiftPhi->SetFunction(*toMinimizeBothPlanesConstraintShiftPhi);


  if (!doTelMatching()) return;

  //float DUT_z = 460.0;//oct16=460;//may16=435
  //float DUT_z_try = 400;

  //std::string filename=inFile();
  //run_ = std::stoi(filename.substr(filename.find(".root")-3,3));
  //cout<<"Run Number : "<<run_<<endl;
  maxEvent_ = getMaxEvt();
  if(nEntries_<maxEvent_ || maxEvent_==0) maxEvent_=nEntries_;
  Long64_t nbytes = 0, nb = 0;
  cout << "#Events=" << nEntries_ <<" -->  MAX EVENTS TO BE PROCESSED : "<<maxEvent_<<endl;
  hist_->fillHist1D("EventInfo","nevents", nEntries_);

  /*
  for (Long64_t jentry=0; jentry < maxEvent_; jentry++) {
    clearEvent();
    Long64_t ientry = analysisTree()->GetEntry(jentry);
    if (ientry < 0) break;
    if (jentry%1000 == 0) {
      cout << " Events processed. " << std::setw(8) << jentry
	   << endl;
    }
    if(jentry==0) {
      hist_->fillHist1D("EventInfo","hvSettings", event()->HVsettings);
      hist_->fillHist1D("EventInfo","dutAngle", event()->DUTangle);
      //hist_->fillHist1D("EventInfo","vcth", event()->vcth);
      //hist_->fillHist1D("EventInfo","offset", cbcOffset1());
      //hist_->fillHist1D("EventInfo","offset", cbcOffset2());
      //hist_->fillHist1D("EventInfo","window", stubWindow());
      //hist_->fillHist1D("EventInfo","tilt", static_cast<unsigned long int>(condEv()->tilt));
    }
    hist_->fillHist1D("EventInfo","condData", static_cast<unsigned int>(event()->condData));
    hist_->fillHist1D("EventInfo","tdcPhase", static_cast<unsigned int>(event()->tdcPhase));
    //et the hits/cluster/stubs from the sensors in user accessable module*******
    //CANNOT REMOVE THIS LINE
    setDetChannelVectors();

    //fill common histograms for dut hits, clusters//Optional
    fillCommonHistograms();
    //Fill track match histograms
    if(event()->tracks.size() != 1)  continue;
    for(auto& m : *modVec()){
      //std::string dnamebottom = m.hdirbottom_ + "/TrackFit";
      //loop over tracks
      for(auto& tk: event()->tracks) {
        if(m.bottomHits.size() != 1) continue;
        //previous hit
        float xtk_prev = tk.xPosPrevHit() + std::abs(m.z - telPlaneprev_.z)*tk.dxdz();
        float ytk_prev = tk.yPosPrevHit() + std::abs(m.z - telPlaneprev_.z)*tk.dydz();
        //std::cout << "Previous hit tk extrapolated Xpos=" << xtk_prev/1000. << "\n";
        //std::cout << "Previous hit tk extrapolated Ypos=" << ytk_prev/1000. << "\n";
        hist_->fillHist1D(dnamebottom, "tkposx_prev", xtk_prev/1000.);
        hist_->fillHist1D(dnamebottom, "tkposy_prev", ytk_prev/1000.);
        //next hit
        float xtk_next = tk.xPosNextHit() + std::abs(m.z - telPlaneprev_.z)*tk.dxdz();
        float ytk_next = tk.yPosNextHit() + std::abs(m.z - telPlaneprev_.z)*tk.dydz();
        hist_->fillHist1D(dnamebottom, "tkposx_next", xtk_next/1000.);
        hist_->fillHist1D(dnamebottom, "tkposy_next", ytk_next/1000.);
        //Fill hit residuals
        for(auto& h: m.bottomHits) {
          float hx = (float(h.strip()) - float(m.nstrips_)/2.)*m.pitch_;
          hist_->fillHist1D(dnamebottom, "hitresidualX_prev", hx - xtk_prev/1000.);
          hist_->fillHist1D(dnamebottom, "hitresidualX_next", hx - xtk_next/1000.);
        }
        //std::cout << "NStrips = " << float(m.nstrips_)/2. << "\tPitch=" << m.pitch_ << "\n";
        //Fill cluster residuals
        for(auto& c: m.bottomOfflineCls) {

          float cx = (c.center() - m.nstrips_/2.)*m.pitch_;
          //std::cout << "Offline clus pos(bottom) =" << c.center() << " in mm=" << (c.center() - 127)*0.09<< "\n";
          //std::cout << "Cluster Xpos(in align)=" << cx << "\tresidual=" << cx - xtk_prev/1000. <<"\n";
          hist_->fillHist1D(dnamebottom, "clusresidualX_prev", cx - xtk_prev/1000.);
          hist_->fillHist1D(dnamebottom, "clusresidualX_next", cx - xtk_next/1000.);
        }
      }
    }
    
  }
  */

  //First Loop : find initial values for the alignment

    //std::string dnamebottom = (*modVec())[0].hdirbottom_ + "/TrackFit";
    //std::string dnametop = (*modVec())[0].hdirtop_ + "/TrackFit";
    int nStrips = (*modVec())[0].nstrips_;
    double DUT_z = (*modVec())[0].z;
    //refPlaneZ = telPlaneprev_.z;

  //BEGIN NOV17 ALIGNMENT
  for (Long64_t jentry=0; jentry<nEntries_ && jentry<maxEvent_;jentry++) {
    clearEvent();
    Long64_t ientry = analysisTree()->GetEntry(jentry);
    if (ientry < 0) break;
    if (jentry%1000 == 0) {
       cout << " Events processed. " << std::setw(8) << jentry
	    << endl;
    }

    setDetChannelVectors();
    const auto& d0c0 = (*modVec())[0].bottomHits;
    const auto& d1c0 = (*modVec())[0].topHits;
    const auto& d0Cls = (*modVec())[0].bottomOfflineCls;
    const auto& d1Cls = (*modVec())[0].topOfflineCls;

    //Match with FEI4
    std::vector<tbeam::OfflineTrack>  selectedTk = event()->tracks;
    //hist_->fillHist1D("TelescopeAnalysis","nTrack_fiducial", selectedTk.size());

    //Find mean of residuals, scanning zDUT
    if (selectedTk.size()!=1) continue;
    //std::cout << "track chi2="<<selectedTk[0].chi2()<<endl;
    if (selectedTk[0].chi2()>5.) continue;

    //float xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk[0], refPlaneZ, 0, DUT_z, 0);
    float xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk[0], refPlaneZ, 0, -556212., 0.);
    //float xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk[0], refPlaneZ, 0, -556212., TMath::Pi());
    //cout << "NHits: d0, "<<d0c0.size()<<" ; d1, "<<d1c0.size()<<endl;
    if (d0c0.size()==1) {
      for(auto& h: d0c0) {
        float xDUT = (h.strip() - nStrips/2) * dutpitch();
	xDUT = -xDUT;
        selectedTk_d0_1Hit.push_back(selectedTk[0]);
        d0_DutXpos.push_back(xDUT);
        hist_->fillHist1D(dnamebottom, "d0_1tk1Hit_diffX_start", xDUT-xTkAtDUT/1000.);
      }
    }
    if (d1c0.size()==1){
      for(auto& h: d1c0) {
        float xDUT = (h.strip() - nStrips/2) * dutpitch();
	xDUT = -xDUT;
        selectedTk_d1_1Hit.push_back(selectedTk[0]);
        d1_DutXpos.push_back(xDUT);
        hist_->fillHist1D(dnametop,"d1_1tk1Hit_diffX_start", xDUT-xTkAtDUT/1000.);
      }
    }
    if (d0Cls.size()==1 && d1Cls.size()==1){
      bool skipEvent = false;
      double D0xDUT;
      double D1xDUT; 
      for(auto& cl : d0Cls ) {
	if (cl.size()!=1) skipEvent=true;
        D0xDUT = (cl.center()-nStrips/2)*dutpitch();
	D0xDUT = -D0xDUT;
      }
      for(auto& cl : d1Cls ) {
        if (cl.size()!=1) skipEvent=true;
        D1xDUT = (cl.center()-nStrips/2)*dutpitch();
	D1xDUT = -D1xDUT;
      }
      if (skipEvent) continue; 
      selectedTk_bothPlanes_1Cls.push_back(selectedTk[0]);
      bothPlanes_DutXposD0.push_back(D0xDUT);
      bothPlanes_DutXposD1.push_back(D1xDUT);
      hist_->fillHist1D(dnamebottom,"d0_1tk1Cls_diffX_start", D0xDUT-xTkAtDUT/1000.);
      hist_->fillHist1D(dnametop,"d1_1tk1Cls_diffX_start", D1xDUT-xTkAtDUT/1000.);
    }
   
  }//End of First Loop


  TH1* hTmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom, "d0_1tk1Hit_diffX_start"));
  TF1* fGausExtractedX = new TF1("fGausExtractedX", Utility::FuncPol1Gaus, -100., 100., 5);
  double offset_init_d0 = ((float)hTmp->GetMaximumBin())*(hTmp->GetXaxis()->GetXmax()-hTmp->GetXaxis()->GetXmin())/((float)hTmp->GetNbinsX()) + hTmp->GetXaxis()->GetXmin();//hTmp->GetMean();
  hTmp->SetAxisRange(offset_init_d0-1., offset_init_d0+1., "X");
  double cte = (hTmp->GetBinContent(hTmp->FindBin(offset_init_d0-1.))+hTmp->GetBinContent(hTmp->FindBin(offset_init_d0+1.)))/2.;

  fGausExtractedX->SetParameter(0, hTmp->GetMaximum());
  //  fGausExtractedX->SetParLimits(1, -50., 50.);
  fGausExtractedX->SetParameter(1, offset_init_d0);
  //fGausExtractedX->SetParLimits(2, 0, 0.3);
  fGausExtractedX->SetParameter(2, 0.03);
  //fGausExtractedX->SetParLimits(3, 0, 1000.);
  fGausExtractedX->SetParameter(3, cte);
  fGausExtractedX->SetParameter(4, 0);
  hTmp->Fit("fGausExtractedX", "WW");
  offset_init_d0 = fGausExtractedX->GetParameter(1);


  hTmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom, "d0_1tk1Cls_diffX_start"));
  double offset_init_ClsD0 = ((float)hTmp->GetMaximumBin())*(hTmp->GetXaxis()->GetXmax()-hTmp->GetXaxis()->GetXmin())/((float)hTmp->GetNbinsX()) + hTmp->GetXaxis()->GetXmin();//hTmp->GetMean();
  hTmp->SetAxisRange(offset_init_ClsD0-1., offset_init_ClsD0+1., "X");
  cte = (hTmp->GetBinContent(hTmp->FindBin(offset_init_ClsD0-1.))+hTmp->GetBinContent(hTmp->FindBin(offset_init_ClsD0+1.)))/2.;

  fGausExtractedX->SetParameter(0, hTmp->GetMaximum());
  fGausExtractedX->SetParameter(1, offset_init_ClsD0);
  fGausExtractedX->SetParameter(2, 0.03);
  fGausExtractedX->SetParameter(3, cte);
  fGausExtractedX->SetParameter(4, 0);
  hTmp->Fit("fGausExtractedX", "WW");
  offset_init_ClsD0 = fGausExtractedX->GetParameter(1);


  hTmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop, "d1_1tk1Hit_diffX_start"));
  double offset_init_d1 = ((float)hTmp->GetMaximumBin())*(hTmp->GetXaxis()->GetXmax()-hTmp->GetXaxis()->GetXmin())/((float)hTmp->GetNbinsX()) + hTmp->GetXaxis()->GetXmin();//hTmp->GetMean();
  hTmp->SetAxisRange(offset_init_d1-1., offset_init_d1+1., "X");
  cte = (hTmp->GetBinContent(hTmp->FindBin(offset_init_d1-1.))+hTmp->GetBinContent(hTmp->FindBin(offset_init_d1+1.)))/2.;

  fGausExtractedX->SetParameter(0, hTmp->GetMaximum());
  fGausExtractedX->SetParameter(1, offset_init_d1);
  fGausExtractedX->SetParameter(2, 0.03);
  fGausExtractedX->SetParameter(3, cte);
  fGausExtractedX->SetParameter(4, 0);
  hTmp->Fit("fGausExtractedX", "WW");
  offset_init_d1 = fGausExtractedX->GetParameter(1);
/*
  hTmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop, "d0_1tk1Cls_diffX_start"));
  double offset_init_ClsD0 = ((float)hTmp->GetMaximumBin())*(hTmp->GetXaxis()->GetXmax()-hTmp->GetXaxis()->GetXmin())/((float)hTmp->GetNbinsX()) + hTmp->GetXaxis()->GetXmin();//hTmp->GetMean();
  hTmp->SetAxisRange(offset_init_ClsD0-1., offset_init_ClsD0+1., "X");
  cte = (hTmp->GetBinContent(hTmp->FindBin(offset_init_ClsD0-1.))+hTmp->GetBinContent(hTmp->FindBin(offset_init_ClsD0+1.)))/2.;

  fGausExtractedX->SetParameter(0, hTmp->GetMaximum());
  fGausExtractedX->SetParameter(1, offset_init_ClsD0);
  fGausExtractedX->SetParameter(2, 0.03);
  fGausExtractedX->SetParameter(3, cte);
  fGausExtractedX->SetParameter(4, 0);
  hTmp->Fit("fGausExtractedX", "WW");
  offset_init_ClsD0 = fGausExtractedX->GetParameter(1);
*/
  hTmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop, "d1_1tk1Cls_diffX_start"));
  double offset_init_ClsD1 = ((float)hTmp->GetMaximumBin())*(hTmp->GetXaxis()->GetXmax()-hTmp->GetXaxis()->GetXmin())/((float)hTmp->GetNbinsX()) + hTmp->GetXaxis()->GetXmin();//hTmp->GetMean();
  hTmp->SetAxisRange(offset_init_ClsD1-1., offset_init_ClsD1+1., "X");
  cte = (hTmp->GetBinContent(hTmp->FindBin(offset_init_ClsD1-1.))+hTmp->GetBinContent(hTmp->FindBin(offset_init_ClsD1+1.)))/2.;

  fGausExtractedX->SetParameter(0, hTmp->GetMaximum());
  fGausExtractedX->SetParameter(1, offset_init_ClsD1);
  fGausExtractedX->SetParameter(2, 0.03);
  fGausExtractedX->SetParameter(3, cte);
  fGausExtractedX->SetParameter(4, 0);
  hTmp->Fit("fGausExtractedX", "WW");
  offset_init_ClsD1 = fGausExtractedX->GetParameter(1);


  cout << "offset_init_d0="<<offset_init_d0<<endl;
  cout << "offset_init_d1="<<offset_init_d1<<endl;
  cout << "offset_init_ClsD0=" << offset_init_ClsD0 <<endl;
  cout << "offset_init_ClsD1="<<offset_init_ClsD1<<endl;

/*
  doD0 = true;
  doD1 = false;
  double chi2 = 0;

//#if defined(MAY_16) || defined(OCT_16)
//  minimizer->SetLimitedVariable(0, "offset", offset_init_d0, 0.0001, -6., 6.);
//  minimizer->SetLimitedVariable(1, "zDUT", 435., 0.01, 200., 800.);
//#elif NOV_15
//  minimizer->SetLimitedVariable(0, "offset", offset_init_d0, 0.0001, -100., 100.);
//  minimizer->SetLimitedVariable(1, "zDUT", 1350., 0.01, 1200., 1600.);
//#endif
  minimizer->SetLimitedVariable(0, "offset", offset_init_d0, 0.0001, -25., -10.);
  minimizer->SetLimitedVariable(1, "zDUT", DUT_z, 1., telPlaneprev_.z, telPlanenext_.z);
  minimizer->SetLimitedVariable(2, "theta", 0.*TMath::Pi()/180., 0.01, -20.*TMath::Pi()/180., 20.*TMath::Pi()/180.);

  cout << "DUT d0: Start chi2 minimization"<<endl;
  minimizer->Minimize();
  const double *resultMinimizerD0 = minimizer->X();
  double *resultD0 = new double[3];
  resultD0[0] = resultMinimizerD0[0];
  resultD0[1] = resultMinimizerD0[1];
  resultD0[2] = resultMinimizerD0[2];
  double chi2D0 = ComputeChi2(resultD0);
  for (unsigned int i=0; i<selectedTk_d0_1Hit.size(); i++){
    double xDUT_d0 =   d0_DutXpos.at(i);
    double xTkAtDUT_d0 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d0_1Hit.at(i), refPlaneZ, resultD0[0], resultD0[1], resultD0[2]);
    double resDUT_d0 = xDUT_d0 - xTkAtDUT_d0;
    hist_->fillHist1D(dnamebottom,"d0_1tk1Hit_diffX_aligned", resDUT_d0);
  }


  doD0 = false;
  doD1 = true;
  minimizer->Clear();
//#if defined(MAY_16) || defined(OCT_16)
//  minimizer->SetLimitedVariable(0, "offset", offset_init_d1, 0.0001, -6., 6.);
//  minimizer->SetLimitedVariable(1, "zDUT", 435., 0.01, 200., 800.);
//#elif NOV_15
//  minimizer->SetLimitedVariable(0, "offset", offset_init_d1, 0.0001, -100., 100.);
//  minimizer->SetLimitedVariable(1, "zDUT", 1350., 0.01, 1200., 1600.);
//#endif
  minimizer->SetLimitedVariable(0, "offset", offset_init_d1, 0.0001, -30., 0.);
  minimizer->SetLimitedVariable(1, "zDUT", DUT_z, 1., telPlaneprev_.z, telPlanenext_.z);
  minimizer->SetLimitedVariable(2, "theta", 0.*TMath::Pi()/180., 0.01, -20.*TMath::Pi()/180., 20.*TMath::Pi()/180.);

  cout << "DUT d1: Start chi2 minimization"<<endl;
  minimizer->Minimize();
  const double *resultMinimizerD1 = minimizer->X();
  double* resultD1 = new double[3];
  resultD1[0] = resultMinimizerD1[0];
  resultD1[1] = resultMinimizerD1[1];
  resultD1[2] = resultMinimizerD1[2];
  double chi2D1 = ComputeChi2(resultD1);
  for (unsigned int i=0; i<selectedTk_d1_1Hit.size(); i++){
    double xDUT_d1 =   d1_DutXpos.at(i);
    double xTkAtDUT_d1 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d1_1Hit.at(i), refPlaneZ, resultD1[0], resultD1[1], resultD1[2]);
    double resDUT_d1 = xDUT_d1 - xTkAtDUT_d1;
    hist_->fillHist1D(dnametop,"d1_1tk1Hit_diffX_aligned", resDUT_d1);
  }

 cout << "D0 offset="<< resultD0[0]<<" zDUT="<<resultD0[1]<<" theta="<<resultD0[2]*180./TMath::Pi()<<" chi2="<<chi2D0<<endl;
 cout << "D1 offset="<< resultD1[0]<<" zDUT="<<resultD1[1]<<" theta="<<resultD1[2]*180./TMath::Pi()<<" chi2="<<chi2D1<<endl;


  doConstrainDeltaOffset = false;
  doD0 = true;
  doD1 = true;
  minimizerBothPlanes->Clear();
//#if defined(MAY_16) || defined(OCT_16)
//  minimizerBothPlanes->SetLimitedVariable(0, "offset_d0", offset_init_d0, 0.0001, -6., 6.);
//  minimizerBothPlanes->SetLimitedVariable(1, "zDUT_d0", 435., 0.01, 200., 800.);
//  minimizerBothPlanes->SetLimitedVariable(2, "offset_d1", offset_init_d1, 0.0001, -6., 6.);
//  minimizerBothPlanes->SetLimitedVariable(3, "zDUT_d1", 435., 0.01, 200., 800.);
//#elif NOV_15
//  minimizerBothPlanes->SetLimitedVariable(0, "offset_d0", offset_init_d0, 0.0001, -100., 100.);
//  minimizerBothPlanes->SetLimitedVariable(1, "zDUT_d0", 1400., 0.01, 1200., 1600.);
//  minimizerBothPlanes->SetLimitedVariable(2, "offset_d1", offset_init_d1, 0.0001, -100., 100.);
//  minimizerBothPlanes->SetLimitedVariable(3, "zDUT_d1", 1400., 0.01, 1200., 1600.);
//#endif

  minimizerBothPlanes->SetLimitedVariable(0, "offset_d0", offset_init_d0, 0.0001, -30., 0.);
  minimizerBothPlanes->SetLimitedVariable(1, "zDUT_d0", DUT_z, 1., telPlaneprev_.z, telPlanenext_.z);
  minimizerBothPlanes->SetLimitedVariable(2, "offset_d1", offset_init_d1, 0.0001, -30., 0.);
  minimizerBothPlanes->SetLimitedVariable(3, "zDUT_d1", DUT_z, 1., telPlaneprev_.z, telPlanenext_.z);
  minimizerBothPlanes->SetLimitedVariable(4, "theta", 0., 0.01, -20.*TMath::Pi()/180., 20.*TMath::Pi()/180.);

  cout << "DUT both planes: Start chi2 minimization"<<endl;
  minimizerBothPlanes->Minimize();
  const double *resultMinimizerBothPlanes = minimizerBothPlanes->X();
  double* resultBothPlanes = new double[5];
  resultBothPlanes[0] = resultMinimizerBothPlanes[0];
  resultBothPlanes[1] = resultMinimizerBothPlanes[1];
  resultBothPlanes[2] = resultMinimizerBothPlanes[2];
  resultBothPlanes[3] = resultMinimizerBothPlanes[3];
  resultBothPlanes[4] = resultMinimizerBothPlanes[4];
  double chi2BothPlanes = ComputeChi2BothPlanes(resultBothPlanes);
  cout << "BothPlanes offset_d0="<< resultBothPlanes[0]<<" zDUT_d0="<<resultBothPlanes[1]<<" offset_d1="<< resultBothPlanes[2]<<" zDUT_d1="<<resultBothPlanes[3] << " theta="<<resultBothPlanes[4]*180./TMath::Pi()<< " chi2="<<chi2BothPlanes<<endl;

  for (unsigned int i=0; i<selectedTk_bothPlanes_1Cls.size(); i++){
    double xDUT_d0 =   bothPlanes_DutXposD0.at(i);
    double xTkAtDUT_d0 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, resultBothPlanes[0], resultBothPlanes[1], resultBothPlanes[4]);
    double resDUT_d0 = xDUT_d0 - xTkAtDUT_d0;
    hist_->fillHist1D(dnamebottom,"d0_1tk1ClusterBothPlanes_diffX_aligned", resDUT_d0);

    double xDUT_d1 =   bothPlanes_DutXposD1.at(i);
    double xTkAtDUT_d1 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, resultBothPlanes[2], resultBothPlanes[3], resultBothPlanes[4]);
    double resDUT_d1 = xDUT_d1 - xTkAtDUT_d1;
    hist_->fillHist1D(dnametop,"d1_1tk1ClusterBothPlanes_diffX_aligned", resDUT_d1);
  }


  doConstrainDeltaOffset = true;
  doAllowOffsetPlanes = false;
  doD0 = true;
  doD1 = true;
  minimizerBothPlanesConstraint->Clear();

  minimizerBothPlanesConstraint->SetLimitedVariable(0, "offset_d0", offset_init_d0, 0.0001, -30., 0.);
  minimizerBothPlanesConstraint->SetLimitedVariable(1, "zDUT_d0", DUT_z, 10., telPlaneprev_.z, telPlanenext_.z);
  minimizerBothPlanesConstraint->SetLimitedVariable(2, "deltaZ", 1800., 1., 0., 8000.);
  minimizerBothPlanesConstraint->SetLimitedVariable(3, "theta", TMath::ATan((offset_init_d1-offset_init_d0)/2.), 0.001, -20.*TMath::Pi()/180., 20.*TMath::Pi()/180.);

  cout << "DUT both planes with deltaOffset constraint: Start chi2 minimization"<<endl;
  minimizerBothPlanesConstraint->Minimize();
  const double *resultMinimizerBothPlanesConstraint = minimizerBothPlanesConstraint->X();
  double* resultBothPlanesConstraint = new double[4];
  resultBothPlanesConstraint[0] = resultMinimizerBothPlanesConstraint[0];
  resultBothPlanesConstraint[1] = resultMinimizerBothPlanesConstraint[1];
  resultBothPlanesConstraint[2] = resultMinimizerBothPlanesConstraint[2];
  resultBothPlanesConstraint[3] = resultMinimizerBothPlanesConstraint[3];
  double chi2BothPlanesConstraint = ComputeChi2BothPlanes(resultBothPlanesConstraint);
  cout << "BothPlanesConstraint offset_d0="<< resultBothPlanesConstraint[0]<<" zDUT_d0="<<resultBothPlanesConstraint[1]<<" deltaZ="<< resultBothPlanesConstraint[2]<<" theta="<<resultBothPlanesConstraint[3]*180./TMath::Pi()<< " chi2="<<chi2BothPlanesConstraint<<endl;

  for (unsigned int i=0; i<selectedTk_bothPlanes_1Cls.size(); i++){
    double xDUT_d0 =   bothPlanes_DutXposD0.at(i);
    double xDUT_d1 =   bothPlanes_DutXposD1.at(i);
    std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, resultBothPlanesConstraint[0], resultBothPlanesConstraint[1], resultBothPlanesConstraint[2], resultBothPlanesConstraint[3]);
    double xTkAtDUT_d0 = xTkAtDUT.first;
    double xTkAtDUT_d1 = xTkAtDUT.second;
    double resDUT_d0 = xDUT_d0 - xTkAtDUT_d0;
    double resDUT_d1 = xDUT_d1 - xTkAtDUT_d1;
    hist_->fillHist1D(dnamebottom,"d0_1tk1ClusterBothPlanesConstraint_diffX_aligned", resDUT_d0);
    hist_->fillHist1D(dnametop,"d1_1tk1ClusterBothPlanesConstraint_diffX_aligned", resDUT_d1);
  }
*/
/*
  doConstrainDeltaOffset = true;
  doAllowOffsetPlanes = true;
  doD0 = true;
  doD1 = true;
  minimizerBothPlanesConstraintShift->Clear();

  minimizerBothPlanesConstraintShift->SetLimitedVariable(0, "offset_d0", offset_init_ClsD0*1000, 0.1, -30000., 30000.);
  //minimizerBothPlanesConstraintShift->SetLimitedVariable(1, "zDUT_d0", DUT_z, 10., telPlaneprev_.z, telPlanenext_.z);
  //minimizerBothPlanesConstraintShift->SetLimitedVariable(1, "zDUT_d0",refPlaneZ+DUT_z, 10., -1000000., 0.);
  minimizerBothPlanesConstraintShift->SetLimitedVariable(1, "zDUT_d0",-556212., 10., -1000000., 0.);
  //minimizerBothPlanesConstraintShift->SetLimitedVariable(1, "zDUT_d0",DUT_z-refPlaneZ, 10., -1000000., 1000000.);
  //minimizerBothPlanesConstraintShift->SetLimitedVariable(1, "zDUT_d0",DUT_z-refPlaneZ, 10., -1000000., 500000.);
  //minimizerBothPlanesConstraintShift->SetLimitedVariable(2, "deltaZ", 1800., 1., 0., 8000.);
  minimizerBothPlanesConstraintShift->SetLimitedVariable(2, "deltaZ", 1800., 1., -3000., 3000.);
  minimizerBothPlanesConstraintShift->SetLimitedVariable(3, "theta", TMath::ATan((offset_init_ClsD1-offset_init_ClsD0)*1000/1800.), 0.001, -20.*TMath::Pi()/180., 20.*TMath::Pi()/180.);
  //minimizerBothPlanesConstraintShift->SetLimitedVariable(3, "theta", TMath::ATan((offset_init_ClsD1-offset_init_ClsD0)*1000/1800), 0.001, (-20.+180)*TMath::Pi()/180., (20.+180)*TMath::Pi()/180.);
  minimizerBothPlanesConstraintShift->SetLimitedVariable(4, "offsetPlanes", 30., 1., -300., 300.);

  cout << "DUT both planes with deltaOffset constraint and shift between Planes: Start chi2 minimization"<<endl;
  minimizerBothPlanesConstraintShift->Minimize();
  const double *resultMinimizerBothPlanesConstraintShift = minimizerBothPlanesConstraintShift->X();
  double* resultBothPlanesConstraintShift = new double[5];
  resultBothPlanesConstraintShift[0] = resultMinimizerBothPlanesConstraintShift[0];
  resultBothPlanesConstraintShift[1] = resultMinimizerBothPlanesConstraintShift[1];
  resultBothPlanesConstraintShift[2] = resultMinimizerBothPlanesConstraintShift[2];
  resultBothPlanesConstraintShift[3] = resultMinimizerBothPlanesConstraintShift[3];
  resultBothPlanesConstraintShift[4] = resultMinimizerBothPlanesConstraintShift[4];
  double chi2BothPlanesConstraintShift = ComputeChi2BothPlanes(resultBothPlanesConstraintShift);
  cout << "BothPlanesConstraintShift offset_d0="<< resultBothPlanesConstraintShift[0]<<" zDUT_d0="<<resultBothPlanesConstraintShift[1]<<" deltaZ="<< resultBothPlanesConstraintShift[2]<<" theta="<<resultBothPlanesConstraintShift[3]*180./TMath::Pi()<< " shiftPlanes="<<resultBothPlanesConstraintShift[4] << " chi2="<<chi2BothPlanesConstraintShift<<endl;

  for (unsigned int i=0; i<selectedTk_bothPlanes_1Cls.size(); i++){
    double xDUT_d0 =   bothPlanes_DutXposD0.at(i);
    double xDUT_d1 =   bothPlanes_DutXposD1.at(i);
    std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, resultBothPlanesConstraintShift[0], resultBothPlanesConstraintShift[1], resultBothPlanesConstraintShift[2], resultBothPlanesConstraintShift[3], resultBothPlanesConstraintShift[4]);
    double xTkAtDUT_d0 = xTkAtDUT.first;
    double xTkAtDUT_d1 = xTkAtDUT.second;
    double resDUT_d0 = xDUT_d0 - xTkAtDUT_d0/1000.;
    double resDUT_d1 = xDUT_d1 - xTkAtDUT_d1/1000.;
    hist_->fillHist1D(dnamebottom,"d0_1tk1ClusterBothPlanesConstraintShift_diffX_aligned", resDUT_d0);
    hist_->fillHist1D(dnametop,"d1_1tk1ClusterBothPlanesConstraintShift_diffX_aligned", resDUT_d1);
  }
  */

  doConstrainDeltaOffset = true;
  doAllowOffsetPlanes = true;
  doAllowPhiBothPlanes = true;
  doD0 = true;
  doD1 = true;
  minimizerBothPlanesConstraintShiftPhi->Clear();

  minimizerBothPlanesConstraintShiftPhi->SetLimitedVariable(0, "offset_d0", offset_init_ClsD0*1000, 0.1, -30000., 30000.);
  minimizerBothPlanesConstraintShiftPhi->SetLimitedVariable(1, "zDUT_d0",-556212., 10., -1000000., 0.);
  minimizerBothPlanesConstraintShiftPhi->SetLimitedVariable(2, "deltaZ", 1800., 1., -3000., 3000.);
  minimizerBothPlanesConstraintShiftPhi->SetLimitedVariable(3, "theta", TMath::ATan((offset_init_ClsD1-offset_init_ClsD0)*1000/1800.), 0.001, -20.*TMath::Pi()/180., 20.*TMath::Pi()/180.);
  minimizerBothPlanesConstraintShiftPhi->SetLimitedVariable(4, "offsetPlanes", 30., 1., -300., 300.);
  minimizerBothPlanesConstraintShiftPhi->SetLimitedVariable(5, "phi_d0", 0., 0.001, -5.*TMath::Pi()/180., 5.*TMath::Pi()/180.);
  minimizerBothPlanesConstraintShiftPhi->SetLimitedVariable(6, "phi_d1", 0., 0.001, -5.*TMath::Pi()/180., 5.*TMath::Pi()/180.);

  cout << "DUT both planes with deltaOffset constraint and shift between Planes: Start chi2 minimization"<<endl;
  minimizerBothPlanesConstraintShiftPhi->Minimize();
  const double *resultMinimizerBothPlanesConstraintShiftPhi = minimizerBothPlanesConstraintShiftPhi->X();
  double* resultBothPlanesConstraintShiftPhi = new double[5];
  resultBothPlanesConstraintShiftPhi[0] = resultMinimizerBothPlanesConstraintShiftPhi[0];
  resultBothPlanesConstraintShiftPhi[1] = resultMinimizerBothPlanesConstraintShiftPhi[1];
  resultBothPlanesConstraintShiftPhi[2] = resultMinimizerBothPlanesConstraintShiftPhi[2];
  resultBothPlanesConstraintShiftPhi[3] = resultMinimizerBothPlanesConstraintShiftPhi[3];
  resultBothPlanesConstraintShiftPhi[4] = resultMinimizerBothPlanesConstraintShiftPhi[4];
  resultBothPlanesConstraintShiftPhi[5] = resultMinimizerBothPlanesConstraintShiftPhi[5];
  resultBothPlanesConstraintShiftPhi[6] = resultMinimizerBothPlanesConstraintShiftPhi[6];

  double chi2BothPlanesConstraintShiftPhi = ComputeChi2BothPlanes(resultBothPlanesConstraintShiftPhi);
  cout << "BothPlanesConstraintShiftPhi offset_d0="<< resultBothPlanesConstraintShiftPhi[0]<<" zDUT_d0="<<resultBothPlanesConstraintShiftPhi[1]<<" deltaZ="<< resultBothPlanesConstraintShiftPhi[2]<<" theta="<<resultBothPlanesConstraintShiftPhi[3]*180./TMath::Pi()<< " shiftPlanes="<<resultBothPlanesConstraintShiftPhi[4] << " phi_d0="<<resultBothPlanesConstraintShiftPhi[5]<<" phi_d1="<<resultBothPlanesConstraintShiftPhi[6] << " chi2="<<chi2BothPlanesConstraintShiftPhi<<endl;

  for (unsigned int i=0; i<selectedTk_bothPlanes_1Cls.size(); i++){
    double xDUT_d0 =   bothPlanes_DutXposD0.at(i);
    double xDUT_d1 =   bothPlanes_DutXposD1.at(i);
    std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, resultBothPlanesConstraintShiftPhi[0], resultBothPlanesConstraintShiftPhi[1], resultBothPlanesConstraintShiftPhi[2], resultBothPlanesConstraintShiftPhi[3], resultBothPlanesConstraintShiftPhi[4], resultBothPlanesConstraintShiftPhi[5], resultBothPlanesConstraintShiftPhi[6]);
    double xTkAtDUT_d0 = xTkAtDUT.first;
    double xTkAtDUT_d1 = xTkAtDUT.second;
    double resDUT_d0 = xDUT_d0 - xTkAtDUT_d0/1000.;
    double resDUT_d1 = xDUT_d1 - xTkAtDUT_d1/1000.;
    hist_->fillHist1D(dnamebottom,"d0_1tk1ClusterBothPlanesConstraintShiftPhi_diffX_aligned", resDUT_d0);
    hist_->fillHist1D(dnametop,"d1_1tk1ClusterBothPlanesConstraintShiftPhi_diffX_aligned", resDUT_d1);
  }




  TF1* fGausResiduals = new TF1("fGausResiduals", "gaus", -100, 100);
  TF1* fStepGaus = new TF1("FuncStepGaus", Utility::FuncStepGausShift, -0.1, 0.1, 5);
  TH1* htmp;
  float center;
  float rms;
/*
  //Fit Residuals Gaussian convulated with Step Function
  //TF1* fGausResiduals = new TF1("fGausResiduals", "gaus", -100, 100);
  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom,"d0_1tk1Hit_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");

  //TF1* fStepGaus = new TF1("FuncStepGaus", FuncStepGaus, -0.1, 0.1, 4);
  fStepGaus->SetLineWidth(2);
  fStepGaus->SetLineColor(kRed);
  fStepGaus->SetParLimits(0, 0., 0.1);
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParLimits(1, 0.0, 0.05);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParLimits(3, 0, 50);
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop,"d1_1tk1Hit_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);


  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom,"d0_1tk1ClusterBothPlanes_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop,"d1_1tk1ClusterBothPlanes_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);


  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom,"d0_1tk1ClusterBothPlanesConstraint_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop,"d1_1tk1ClusterBothPlanesConstraint_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);
*/
/*
  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom,"d0_1tk1ClusterBothPlanesConstraintShift_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop,"d1_1tk1ClusterBothPlanesConstraintShift_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);
*/

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom,"d0_1tk1ClusterBothPlanesConstraintShiftPhi_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop,"d1_1tk1ClusterBothPlanesConstraintShiftPhi_diffX_aligned"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

  bool doMinimizerChecks = true;
  if (doMinimizerChecks){
/*
    doConstrainDeltaOffset = false;
    doD0 = true;
    doD1 = true;
    TH1* hChi2vsTheta = dynamic_cast<TH1F*>(hist_->GetHistoByName(dnamebottom,"bothPlanes_chi2VsTheta"));
    for (int i=0; i<=40; i+=1){
      double theta_i = ((double)(i)-20.) * TMath::Pi()/180.;
      resultBothPlanes[4] = theta_i;
      double chi2_i = ComputeChi2BothPlanes(resultBothPlanes);
      cout << "theta_i="<<theta_i*180./TMath::Pi()<<" chi2_i="<<chi2_i <<endl;
      hChi2vsTheta->Fill((double)(i)-20., chi2_i);
    }
*/
    doConstrainDeltaOffset = true;
    doAllowOffsetPlanes = true;
    doAllowPhiBothPlanes = true;
    doD0 = true;
    doD1 = true;
    double theta_save = resultBothPlanesConstraintShiftPhi[3];
    TH1* hChi2vsTheta = dynamic_cast<TH1F*>(hist_->GetHistoByName(dnamebottom,"bothPlanesConstraintShiftPhi_chi2VsTheta"));
    for (int i=0; i<=40; i+=1){
      double theta_i = ((double)(i)-20.) * TMath::Pi()/180.;
      resultBothPlanesConstraintShiftPhi[3] = theta_i;
      double chi2_i = ComputeChi2BothPlanes(resultBothPlanesConstraintShiftPhi);
      cout << "theta_i="<<theta_i*180./TMath::Pi()<<" chi2_i="<<chi2_i <<endl;
      hChi2vsTheta->Fill((double)(i)-20., chi2_i);
    }
    resultBothPlanesConstraintShiftPhi[3] = theta_save;

    double deltaZ_save = resultBothPlanesConstraintShiftPhi[2];
    TH1* hChi2vsDeltaZ = dynamic_cast<TH1F*>(hist_->GetHistoByName(dnamebottom,"bothPlanesConstraintShiftPhi_chi2VsDeltaZ"));
    for (int i=0; i<=40; i+=1){
      double deltaZ_i = ((double)(i))*250.;
      resultBothPlanesConstraintShiftPhi[2] = deltaZ_i;
      double chi2_i = ComputeChi2BothPlanes(resultBothPlanesConstraintShiftPhi);
      cout << "deltaZ_i="<<deltaZ_i<<" chi2_i="<<chi2_i <<endl;
      hChi2vsDeltaZ->Fill(deltaZ_i, chi2_i);
    }
    resultBothPlanesConstraintShiftPhi[2] = deltaZ_save;

   double zDUT0_save = resultBothPlanesConstraintShiftPhi[1];
   TH1* hChi2vsZdut0 = dynamic_cast<TH1F*>(hist_->GetHistoByName(dnamebottom,"bothPlanesConstraintShiftPhi_chi2VsZdut0"));
   for (int i=0; i<=40; i+=1){
     double zDUT0_i = ((double)(i))*10000.-200000.;
     resultBothPlanesConstraintShiftPhi[1] = zDUT0_i;
     double chi2_i = ComputeChi2BothPlanes(resultBothPlanesConstraintShiftPhi);
     cout << "zDUT0_i="<<zDUT0_i<<" chi2_i="<<chi2_i <<endl;
     hChi2vsZdut0->Fill(zDUT0_i, chi2_i);
   }
   resultBothPlanesConstraintShiftPhi[1] = zDUT0_save;

   double shiftPlanes_save = resultBothPlanesConstraintShiftPhi[4];
   TH1* hChi2vsShiftPlanes = dynamic_cast<TH1F*>(hist_->GetHistoByName(dnamebottom,"bothPlanesConstraintShiftPhi_chi2VsShiftPlanes"));
   for (int i=0; i<=40; i+=1){
     double shiftPlanes_i = ((double)(i))*2.5 -50.;  
     resultBothPlanesConstraintShiftPhi[4] = shiftPlanes_i;
     double chi2_i = ComputeChi2BothPlanes(resultBothPlanesConstraintShiftPhi);
     cout << "shiftPlanes_i="<<shiftPlanes_i<<" chi2_i="<<chi2_i <<endl;
     hChi2vsShiftPlanes->Fill(shiftPlanes_i, chi2_i);
   }
   resultBothPlanesConstraintShiftPhi[4] = shiftPlanes_save;

  }
  //END NOV17 code
  
  /*NOT required
  cout << "------------------"<<endl;
  cout << "Telescope Fei4 matching summary" << endl;
  std::cout << "offsetFEI4X=" << al.offsetFEI4x() << endl;
  std::cout << "offsetFEI4Y=" << al.offsetFEI4y()<<endl;
  std::cout << "residualSigmaFEI4X=" << al.residualSigmaFEI4x() << endl;
  std::cout << "residualSigmaFEI4Y=" << al.residualSigmaFEI4y() << endl;
  cout << "------------------"<<endl;
  */
  
  cout << "Alignment summary: "<<endl;
  //cout << "D0 offset="<< resultD0[0]<<" zDUT="<<resultD0[1]<<" theta="<<resultD0[2]*180./TMath::Pi()<<" chi2="<<chi2D0<<endl;
  //cout << "D1 offset="<< resultD1[0]<<" zDUT="<<resultD1[1]<<" theta="<<resultD1[2]*180./TMath::Pi()<<" chi2="<<chi2D1<<endl;
  //cout << "BothPlanes offset_d0="<< resultBothPlanes[0]<<" zDUT_d0="<<resultBothPlanes[1]<<" offset_d1="<< resultBothPlanes[2]<<" zDUT_d1="<<resultBothPlanes[3] << " theta="<<resultBothPlanes[4]*180./TMath::Pi()<< " chi2="<<chi2BothPlanes<<endl;
  //cout << "BothPlanesConstraint offset_d0="<< resultBothPlanesConstraint[0]<<" zDUT_d0="<<resultBothPlanesConstraint[1]<<" deltaZ="<< resultBothPlanesConstraint[2]<<" theta="<<resultBothPlanesConstraint[3]*180./TMath::Pi()<< " chi2="<<chi2BothPlanesConstraint<<endl;
  //cout << "BothPlanesConstraintShift offset_d0="<< resultBothPlanesConstraintShift[0]<<" zDUT_d0="<<resultBothPlanesConstraintShift[1]<<" deltaZ="<< resultBothPlanesConstraintShift[2]<<" theta="<<resultBothPlanesConstraintShift[3]*180./TMath::Pi()<< " shiftPlanes="<<resultBothPlanesConstraintShift[4] << " chi2="<<chi2BothPlanesConstraintShift<<endl;
  cout << "BothPlanesConstraintShiftPhi offset_d0="<< resultBothPlanesConstraintShiftPhi[0]<<" zDUT_d0="<<resultBothPlanesConstraintShiftPhi[1]<<" deltaZ="<< resultBothPlanesConstraintShiftPhi[2]<<" theta="<<resultBothPlanesConstraintShiftPhi[3]*180./TMath::Pi()<< " shiftPlanes="<<resultBothPlanesConstraintShiftPhi[4] << " phi_d0="<<resultBothPlanesConstraintShiftPhi[5]*180./TMath::Pi()<<" phi_d1="<<resultBothPlanesConstraintShiftPhi[6]*180./TMath::Pi() << " chi2="<<chi2BothPlanesConstraintShiftPhi<<endl;
  std::cout << "Dumping alignment in Data directory!!" << std::endl;
  dumpAlignment(resultBothPlanesConstraintShiftPhi);


/*
  //Dump Alignment output to alignment text file
  std::ofstream fileAlignment;
  if(isProduction_) {
    fileAlignment.open(alignparFile_.c_str(), ios::out | ios::app);
  } else {
    fileAlignment.open(alignparFile_.c_str(), ios::out);
  }
  if(fileAlignment) {
    fileAlignment << "Run="<< runNumber_
                  //<< ":offsetFEI4X=" << al.offsetFEI4x()
                  //<< ":offsetFEI4Y=" << al.offsetFEI4y()
                  //<< ":residualSigmaFEI4X=" << al.residualSigmaFEI4x()
                  //<< ":residualSigmaFEI4Y=" << al.residualSigmaFEI4y()
                  << ":zD0="<< resultBothPlanesConstraint[1]
                  << ":offsetD0=" << resultBothPlanesConstraint[0]
                  << ":deltaZ=" << resultBothPlanesConstraint[2]
                  << ":angle=" << resultBothPlanesConstraint[3]*180./TMath::Pi() << endl;
    fileAlignment.close();
  } else std::cout << "Dump File could not be opened!!" << std::endl;
  */


  //GET FINAL RESIDUALS AFTER ALIGNMENT AND FIT WITH STEP+GAUS
  for (Long64_t jentry=0; jentry<nEntries_ && jentry<maxEvent_;jentry++) {
    clearEvent();
    Long64_t ientry = analysisTree()->GetEntry(jentry);
    if (ientry < 0) break;
    if (jentry%1000 == 0) {
       cout << " Events processed. " << std::setw(8) << jentry
            << endl;
    }

    setDetChannelVectors();
    const auto& d0c0 = (*modVec())[0].bottomHits;
    const auto& d1c0 = (*modVec())[0].topHits;
    const auto& d0Cls = (*modVec())[0].bottomOfflineCls;
    const auto& d1Cls = (*modVec())[0].topOfflineCls;

    std::vector<tbeam::OfflineTrack>  selectedTk = event()->tracks;
    if (selectedTk.size()!=1) continue;
    if (selectedTk[0].chi2()>5.) continue;
 
    std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk[0], refPlaneZ, resultBothPlanesConstraintShiftPhi[0], resultBothPlanesConstraintShiftPhi[1], resultBothPlanesConstraintShiftPhi[2], resultBothPlanesConstraintShiftPhi[3], resultBothPlanesConstraintShiftPhi[4], resultBothPlanesConstraintShiftPhi[5], resultBothPlanesConstraintShiftPhi[6]);

    double D0xDUT;
    double D1xDUT;

    if (d0Cls.size()==1){
      for(auto& cl : d0Cls ) {
        D0xDUT = (cl.center()-nStrips/2)*dutpitch();
        D0xDUT = -D0xDUT;
        if (cl.size()==1) hist_->fillHist1D(dnamebottom,"d0_1tk1Cls_1Hit_diffX_afterAlignment", D0xDUT-xTkAtDUT.first/1000.);
	if (cl.size()==2) hist_->fillHist1D(dnamebottom,"d0_1tk1Cls_2Hit_diffX_afterAlignment", D0xDUT-xTkAtDUT.first/1000.);
        if (cl.size()>2) hist_->fillHist1D(dnamebottom,"d0_1tk1Cls_gt2Hit_diffX_afterAlignment", D0xDUT-xTkAtDUT.first/1000.);
        hist_->fillHist1D(dnamebottom,"d0_1tk1Cls_allHit_diffX_afterAlignment", D0xDUT-xTkAtDUT.first/1000.);
      }
    }
    if (d1Cls.size()==1){
      for(auto& cl : d1Cls ) {
        D1xDUT = (cl.center()-nStrips/2)*dutpitch();
        D1xDUT = -D1xDUT;
        if (cl.size()==1) hist_->fillHist1D(dnametop,"d1_1tk1Cls_1Hit_diffX_afterAlignment", D1xDUT-xTkAtDUT.second/1000.);
        if (cl.size()==2) hist_->fillHist1D(dnametop,"d1_1tk1Cls_2Hit_diffX_afterAlignment", D1xDUT-xTkAtDUT.second/1000.);
        if (cl.size()>2) hist_->fillHist1D(dnametop,"d1_1tk1Cls_gt2Hit_diffX_afterAlignment", D1xDUT-xTkAtDUT.second/1000.);
        hist_->fillHist1D(dnametop,"d1_1tk1Cls_allHit_diffX_afterAlignment", D1xDUT-xTkAtDUT.second/1000.);
      }
    }


  }

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom,"d0_1tk1Cls_1Hit_diffX_afterAlignment"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop,"d1_1tk1Cls_1Hit_diffX_afterAlignment"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom,"d0_1tk1Cls_allHit_diffX_afterAlignment"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop,"d1_1tk1Cls_allHit_diffX_afterAlignment"));
  center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");

  center = fGausResiduals->GetParameter(1);
  rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  fStepGaus->SetParameter(0, 0.09);
  fStepGaus->SetParameter(1, 0.01);
  fStepGaus->SetParameter(2, htmp->GetMaximum());
  fStepGaus->SetParameter(3, 0);

  htmp->Fit(fStepGaus);

}

//Each of the following histogramns will be booked for each sensor
void AlignmentMultiDimAnalysis::bookTrackFitHistograms(TString& detId, float zMin, float zStep, int zNsteps){
  int nStrips = 254;
  TString dname = detId + "/TrackFit";
  //std::cout << "Entering bookTrackMatchHistograms with dnmae=" << dname << std::endl;
  hist_->hfile()->cd();
  hist_->hfile()->mkdir(dname);
  hist_->hfile()->cd(dname);
  //these will be added back once the zposition of the reference plane is known
  //new TH1D("tkposx_ref","Xpos of etrapolated track from reference plane; x [mm]; Events [a.u]",60/(250e-3),-30.,30.);
  //new TH1D("tkposy_ref","Ypos of etrapolated track from reference plane; x [mm]; Events [a.u]",60/(250e-3),-30.,30.);
  //new TH1D("hitresidualX_ref","Residual of extrapolated track(ref plane) with respect to hit; residual [mm]; Events [a.u]",40/(50e-3),-20.,20.);
  //new TH1D("clusresidualX_ref","Residual of extrapolated track(ref plane) with respect to cluster; residualX [mm]; Events [a.u]",80/(50e-3),-40.,40.);

  new TH1D("tkposx_prev","Xpos of etrapolated track from previous plane; x [mm]; Events [a.u]",60/(250e-3),-30.,30.);
  new TH1D("tkposy_prev","Ypos of etrapolated track from previous plane; x [mm]; Events [a.u]",60/(250e-3),-30.,30.);
  new TH1D("hitresidualX_prev","Residual of extrapolated track(prev plane) with respect to hit; residualX [mm]; Events [a.u]",80/(50e-3),-40.,40.);
  new TH1D("clusresidualX_prev","Residual of extrapolated track(prev plane) with respect to cluster; residualX [mm]; Events [a.u]",80/(50e-3),-40.,40.);

  new TH1D("tkposx_next","Xpos of etrapolated track from next plane; x [mm]; Events [a.u]",60/(250e-3),-30.,30.);
  new TH1D("tkposy_next","Ypos of etrapolated track from next plane; x [mm]; Events [a.u]",60/(250e-3),-30.,30.);
  new TH1D("hitresidualX_next","Residual of extrapolated track(next plane) with respect to hit; residualX [mm]; Events [a.u]",80/(50e-3),-40.,40.);
  new TH1D("clusresidualX_next","Residual of extrapolated track(next plane) with respect to cluster; residualX [mm]; Events [a.u]",80/(50e-3),-40.,40.);

  //check with Nicolas about the following histograms
  
  new TH1I("d0_1tk1Hit_diffX_tmp","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1Hit_diffX_tmp","X_{TkAtDUT}-X_{DUT}, d1",100000,-100,100);

  new TH1I("d0_1tk1Hit_diffX_start","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1Hit_diffX_start","X_{TkAtDUT}-X_{DUT}, d1",100000,-100,100);

  new TH1I("d0_1tk1Cls_diffX_start","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1Cls_diffX_start","X_{TkAtDUT}-X_{DUT}, d1",100000,-100,100);

  new TH1I("d0_1tk1Hit_diffX_aligned","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1Hit_diffX_aligned","X_{TkAtDUT}-X_{DUT}, d1",100000,-100,100);

  new TH1I("d0_1tk1ClusterBothPlanes_diffX_aligned","X_{TkAtDUT}-X_{Cls,DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1ClusterBothPlanes_diffX_aligned","X_{TkAtDUT}-X_{Cls,DUT}, d1",100000,-100,100);

  new TH1I("d0_1tk1ClusterBothPlanesConstraint_diffX_aligned","X_{TkAtDUT}-X_{Cls,DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1ClusterBothPlanesConstraint_diffX_aligned","X_{TkAtDUT}-X_{Cls,DUT}, d1",100000,-100,100);

  new TH1I("d0_1tk1ClusterBothPlanesConstraintShift_diffX_aligned","X_{TkAtDUT}-X_{Cls,DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1ClusterBothPlanesConstraintShift_diffX_aligned","X_{TkAtDUT}-X_{Cls,DUT}, d1",100000,-100,100);

  new TH1I("d0_1tk1ClusterBothPlanesConstraintShiftPhi_diffX_aligned","X_{TkAtDUT}-X_{Cls,DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1ClusterBothPlanesConstraintShiftPhi_diffX_aligned","X_{TkAtDUT}-X_{Cls,DUT}, d1",100000,-100,100);

  new TH1F("bothPlanes_chi2VsTheta","chi2 vs injected #theta", 41, -20.-0.5, 21.-0.5);
  new TH1F("bothPlanesConstraintShiftPhi_chi2VsTheta","chi2 vs injected #theta", 41, -20.-0.5, 21.-0.5);
  new TH1F("bothPlanesConstraintShiftPhi_chi2VsDeltaZ","chi2 vs injected #deltaZ", 41, 0.-125, 10250-125);
  new TH1F("bothPlanesConstraintShiftPhi_chi2VsZdut0", "chi2 vs injected #z_{DUT0}", 41, -200000, 200000);  
  new TH1F("bothPlanesConstraintShiftPhi_chi2VsShiftPlanes","chi2 vs injected shift between planes",41,-50,50);

  new TH1I("d0_1tk1Cls_1Hit_diffX_afterAlignment","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d0_1tk1Cls_2Hit_diffX_afterAlignment","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d0_1tk1Cls_gt2Hit_diffX_afterAlignment","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d0_1tk1Cls_allHit_diffX_afterAlignment","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);

  new TH1I("d1_1tk1Cls_1Hit_diffX_afterAlignment","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1Cls_2Hit_diffX_afterAlignment","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1Cls_gt2Hit_diffX_afterAlignment","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);
  new TH1I("d1_1tk1Cls_allHit_diffX_afterAlignment","X_{TkAtDUT}-X_{DUT}, d0",100000,-100,100);



}

void AlignmentMultiDimAnalysis::FillAlignmentOffsetVsZ(const char* det, const char* histo, int iz, float z, float x, float x_err){

hist_->hfile()->cd("TrackFit");
char histname[50];
strcpy( histname, det );
strcat( histname, histo );
TH1* h = (TH1*) gDirectory->Get(histname);
h->Fill(z, x);
h->SetBinError(iz+1, x_err);
}

double AlignmentMultiDimAnalysis::ComputeChi2(const double* x) const{
  
  double chi2 = 0;
  double offset = x[0];
  double zDUT = x[1];
  double theta = x[2];

  //double resTelescope = sqrt(0.090*0.090/12. + 0.0035*0.0035);
  double resTelescope = sqrt(0.090*0.090/12. + 0.010*0.010);

  unsigned int nEv = 0;
  if (doD0) nEv = selectedTk_d0_1Hit.size();
  if (doD1) nEv = selectedTk_d1_1Hit.size();

  double xTkAtDUT = 0;
  double xDUT = 0;
  double resDUT = 0;

  //std::string dnamebottom = (*modVec())[0].hdirbottom_ + "/TrackFit";
  //std::string dnametop = (*modVec())[0].hdirtop_ + "/TrackFit";

  TH1* htmp;
  if (doD0) htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom, "d0_1tk1Hit_diffX_tmp"));
  if (doD1) htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop, "d1_1tk1Hit_diffX_tmp"));
  htmp->Reset();

  //double refPlaneZ = telPlaneprev_.z;

  for (unsigned int i=0; i<nEv; i++){
    if (doD0){
      xDUT = d0_DutXpos.at(i);
      xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d0_1Hit.at(i), refPlaneZ, offset, zDUT, theta);
    }
    if (doD1){
      xDUT = d1_DutXpos.at(i);
      xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d1_1Hit.at(i), refPlaneZ, offset, zDUT, theta);
    }

    resDUT = xDUT - xTkAtDUT;
    if (doD0) hist_->fillHist1D(dnamebottom,"d0_1tk1Hit_diffX_tmp", resDUT);
    if (doD1) hist_->fillHist1D(dnametop,"d1_1tk1Hit_diffX_tmp", resDUT);

    chi2 += (resDUT/resTelescope)*(resDUT/resTelescope);
  }

  chi2 /= ((double)nEv);


  TF1* fGausExtractedX = new TF1("fGausExtractedX", Utility::FuncPol1Gaus, -10, 10, 5);

  if (doD0) htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom, "d0_1tk1Hit_diffX_tmp"));
  if (doD1) htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop, "d1_1tk1Hit_diffX_tmp"));
  double center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-2., center+2., "X");
  double cte = (htmp->GetBinContent(htmp->FindBin(center-2.))+htmp->GetBinContent(htmp->FindBin(center+2.)))/2.;
  fGausExtractedX->SetParLimits(0, 0, 2*htmp->GetMaximum());
  fGausExtractedX->SetParameter(0, htmp->GetMaximum());
  fGausExtractedX->SetParLimits(1, center-2., center+2.);
  fGausExtractedX->SetParameter(1, center);
  fGausExtractedX->SetParLimits(2, 0, 0.3);
  fGausExtractedX->SetParameter(2, 0.03);
  fGausExtractedX->SetParLimits(3, 0, 1000.);
  fGausExtractedX->SetParameter(3, cte);
  fGausExtractedX->SetParLimits(4, 0., 0.);
  fGausExtractedX->SetParameter(4, 0.);
  htmp->Fit("fGausExtractedX", "WW"); //WW

  center = fGausExtractedX->GetParameter(1);
  double height = fGausExtractedX->GetParameter(0);
  double rms = fGausExtractedX->GetParameter(2);
  bool failedFit = false;
  double center_err = fGausExtractedX->GetParError(1);
  if (!(center_err>0) || !(height>0)) failedFit = true;
  if (failedFit) {
    chi2=9999.;
    cout << "offset="<< offset<<" zDUT="<<zDUT<<" theta="<<theta*180./TMath::Pi()<<" chi2="<<chi2<<" Fit Failed !"<<endl;
    return chi2;
  }

  htmp->SetAxisRange(center-5*rms, center+5*rms, "X");

  int nEvWindow = 0;
  chi2 = 0;
  for (unsigned int i=0; i<nEv; i++){
    if (doD0){
      xDUT = d0_DutXpos.at(i);
      xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d0_1Hit.at(i), refPlaneZ, offset, zDUT, theta);
    }
    if (doD1){
      xDUT = d1_DutXpos.at(i);
      xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d1_1Hit.at(i), refPlaneZ, offset, zDUT, theta);
    }

    resDUT = xDUT - xTkAtDUT;
    //if (fabs(resDUT-center)<10.*rms) {
    if (fabs(resDUT-center)<3.*rms) {
      chi2 += (resDUT/resTelescope)*(resDUT/resTelescope);
      nEvWindow++;
    }
  }

  chi2 /= ((double)nEvWindow);
  cout << "nEvWindow="<<nEvWindow<<endl;
  if (chi2==0 || nEvWindow==0) chi2=9999.;

  cout << "offset="<< offset<<" zDUT="<<zDUT<<" theta="<<theta*180./TMath::Pi()<<" chi2="<<chi2<<endl;


  return chi2;
  
}

double AlignmentMultiDimAnalysis::ComputeChi2BothPlanes(const double* x) const{

  double chi2 = 0;
  double offset_d0 = 0;
  double zDUT_d0 = 0;
  double offset_d1 = 0;
  double zDUT_d1 = 0;
  double theta = 0;
  double deltaZ = 0;
  double offsetPlanes = 0;
  double phi_d0 = 0;
  double phi_d1 = 0;

  if (!doConstrainDeltaOffset){
    offset_d0 = x[0];
    zDUT_d0 = x[1];
    offset_d1 = x[2];
    zDUT_d1 = x[3];
    theta = x[4];
  }
  if (doConstrainDeltaOffset && !doAllowOffsetPlanes){
    offset_d0 = x[0];
    zDUT_d0 = x[1];
    deltaZ = x[2];
    theta = x[3];
    zDUT_d1 = zDUT_d0 + deltaZ;
  }
  if (doConstrainDeltaOffset && doAllowOffsetPlanes){
    offset_d0 = x[0];
    zDUT_d0 = x[1];
    deltaZ = x[2];
    theta = x[3];
    offsetPlanes = x[4];
    if (doAllowPhiBothPlanes){
      phi_d0 = x[5];
      phi_d1 = x[6];
    }
  }

  double resTelescope = sqrt(0.090*0.090/12. + 0.010*0.010);

  unsigned int nEv = selectedTk_bothPlanes_1Cls.size();

  double xTkAtDUT_d0 = 0;
  double xDUT_d0 = 0;
  double resDUT_d0 = 0;
  double xTkAtDUT_d1 = 0;
  double xDUT_d1 = 0;
  double resDUT_d1 = 0;

  TH1* htmp0;
  TH1* htmp1;
  if (doD0) htmp0 = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom, "d0_1tk1Hit_diffX_tmp"));
  if (doD1) htmp1 = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop, "d1_1tk1Hit_diffX_tmp"));
  htmp0->Reset("ICESM");
  htmp1->Reset("ICESM");

  //double refPlaneZ = telPlaneprev_.z;

  for (unsigned int i=0; i<nEv; i++){
    if (doD0 && doD1){
      xDUT_d0 = bothPlanes_DutXposD0.at(i);
      xDUT_d1 = bothPlanes_DutXposD1.at(i);
      if (!doConstrainDeltaOffset){
        xTkAtDUT_d0 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d0, zDUT_d0, theta);
        xTkAtDUT_d1 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d1, zDUT_d1, theta);
      }
      if (doConstrainDeltaOffset && !doAllowOffsetPlanes){
	std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d0, zDUT_d0, deltaZ, theta);
        xTkAtDUT_d0 = xTkAtDUT.first;
        xTkAtDUT_d1 = xTkAtDUT.second;
      }
      if (doConstrainDeltaOffset && doAllowOffsetPlanes && !doAllowPhiBothPlanes){
        std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d0, zDUT_d0, deltaZ, theta, offsetPlanes);
	xTkAtDUT_d0 = xTkAtDUT.first;
        xTkAtDUT_d1 = xTkAtDUT.second;
      }
      if (doConstrainDeltaOffset && doAllowOffsetPlanes && doAllowPhiBothPlanes){
        std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d0, zDUT_d0, deltaZ, theta, offsetPlanes, phi_d0, phi_d1);
        xTkAtDUT_d0 = xTkAtDUT.first;
        xTkAtDUT_d1 = xTkAtDUT.second;
      }

      resDUT_d0 = xDUT_d0 - xTkAtDUT_d0/1000.;
      resDUT_d1 = xDUT_d1 - xTkAtDUT_d1/1000.;

      hist_->fillHist1D(dnamebottom,"d0_1tk1Hit_diffX_tmp", resDUT_d0);
      hist_->fillHist1D(dnametop,"d1_1tk1Hit_diffX_tmp", resDUT_d1);

     //chi2 += ( (resDUT_d0/resTelescope)*(resDUT_d0/resTelescope) + (resDUT_d1/resTelescope)*(resDUT_d1/resTelescope) );
     chi2 += fabs(resDUT_d0/resTelescope) + fabs(resDUT_d1/resTelescope);
    }
  }

  chi2 /= ((double)nEv);


  TF1* fGausExtractedX = new TF1("fGausExtractedX", Utility::FuncPol1Gaus, -10, 10, 5);

  double xwindow = 2.;

  if (doD0) htmp0 = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnamebottom, "d0_1tk1Hit_diffX_tmp"));
  htmp0->GetXaxis()->UnZoom();
  double center_d0 = ((float)htmp0->GetMaximumBin())*(htmp0->GetXaxis()->GetXmax()-htmp0->GetXaxis()->GetXmin())/((float)htmp0->GetNbinsX()) + htmp0->GetXaxis()->GetXmin();//htmp0->GetMean();
  htmp0->SetAxisRange(center_d0-xwindow, center_d0+xwindow, "X");
  double cte_d0 = (htmp0->GetBinContent(htmp0->FindBin(center_d0-xwindow))+htmp0->GetBinContent(htmp0->FindBin(center_d0+xwindow)))/2.;
  fGausExtractedX->SetParLimits(0, 0, 2*htmp0->GetMaximum());
  fGausExtractedX->SetParameter(0, htmp0->GetMaximum());
  fGausExtractedX->SetParLimits(1, center_d0-xwindow, center_d0+xwindow);
  fGausExtractedX->SetParameter(1, center_d0);
  fGausExtractedX->SetParLimits(2, 0, 0.3);
  fGausExtractedX->SetParameter(2, 0.03);
  fGausExtractedX->SetParLimits(3, 0, 1000.);
  fGausExtractedX->SetParameter(3, cte_d0);
  //fGausExtractedX->SetParLimits(4, 0., 0.);
  //fGausExtractedX->SetParameter(4, 0.);
  fGausExtractedX->FixParameter(4, 0);
  htmp0->Fit("fGausExtractedX", "WW"); //WW

  center_d0 = fGausExtractedX->GetParameter(1);
  double height_d0 = fGausExtractedX->GetParameter(0);
  double rms_d0 = fGausExtractedX->GetParameter(2);
  bool failedFit_d0 = false;
  double center_err_d0 = fGausExtractedX->GetParError(1);
  if (!(center_err_d0>0) || !(height_d0>5.) || !(fGausExtractedX->GetChisquare()>0)) failedFit_d0 = true;
  if (failedFit_d0) {
    chi2=99999.;
      if (!doConstrainDeltaOffset) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" offset_d1=" << offset_d1<< " zDUT_d1="<< zDUT_d1<<" chi2="<<chi2<<" theta="<<theta*180./TMath::Pi()<<" Fit Failed !"<<endl;
      if (doConstrainDeltaOffset) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" deltaZ="<<deltaZ<<" theta="<<theta*180./TMath::Pi()<<" chi2="<<chi2<< " Fit Failed !"<<endl;
    return chi2;
  }

  //htmp0->SetAxisRange(center_d0-5*rms_d0, center_d0+5*rms_d0, "X");

  if (doD1) htmp1 = dynamic_cast<TH1I*>(hist_->GetHistoByName(dnametop, "d1_1tk1Hit_diffX_tmp"));
  htmp1->GetXaxis()->UnZoom();
  double center_d1 = ((float)htmp1->GetMaximumBin())*(htmp1->GetXaxis()->GetXmax()-htmp1->GetXaxis()->GetXmin())/((float)htmp1->GetNbinsX()) + htmp1->GetXaxis()->GetXmin();//htmp1->GetMean();
  htmp1->SetAxisRange(center_d1-xwindow, center_d1+xwindow, "X");
  double cte_d1 = (htmp1->GetBinContent(htmp1->FindBin(center_d1-xwindow))+htmp1->GetBinContent(htmp1->FindBin(center_d1+xwindow)))/2.;
  fGausExtractedX->SetParLimits(0, 0, 2*htmp1->GetMaximum());
  fGausExtractedX->SetParameter(0, htmp1->GetMaximum());
  fGausExtractedX->SetParLimits(1, center_d1-xwindow, center_d1+xwindow);
  fGausExtractedX->SetParameter(1, center_d1);
  fGausExtractedX->SetParLimits(2, 0, 0.3);
  fGausExtractedX->SetParameter(2, 0.03);
  fGausExtractedX->SetParLimits(3, 0, 1000.);
  fGausExtractedX->SetParameter(3, cte_d1);
  //fGausExtractedX->SetParLimits(4, 0., 0.);
  //fGausExtractedX->SetParameter(4, 0.);
  fGausExtractedX->FixParameter(4, 0);
  htmp1->Fit("fGausExtractedX", "WW"); //WW

  center_d1 = fGausExtractedX->GetParameter(1);
  double height_d1 = fGausExtractedX->GetParameter(0);
  double rms_d1 = fGausExtractedX->GetParameter(2);
  bool failedFit_d1 = false;
  double center_err_d1 = fGausExtractedX->GetParError(1);
  if (!(center_err_d1>0) || !(height_d1>5.) || !(fGausExtractedX->GetChisquare()>0)) failedFit_d1 = true;
  if (failedFit_d1) {
    chi2=99999.;
      if (!doConstrainDeltaOffset) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" offset_d1=" << offset_d1<< " zDUT_d1="<< zDUT_d1<<" chi2="<<chi2<<" theta="<<theta*180./TMath::Pi()<<" Fit Failed !"<<endl;
      if (doConstrainDeltaOffset) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" deltaZ="<<deltaZ<<" theta="<<theta*180./TMath::Pi()<<" chi2="<<chi2<< " Fit Failed !"<<endl;
    return chi2;
  }

  //htmp1->SetAxisRange(center_d1-5*rms_d1, center_d1+5*rms_d1, "X");

  chi2 = 0;
  int nEvWindow = 0;
  for (unsigned int i=0; i<nEv; i++){
    if (doD0 && doD1){
      xDUT_d0 = bothPlanes_DutXposD0.at(i);
      xDUT_d1 = bothPlanes_DutXposD1.at(i);
      if (!doConstrainDeltaOffset){
        xTkAtDUT_d0 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d0, zDUT_d0, theta);
        xTkAtDUT_d1 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d1, zDUT_d1, theta);
      }
      if (doConstrainDeltaOffset && !doAllowOffsetPlanes){
        std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d0, zDUT_d0, deltaZ, theta);
        xTkAtDUT_d0 = xTkAtDUT.first;
        xTkAtDUT_d1 = xTkAtDUT.second;
      }
      if (doConstrainDeltaOffset && doAllowOffsetPlanes && !doAllowPhiBothPlanes){
        std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d0, zDUT_d0, deltaZ, theta, offsetPlanes);
        xTkAtDUT_d0 = xTkAtDUT.first;
        xTkAtDUT_d1 = xTkAtDUT.second;
      }
      if (doConstrainDeltaOffset && doAllowOffsetPlanes && doAllowPhiBothPlanes){
        std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), refPlaneZ, offset_d0, zDUT_d0, deltaZ, theta, offsetPlanes, phi_d0, phi_d1);
        xTkAtDUT_d0 = xTkAtDUT.first;
        xTkAtDUT_d1 = xTkAtDUT.second;
      }

      resDUT_d0 = xDUT_d0 - xTkAtDUT_d0/1000.;
      resDUT_d1 = xDUT_d1 - xTkAtDUT_d1/1000.;
    }

  //if (fabs(resDUT_d0-center_d0)<10.*rms_d0 && fabs(resDUT_d1-center_d1)<10.*rms_d1) {
    if (fabs(resDUT_d0-center_d0)<3.*rms_d0 && fabs(resDUT_d1-center_d1)<3.*rms_d1) {
      //chi2 += ( (resDUT_d0/resTelescope)*(resDUT_d0/resTelescope) + (resDUT_d1/resTelescope)*(resDUT_d1/resTelescope) );
      chi2 += fabs(resDUT_d0/resTelescope) + fabs(resDUT_d1/resTelescope);
      nEvWindow++;
    }
  }

  cout << "nEvWindow="<<nEvWindow<<endl;
  if (nEvWindow>0) chi2 /= ((double)nEvWindow);
  else chi2 = 99999.;

  if (!doConstrainDeltaOffset) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" offset_d1=" << offset_d1<< " zDUT_d1="<< zDUT_d1<<" theta="<<theta*180./TMath::Pi()<<" chi2="<<chi2<<endl;
  if (doConstrainDeltaOffset && !doAllowOffsetPlanes) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" deltaZ="<<deltaZ<<" theta="<<theta*180./TMath::Pi()<<" chi2="<<chi2<<endl;
  if (doConstrainDeltaOffset && doAllowOffsetPlanes && !doAllowPhiBothPlanes) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" deltaZ="<<deltaZ<<" theta="<<theta*180./TMath::Pi()<< " offsetPlanes="<<offsetPlanes <<" chi2="<<chi2<<endl;
  if (doConstrainDeltaOffset && doAllowOffsetPlanes && doAllowPhiBothPlanes) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" deltaZ="<<deltaZ<<" theta="<<theta*180./TMath::Pi()<< " offsetPlanes="<<offsetPlanes << " phi_d0="<<phi_d0*180./TMath::Pi()<<" phi_d1="<<phi_d1*180./TMath::Pi()<<" chi2="<<chi2<<endl;

  return chi2;
  
}

void AlignmentMultiDimAnalysis::clearEvent() {
  BeamAnaBase::clearEvent();
}
void AlignmentMultiDimAnalysis::endJob() {
  BeamAnaBase::endJob();
  hist_->closeFile();
}

AlignmentMultiDimAnalysis::~AlignmentMultiDimAnalysis(){
  delete hist_;
}
