/*!
 *         \file                AlignmentAnalysis.cc
 *         \brief               Alignment analysis, written starting from BaselineAnalysis
 *         \author              Nicolas Chanon
 *         \date                20/07/16
 *         Support :            mail to : nicolas.pierre.chanon@cern.ch
 * */

#include "TROOT.h"
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
  BeamAnaBase::BeamAnaBase(),
  isProduction_(false),
  alignparFile_("alignmentParameters.txt")
{
}

void AlignmentMultiDimAnalysis::beginJob() {
  BeamAnaBase::beginJob();
  hist_ = outFile();
  setAddresses();
  nEntries_ = analysisTree()->GetEntries();

  zMin = 200;
  zStep = 20.;
  zNsteps = 50;

  bookHistograms();
  analysisTree()->GetEntry(0);
  getCbcConfig(condEv()->cwd, condEv()->window);
  
  if(jobCardmap().find("isProductionmode") != jobCardmap().end())
    isProduction_ = (atoi(jobCardmap().at("isProductionmode").c_str()) > 0) ? true : false;
  if(jobCardmap().find("alignmentOutputFile") != jobCardmap().end())
    alignparFile_ = jobCardmap().at("alignmentOutputFile");
  if(jobCardmap().find("Run") != jobCardmap().end())
    runNumber_ = jobCardmap().at("Run");
  
  std::cout << "Additional Parameter specific to AlignmentReco>>" 
            << "\nisProductionMode:" << isProduction_
            << "\nalignparameterOutputFile:" << alignparFile_
            << std::endl;

}

void AlignmentMultiDimAnalysis::bookHistograms() {
  hist_->bookEventHistograms();
  hist_->bookTrackFitHistograms(zMin, zStep, zNsteps);
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


  if (!doTelMatching() || !hasTelescope()) return;
  
  float DUT_z = 460.0;//oct16=460;//may16=435
  float DUT_z_try = 400;
  
  Long64_t nbytes = 0, nb = 0;
  cout << "#Events=" << nEntries_ << endl;
  hist_->fillHist1D("EventInfo","nevents", nEntries_);
  
  std::cout << "CBC configuration:: SW=" << stubWindow()
	    << "\tCWD=" << cbcClusterWidth()
             << "\tOffset1="<< cbcOffset1() 
	    << "\tOffset2" << cbcOffset2()
	    << std::endl;
  //First Loop over events-inject z and compute residual
  //evaluate the best z alignment
  for (Long64_t jentry=0; jentry<nEntries_;jentry++) {
    clearEvent();
    Long64_t ientry = analysisTree()->GetEntry(jentry);
    if (ientry < 0) break;
    if (jentry%1000 == 0) {
       cout << " Events processed. " << std::setw(8) << jentry 
	    << endl;
    }
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
   
    if(fei4Ev()->nPixHits != 1)    continue;
 
    hist_->fillHist1D("EventInfo","condData", condEv()->condData);
    hist_->fillHist1D("EventInfo","tdcPhase", static_cast<unsigned int>(condEv()->tdcPhase));
    
    setDetChannelVectors();
    const auto& d0c0 = *det0C0();
    const auto& d0c1 = *det0C1();
    const auto& d1c0 = *det1C0();
    const auto& d1c1 = *det1C1();
 
    //Remove track duplicates 
    std::vector<tbeam::Track>  tkNoOv;
    Utility::removeTrackDuplicates(telEv(), tkNoOv);
	
    //Match with FEI4
    std::vector<tbeam::Track>  selectedTk;
    Utility::cutTrackFei4Residuals(fei4Ev(), tkNoOv, selectedTk, offsetfei4x(), offsetfei4y(), resfei4x(), resfei4y(), true); 
    //Find mean of residuals, scanning zDUT      
    if (selectedTk.size()!=1) continue;
    //cout << "NHits: d0, "<<d0c0.size()<<" ; d1, "<<d1c0.size()<<endl;
    if (d0c0.size()==1) {
      for (unsigned int ih=0; ih<d0c0.size(); ih++){
        float xDUT = (d0c0.at(ih) - nstrips()/2) * dutpitch();
        selectedTk_d0_1Hit.push_back(selectedTk[0]);
        d0_DutXpos.push_back(xDUT);
	float xTkAtDUT = selectedTk[0].xPos + (DUT_z-aLparameteres().FEI4_z)*selectedTk[0].dxdz;
        hist_->fillHist1D("TrackFit","d0_1tk1Hit_diffX_bis", xDUT-xTkAtDUT);
	DUT_z_try = zMin; //300
	//for (int iz=0; iz<zNsteps; iz++){
	//  DUT_z_try += zStep;
	  //xTkAtDUT = selectedTk[0].yPos + (DUT_z_try-aLparameteres().FEI4_z)*selectedTk[0].dydz;
	//  xTkAtDUT = selectedTk[0].xPos + (DUT_z_try-aLparameteres().FEI4_z)*selectedTk[0].dxdz;
          //xTkAtDUT = -1.*xTkAtDUT;
	//  hist_->fillHist1D("TrackFit",Form("d0_1tk1Hit_diffX_iz%i", iz), xDUT-xTkAtDUT);
	//}
      }
    }
    if (d1c0.size()==1){
      for (unsigned int ih=0; ih<d1c0.size(); ih++){
        float xDUT = (d1c0.at(ih) - nstrips()/2) * dutpitch();
        selectedTk_d1_1Hit.push_back(selectedTk[0]);
        d1_DutXpos.push_back(xDUT);
	float xTkAtDUT = selectedTk[0].xPos + (DUT_z-aLparameteres().FEI4_z)*selectedTk[0].dxdz;
        //xTkAtDUT = -1.*xTkAtDUT;
        hist_->fillHist1D("TrackFit","d1_1tk1Hit_diffX_bis", xDUT-xTkAtDUT);
	DUT_z_try = zMin;//300;
	//for (int iz=0; iz<zNsteps; iz++){
	  //DUT_z_try += zStep;
	  //xTkAtDUT = selectedTk[0].yPos + (DUT_z_try-aLparameteres().FEI4_z)*selectedTk[0].dydz;
	  //xTkAtDUT = selectedTk[0].xPos + (DUT_z_try-aLparameteres().FEI4_z)*selectedTk[0].dxdz;
          //xTkAtDUT = -1.*xTkAtDUT;
	  //hist_->fillHist1D("TrackFit",Form("d1_1tk1Hit_diffX_iz%i", iz), xDUT-xTkAtDUT);
	//}
      }
    }
    if (dutRecoClmap()->at("det0C0").size()==1 && dutRecoClmap()->at("det1C0").size()==1){
      double D0xDUT;
      double D1xDUT;
      for(auto& cl : dutRecoClmap()->at("det0C0") ) {
        D0xDUT = (cl.x-nstrips()/2)*dutpitch();
      }
      for(auto& cl : dutRecoClmap()->at("det1C0") ) {
        D1xDUT = (cl.x-nstrips()/2)*dutpitch();
      }
      float xTkAtDUT = selectedTk[0].xPos + (DUT_z-aLparameteres().FEI4_z)*selectedTk[0].dxdz;
      selectedTk_bothPlanes_1Cls.push_back(selectedTk[0]);
      bothPlanes_DutXposD0.push_back(D0xDUT);
      bothPlanes_DutXposD1.push_back(D1xDUT);
      hist_->fillHist1D("TrackFit","d0_1tk1Hit_diffX_ter", D0xDUT-xTkAtDUT);
      hist_->fillHist1D("TrackFit","d1_1tk1Hit_diffX_ter", D1xDUT-xTkAtDUT);
    }
  }//End of First Loop

  TH1* hTmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d0_1tk1Hit_diffX_bis"));
  TF1* fGausExtractedX = new TF1("fGausExtractedX", Utility::FuncPol1Gaus, -10, 10, 5);
  double offset_init_d0 = ((float)hTmp->GetMaximumBin())*(hTmp->GetXaxis()->GetXmax()-hTmp->GetXaxis()->GetXmin())/((float)hTmp->GetNbinsX()) + hTmp->GetXaxis()->GetXmin();//hTmp->GetMean();
  hTmp->SetAxisRange(offset_init_d0-1., offset_init_d0+1., "X");
  double cte = (hTmp->GetBinContent(hTmp->FindBin(offset_init_d0-1.))+hTmp->GetBinContent(hTmp->FindBin(offset_init_d0+1.)))/2.;
  fGausExtractedX->SetParameter(0, hTmp->GetMaximum());
  fGausExtractedX->SetParLimits(1, -5., 5.);
  fGausExtractedX->SetParameter(1, offset_init_d0);
  fGausExtractedX->SetParLimits(2, 0, 0.3);
  fGausExtractedX->SetParameter(2, 0.03);
  fGausExtractedX->SetParLimits(3, 0, 1000.);
  fGausExtractedX->SetParameter(3, cte);
  fGausExtractedX->SetParameter(4, 0);
  hTmp->Fit("fGausExtractedX", "WW");
  offset_init_d0 = fGausExtractedX->GetParameter(1);
  cout << "offset_init_d0="<<offset_init_d0<<endl; 

  hTmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d1_1tk1Hit_diffX_bis"));
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
  cout << "offset_init_d1="<<offset_init_d1<<endl;

  doD0 = true;
  doD1 = false;
  double chi2 = 0;
  minimizer->SetLimitedVariable(0, "offset", offset_init_d0, 0.0001, -5., 5.);
  minimizer->SetLimitedVariable(1, "zDUT", 435., 0.01, 200., 800.);
  minimizer->SetLimitedVariable(2, "theta", 0.*TMath::Pi()/180., 0.01, -90.*TMath::Pi()/180., 90.*TMath::Pi()/180.);

  cout << "DUT d0: Start chi2 minimization"<<endl;
  minimizer->Minimize();
  const double *resultMinimizerD0 = minimizer->X();
  double *resultD0 = new double[3];
  resultD0[0] = resultMinimizerD0[0];
  resultD0[1] = resultMinimizerD0[1];
  resultD0[2] = resultMinimizerD0[2];
  double chi2D0 = ComputeChi2(resultD0);
  cout << "D0 offset="<< resultD0[0]<<" zDUT="<<resultD0[1]<<" theta="<<resultD0[2]*180./TMath::Pi()<<" chi2="<<chi2D0<<endl;

  for (unsigned int i=0; i<selectedTk_d0_1Hit.size(); i++){
    double xDUT_d0 =   d0_DutXpos.at(i);
    double xTkAtDUT_d0 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d0_1Hit.at(i), aLparameteres().FEI4_z, resultD0[0], resultD0[1], resultD0[2]);
    double resDUT_d0 = xDUT_d0 - xTkAtDUT_d0;
    hist_->fillHist1D("TrackFit","d0_1tk1Hit_diffX_aligned", resDUT_d0);
  }  


  doD0 = false;
  doD1 = true;
  minimizer->Clear();
  minimizer->SetLimitedVariable(0, "offset", offset_init_d1, 0.0001, -5., 5.);
  minimizer->SetLimitedVariable(1, "zDUT", 435., 0.01, 200., 800.);
  minimizer->SetLimitedVariable(2, "theta", 0.*TMath::Pi()/180., 0.01, -90.*TMath::Pi()/180., 90.*TMath::Pi()/180.);

  cout << "DUT d1: Start chi2 minimization"<<endl;
  minimizer->Minimize();
  const double *resultMinimizerD1 = minimizer->X();
  double* resultD1 = new double[3];
  resultD1[0] = resultMinimizerD1[0];
  resultD1[1] = resultMinimizerD1[1];
  resultD1[2] = resultMinimizerD1[2];
  double chi2D1 = ComputeChi2(resultD1);
  cout << "D1 offset="<< resultD1[0]<<" zDUT="<<resultD1[1]<<" theta="<<resultD1[2]*180./TMath::Pi()<<" chi2="<<chi2D1<<endl;

  for (unsigned int i=0; i<selectedTk_d1_1Hit.size(); i++){
    double xDUT_d1 =   d1_DutXpos.at(i);
    double xTkAtDUT_d1 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d1_1Hit.at(i), aLparameteres().FEI4_z, resultD1[0], resultD1[1], resultD1[2]);
    double resDUT_d1 = xDUT_d1 - xTkAtDUT_d1;
    hist_->fillHist1D("TrackFit","d1_1tk1Hit_diffX_aligned", resDUT_d1);
  }


  doConstrainDeltaOffset = false;
  doD0 = true;
  doD1 = true;
  minimizerBothPlanes->Clear();
  minimizerBothPlanes->SetLimitedVariable(0, "offset_d0", offset_init_d0, 0.0001, -5., 5.);
  minimizerBothPlanes->SetLimitedVariable(1, "zDUT_d0", 435., 0.01, 200., 800.);
  minimizerBothPlanes->SetLimitedVariable(2, "offset_d1", offset_init_d1, 0.0001, -5., 5.);
  minimizerBothPlanes->SetLimitedVariable(3, "zDUT_d1", 435., 0.01, 200., 800.);
  minimizerBothPlanes->SetLimitedVariable(4, "theta", 0., 0.01, -90.*TMath::Pi()/180., 90.*TMath::Pi()/180.);

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

  for (unsigned int i=0; i<selectedTk_d1_1Hit.size(); i++){
    double xDUT_d0 =   bothPlanes_DutXposD0.at(i);
    double xTkAtDUT_d0 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), aLparameteres().FEI4_z, resultBothPlanes[0], resultBothPlanes[1], resultBothPlanes[4]);
    double resDUT_d0 = xDUT_d0 - xTkAtDUT_d0;
    hist_->fillHist1D("TrackFit","d0_1tk1ClusterBothPlanes_diffX_aligned", resDUT_d0);

    double xDUT_d1 =   bothPlanes_DutXposD1.at(i);
    double xTkAtDUT_d1 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), aLparameteres().FEI4_z, resultBothPlanes[2], resultBothPlanes[3], resultBothPlanes[4]);
    double resDUT_d1 = xDUT_d1 - xTkAtDUT_d1;
    hist_->fillHist1D("TrackFit","d1_1tk1ClusterBothPlanes_diffX_aligned", resDUT_d1);
  }


  doConstrainDeltaOffset = true;
  doD0 = true;
  doD1 = true;
  minimizerBothPlanesConstraint->Clear();
  minimizerBothPlanesConstraint->SetLimitedVariable(0, "offset_d0", offset_init_d0, 0.0001, -5., 5.);
  minimizerBothPlanesConstraint->SetLimitedVariable(1, "zDUT_d0", 435., 0.01, 200., 800.);
  minimizerBothPlanesConstraint->SetLimitedVariable(2, "deltaZ", 3., 0.01, 0., 8.);
  minimizerBothPlanesConstraint->SetLimitedVariable(3, "theta", TMath::ATan((offset_init_d1-offset_init_d0)/3.), 0.01, -20.*TMath::Pi()/180., 20.*TMath::Pi()/180.);

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
    std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), aLparameteres().FEI4_z, resultBothPlanesConstraint[0], resultBothPlanesConstraint[1], resultBothPlanesConstraint[2], resultBothPlanesConstraint[3]);
    double xTkAtDUT_d0 = xTkAtDUT.first;
    double xTkAtDUT_d1 = xTkAtDUT.second;
    double resDUT_d0 = xDUT_d0 - xTkAtDUT_d0;
    double resDUT_d1 = xDUT_d1 - xTkAtDUT_d1;
    hist_->fillHist1D("TrackFit","d0_1tk1ClusterBothPlanesConstraint_diffX_aligned", resDUT_d0);
    hist_->fillHist1D("TrackFit","d1_1tk1ClusterBothPlanesConstraint_diffX_aligned", resDUT_d1);
  }

/*
  cout << "------------------"<<endl;
  cout << "Alignment summary: "<<endl;
  cout << "D0 offset="<< resultD0[0]<<" zDUT="<<resultD0[1]<<" theta="<<resultD0[2]*180./TMath::Pi()<<" chi2="<<chi2D0<<endl;
  cout << "D1 offset="<< resultD1[0]<<" zDUT="<<resultD1[1]<<" theta="<<resultD1[2]*180./TMath::Pi()<<" chi2="<<chi2D1<<endl;
  cout << "BothPlanes offset_d0="<< resultBothPlanes[0]<<" zDUT_d0="<<resultBothPlanes[1]<<" offset_d1="<< resultBothPlanes[2]<<" zDUT_d1="<<resultBothPlanes[3] << " theta="<<resultBothPlanes[4]*180./TMath::Pi()<< " chi2="<<chi2BothPlanes<<endl;
  cout << "BothPlanesConstraint offset_d0="<< resultBothPlanesConstraint[0]<<" zDUT_d0="<<resultBothPlanesConstraint[1]<<" deltaZ="<< resultBothPlanesConstraint[2]<<" theta="<<resultBothPlanesConstraint[3]*180./TMath::Pi()<< " chi2="<<chi2BothPlanesConstraint<<endl;
*/

  std::ofstream fileAlignment;
  if(isProduction_) {
    fileAlignment.open(alignparFile_.c_str(), ios::out | ios::app);
  } else {
    fileAlignment.open(alignparFile_.c_str(), ios::out);
  }
  if(fileAlignment) {
    fileAlignment << "Run="<< runNumber_
                  << ":zD0="<< resultBothPlanesConstraint[1]
                  << ":offsetD0=" << resultBothPlanesConstraint[0]
                  << ":deltaZ=" << resultBothPlanesConstraint[2]
                  << ":angle=" << resultBothPlanesConstraint[3]*180./TMath::Pi() << endl;
    fileAlignment.close();
  } else std::cout << "Dump File could not be opened!!" << std::endl;


  //Fit Residuals Gaussian convulated with Step Function
  TF1* fGausResiduals = new TF1("fGausResiduals", "gaus", -10, 10);
  TH1* htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit","d0_1tk1Hit_diffX_aligned"));   
  float center = ((float)htmp->GetMaximumBin())*(htmp->GetXaxis()->GetXmax()-htmp->GetXaxis()->GetXmin())/((float)htmp->GetNbinsX()) + htmp->GetXaxis()->GetXmin();//htmp->GetMean();
  htmp->SetAxisRange(center-0.3, center+0.3, "X");
  fGausResiduals->SetRange(center-0.3, center+0.3);
  fGausResiduals->SetParameter(1, center);
  fGausResiduals->SetParameter(2, 0.026);
  htmp->Fit("fGausResiduals");
  
  center = fGausResiduals->GetParameter(1);
  float rms = 5*fGausResiduals->GetParameter(2);
  htmp->SetAxisRange(center-rms, center+rms, "X");
  
  TF1* fStepGaus = new TF1("FuncStepGaus", FuncStepGaus, -0.1, 0.1, 4);
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

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit","d1_1tk1Hit_diffX_aligned"));   
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

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit","d0_1tk1ClusterBothPlanes_diffX_aligned"));
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

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit","d1_1tk1ClusterBothPlanes_diffX_aligned"));
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

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit","d0_1tk1ClusterBothPlanesConstraint_diffX_aligned"));
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

  htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit","d1_1tk1ClusterBothPlanesConstraint_diffX_aligned"));
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

    doConstrainDeltaOffset = false; 
    doD0 = true;
    doD1 = true;
    TH1* hChi2vsTheta = dynamic_cast<TH1F*>(hist_->GetHistoByName("TrackFit","bothPlanes_chi2VsTheta"));
    for (int i=0; i<=40; i+=1){
      double theta_i = ((double)(i)-20.) * TMath::Pi()/180.;
      resultBothPlanes[4] = theta_i;
      double chi2_i = ComputeChi2BothPlanes(resultBothPlanes);
      cout << "theta_i="<<theta_i*180./TMath::Pi()<<" chi2_i="<<chi2_i <<endl;
      hChi2vsTheta->Fill(theta_i*180./TMath::Pi(), chi2_i);
    }

    doConstrainDeltaOffset = true;
    doD0 = true;
    doD1 = true;
    double theta_save = resultBothPlanesConstraint[3];
    hChi2vsTheta = dynamic_cast<TH1F*>(hist_->GetHistoByName("TrackFit","bothPlanesConstraint_chi2VsTheta"));
    for (int i=0; i<=40; i+=1){
      double theta_i = ((double)(i)-20.) * TMath::Pi()/180.;
      resultBothPlanesConstraint[3] = theta_i;
      double chi2_i = ComputeChi2BothPlanes(resultBothPlanesConstraint);
      cout << "theta_i="<<theta_i*180./TMath::Pi()<<" chi2_i="<<chi2_i <<endl;
      hChi2vsTheta->Fill(theta_i*180./TMath::Pi(), chi2_i);
    }
    resultBothPlanesConstraint[3] = theta_save;

    double deltaZ_save = resultBothPlanesConstraint[2];
    TH1* hChi2vsDeltaZ = dynamic_cast<TH1F*>(hist_->GetHistoByName("TrackFit","bothPlanesConstraint_chi2VsDeltaZ"));
    for (int i=0; i<=40; i+=1){
      double deltaZ_i = ((double)(i))/4.;
      resultBothPlanesConstraint[2] = deltaZ_i;
      double chi2_i = ComputeChi2BothPlanes(resultBothPlanesConstraint);
      cout << "deltaZ_i="<<deltaZ_i<<" chi2_i="<<chi2_i <<endl;
      hChi2vsDeltaZ->Fill(deltaZ_i, chi2_i);
    }
    resultBothPlanesConstraint[2] = deltaZ_save;

  }

  cout << "------------------"<<endl;
  cout << "Alignment summary: "<<endl;
  cout << "D0 offset="<< resultD0[0]<<" zDUT="<<resultD0[1]<<" theta="<<resultD0[2]*180./TMath::Pi()<<" chi2="<<chi2D0<<endl;
  cout << "D1 offset="<< resultD1[0]<<" zDUT="<<resultD1[1]<<" theta="<<resultD1[2]*180./TMath::Pi()<<" chi2="<<chi2D1<<endl;
  cout << "BothPlanes offset_d0="<< resultBothPlanes[0]<<" zDUT_d0="<<resultBothPlanes[1]<<" offset_d1="<< resultBothPlanes[2]<<" zDUT_d1="<<resultBothPlanes[3] << " theta="<<resultBothPlanes[4]*180./TMath::Pi()<< " chi2="<<chi2BothPlanes<<endl;
  cout << "BothPlanesConstraint offset_d0="<< resultBothPlanesConstraint[0]<<" zDUT_d0="<<resultBothPlanesConstraint[1]<<" deltaZ="<< resultBothPlanesConstraint[2]<<" theta="<<resultBothPlanesConstraint[3]*180./TMath::Pi()<< " chi2="<<chi2BothPlanesConstraint<<endl;


}

double AlignmentMultiDimAnalysis::ComputeChi2(const double* x) const{

  double chi2 = 0;
  double offset = x[0];
  double zDUT = x[1];
  double theta = x[2];

  double resTelescope = sqrt(0.090*0.090/12. + 0.0035*0.0035);

  unsigned int nEv = 0;
  if (doD0) nEv = selectedTk_d0_1Hit.size();
  if (doD1) nEv = selectedTk_d1_1Hit.size();

  double xTkAtDUT = 0;
  double xDUT = 0;
  double resDUT = 0;

  TH1* htmp;
  if (doD0) htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d0_1tk1Hit_diffX"));
  if (doD1) htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d1_1tk1Hit_diffX"));
  htmp->Reset();

  for (unsigned int i=0; i<nEv; i++){
    if (doD0){
      xDUT = d0_DutXpos.at(i);
      xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d0_1Hit.at(i), aLparameteres().FEI4_z, offset, zDUT, theta);
    }
    if (doD1){
      xDUT = d1_DutXpos.at(i);
      xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d1_1Hit.at(i), aLparameteres().FEI4_z, offset, zDUT, theta);
    }

    resDUT = xDUT - xTkAtDUT;
    if (doD0) hist_->fillHist1D("TrackFit","d0_1tk1Hit_diffX", resDUT);
    if (doD1) hist_->fillHist1D("TrackFit","d1_1tk1Hit_diffX", resDUT);

    chi2 += (resDUT/resTelescope)*(resDUT/resTelescope);
  }

  chi2 /= ((double)nEv);


  TF1* fGausExtractedX = new TF1("fGausExtractedX", Utility::FuncPol1Gaus, -10, 10, 5);

  if (doD0) htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d0_1tk1Hit_diffX"));
  if (doD1) htmp = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d1_1tk1Hit_diffX"));
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
      xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d0_1Hit.at(i), aLparameteres().FEI4_z, offset, zDUT, theta);
    }
    if (doD1){
      xDUT = d1_DutXpos.at(i);
      xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_d1_1Hit.at(i), aLparameteres().FEI4_z, offset, zDUT, theta);
    }

    resDUT = xDUT - xTkAtDUT;
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

  if (!doConstrainDeltaOffset){
    offset_d0 = x[0];
    zDUT_d0 = x[1];
    offset_d1 = x[2];
    zDUT_d1 = x[3];
    theta = x[4];
  }
  if (doConstrainDeltaOffset){
    offset_d0 = x[0];
    zDUT_d0 = x[1];
    deltaZ = x[2];
    theta = x[3];
    zDUT_d1 = zDUT_d0 + deltaZ;
  }

  double resTelescope = sqrt(0.090*0.090/12. + 0.0035*0.0035);

  unsigned int nEv = selectedTk_bothPlanes_1Cls.size();

  double xTkAtDUT_d0 = 0;
  double xDUT_d0 = 0;
  double resDUT_d0 = 0;
  double xTkAtDUT_d1 = 0;
  double xDUT_d1 = 0;
  double resDUT_d1 = 0;

  TH1* htmp0;
  TH1* htmp1;
  if (doD0) htmp0 = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d0_1tk1Hit_diffX"));
  if (doD1) htmp1 = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d1_1tk1Hit_diffX"));
  htmp0->Reset("ICESM");
  htmp1->Reset("ICESM");

  for (unsigned int i=0; i<nEv; i++){
    if (doD0 && doD1){
      xDUT_d0 = bothPlanes_DutXposD0.at(i);
      xDUT_d1 = bothPlanes_DutXposD1.at(i);
      if (!doConstrainDeltaOffset){
        xTkAtDUT_d0 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), aLparameteres().FEI4_z, offset_d0, zDUT_d0, theta);
        xTkAtDUT_d1 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), aLparameteres().FEI4_z, offset_d1, zDUT_d1, theta);
      }
      if (doConstrainDeltaOffset){
	std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), aLparameteres().FEI4_z, offset_d0, zDUT_d0, deltaZ, theta);
        xTkAtDUT_d0 = xTkAtDUT.first;
        xTkAtDUT_d1 = xTkAtDUT.second;
      }
      resDUT_d0 = xDUT_d0 - xTkAtDUT_d0;
      resDUT_d1 = xDUT_d1 - xTkAtDUT_d1;

      hist_->fillHist1D("TrackFit","d0_1tk1Hit_diffX", resDUT_d0);
      hist_->fillHist1D("TrackFit","d1_1tk1Hit_diffX", resDUT_d1);

     chi2 += ( (resDUT_d0/resTelescope)*(resDUT_d0/resTelescope) + (resDUT_d1/resTelescope)*(resDUT_d1/resTelescope) );
    }
  }

  chi2 /= ((double)nEv);


  TF1* fGausExtractedX = new TF1("fGausExtractedX", Utility::FuncPol1Gaus, -10, 10, 5);

  double xwindow = 2.;

  if (doD0) htmp0 = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d0_1tk1Hit_diffX"));
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
  if (!(center_err_d0>0) || !(height_d0>5.)) failedFit_d0 = true;
  if (failedFit_d0) {
    chi2=99999.;
      if (!doConstrainDeltaOffset) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" offset_d1=" << offset_d1<< " zDUT_d1="<< zDUT_d1<<" chi2="<<chi2<<" theta="<<theta*180./TMath::Pi()<<" Fit Failed !"<<endl;
      if (doConstrainDeltaOffset) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" deltaZ="<<deltaZ<<" theta="<<theta*180./TMath::Pi()<<" chi2="<<chi2<< " Fit Failed !"<<endl;
    return chi2;
  }

  //htmp0->SetAxisRange(center_d0-5*rms_d0, center_d0+5*rms_d0, "X");

  if (doD1) htmp1 = dynamic_cast<TH1I*>(hist_->GetHistoByName("TrackFit", "d1_1tk1Hit_diffX"));
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
  if (!(center_err_d1>0) || !(height_d1>5.)) failedFit_d1 = true;
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
        xTkAtDUT_d0 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), aLparameteres().FEI4_z, offset_d0, zDUT_d0, theta);
        xTkAtDUT_d1 = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), aLparameteres().FEI4_z, offset_d1, 
zDUT_d1, theta);
      }
      if (doConstrainDeltaOffset){
        std::pair<double, double> xTkAtDUT = Utility::extrapolateTrackAtDUTwithAngles(selectedTk_bothPlanes_1Cls.at(i), aLparameteres().FEI4_z, offset_d0, zDUT_d0, deltaZ, theta);
        xTkAtDUT_d0 = xTkAtDUT.first;
        xTkAtDUT_d1 = xTkAtDUT.second;
      }
      resDUT_d0 = xDUT_d0 - xTkAtDUT_d0;
      resDUT_d1 = xDUT_d1 - xTkAtDUT_d1;
    }

    if (fabs(resDUT_d0-center_d0)<3.*rms_d0 && fabs(resDUT_d1-center_d1)<3.*rms_d1) {
      chi2 += ( (resDUT_d0/resTelescope)*(resDUT_d0/resTelescope) + (resDUT_d1/resTelescope)*(resDUT_d1/resTelescope) );
      nEvWindow++;
    }
  }

  cout << "nEvWindow="<<nEvWindow<<endl;
  chi2 /= ((double)nEvWindow);

  if (!doConstrainDeltaOffset) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" offset_d1=" << offset_d1<< " zDUT_d1="<< zDUT_d1<<" theta="<<theta*180./TMath::Pi()<<" chi2="<<chi2<<endl;
  if (doConstrainDeltaOffset) cout << "offset_d0="<< offset_d0<<" zDUT_d0="<<zDUT_d0<<" deltaZ="<<deltaZ<<" theta="<<theta*180./TMath::Pi()<<" chi2="<<chi2<<endl; 

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
