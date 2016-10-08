/*!
        \file                BeamAnaBase.cc
        \brief               Base Analysis class, reads input files, sets the hit vectors etc. Provides
                             access to hits, clusters, stubs, condition, telescope data. All individual applications
                             should inherit from this class.
        \author              Suvankar Roy Chowdhury
        \date                05/07/16
        Support :            mail to : suvankar.roy.chowdhury@cern.ch
*/

#include "BeamAnaBase.h"
#include "Utility.h"
#include "TSystem.h"
#include "TChain.h"
#include<algorithm>
#include <fstream>

BeamAnaBase::BeamAnaBase() :
  fin_(nullptr),
  analysisTree_(nullptr),
  dutEv_(new tbeam::dutEvent()),
  condEv_(new tbeam::condEvent()),
  telEv_(new  tbeam::TelescopeEvent()),
  fei4Ev_(new tbeam::FeIFourEvent()),
  periodcictyF_(false),
  isGood_(false),
  hasTelescope_(false),
  doTelMatching_(false),
  doChannelMasking_(false),  
  sw_(-1),
  offset1_(-1),
  offset2_(-1),
  cwd_(-1),
  dut0_chtempC0_(new std::vector<int>()),
  dut0_chtempC1_(new std::vector<int>()),
  dut1_chtempC0_(new std::vector<int>()),
  dut1_chtempC1_(new std::vector<int>()),
  dutRecoClmap_(new std::map<std::string,std::vector<tbeam::cluster>>),
  dutRecoStubmap_(new std::map<std::string,std::vector<tbeam::stub>>),
  recostubChipids_(new std::map<std::string,std::vector<unsigned int>>()),
  cbcstubChipids_(new std::map<std::string,std::vector<unsigned int>>()),
  dut_maskedChannels_(new std::map<std::string,std::vector<int>>()),
  nStubsrecoSword_(0),
  nStubscbcSword_(0)
{
  dutRecoClmap_->insert({("det0C0"),std::vector<tbeam::cluster>()});
  dutRecoClmap_->insert({("det0C1"),std::vector<tbeam::cluster>()});
  dutRecoClmap_->insert({("det1C0"),std::vector<tbeam::cluster>()});
  dutRecoClmap_->insert({("det1C1"),std::vector<tbeam::cluster>()});
  dutRecoStubmap_->insert({("C0"),std::vector<tbeam::stub>()});
  dutRecoStubmap_->insert({("C1"),std::vector<tbeam::stub>()});
  recostubChipids_->insert({("C0"),std::vector<unsigned int>()});
  recostubChipids_->insert({("C1"),std::vector<unsigned int>()});
  cbcstubChipids_->insert({("C0"),std::vector<unsigned int>()});
  cbcstubChipids_->insert({("C1"),std::vector<unsigned int>()});
}

bool BeamAnaBase::readJob(const std::string jfile) {
  std::ifstream jobcardFile(jfile.c_str());
  if (!jobcardFile) {
    std::cerr << "Input File: " << " could not be opened!" << std::endl;
    return false;
  }
  std::string line;
  if(jobcardFile.is_open()) {
    while(std::getline(jobcardFile,line)) {
      // enable '#' and '//' style comments
      if (line.substr(0,1) == "#" || line.substr(0,2) == "//") continue;
      std::vector<std::string> tokens;
      Utility::tokenize(line,tokens,"=");
      std::cout << tokens[0] << ":" << tokens[1] << std::endl;
      std::string key   = tokens.at(0);
      std::string value = tokens.at(1);
      jobCardmap_[key] = value;
      if(key=="inputFile")  iFilename_ = value;
      else if(key=="outputFile")  outFilename_ = value;
      else if(key=="fei4Z")  alPars_.FEI4_z = std::atof(value.c_str());
      else if(key=="d0minZ") alPars_.d0_chi2_min_z = std::atof(value.c_str());
      else if(key=="d1minZ") alPars_.d1_chi2_min_z = std::atof(value.c_str());
      else if(key=="d0Offsetaligned") alPars_.d0_Offset_aligned = std::atof(value.c_str()); 
      else if(key=="d1Offsetaligned") alPars_.d1_Offset_aligned = std::atof(value.c_str());
      else if(key=="doTelescopeMatching") doTelMatching_ = (atoi(value.c_str()) > 0) ? true : false;
      else if(key=="doChannelMasking") doChannelMasking_ = (atoi(value.c_str()) > 0) ? true : false;
      else if(key=="channelMaskFile")  chmaskFilename_ = value;
      else if(key=="residualSigmaDUT") residualSigmaDUT_ = std::atof(value.c_str());
      else if(key=="residualSigmaFEI4X") residualSigmaFEI4x_ = std::atof(value.c_str()); 
      else if(key=="residualSigmaFEI4Y") residualSigmaFEI4y_ = std::atof(value.c_str()); 
      else if(key=="offsetFEI4X") offsetFEI4x_ = std::atof(value.c_str()); 
      else if(key=="offsetFEI4Y") offsetFEI4y_ = std::atof(value.c_str()); 
      else if(key=="nStrips") nStrips_ = atoi(value.c_str());
      else if(key=="pitchDUT") pitchDUT_ = std::atof(value.c_str());
    }
  }
  jobcardFile.close();
  std::cout << "Initialized with the following options::"
            << "Infile: " << iFilename_
            << "\nOutFile: " << outFilename_
            << "\nFEI4Z:" << alPars_.FEI4_z
            << "\nd0_chi2_min_z:" << alPars_.d0_chi2_min_z
            << "\nd1_chi2_min_z:" << alPars_.d1_chi2_min_z
            << "\nd0_Offset_aligned:" << alPars_.d0_Offset_aligned
            << "\nd1_Offset_aligned:" << alPars_.d1_Offset_aligned
            << "\ndoTelescopeMatching:" << doTelMatching_
            << "\ndoChannelMasking:" << doChannelMasking_
            << "\nchannelMaskFile:" << chmaskFilename_
            << "\nresidualSigmaDUT:" << residualSigmaDUT_
            << "\nresidualSigmaFEI4y:" << residualSigmaFEI4y_
            << "\nresidualSigmaFEI4y:" << residualSigmaFEI4y_
            << "\nresidualSigmaFEI4y:" << offsetFEI4x_
            << "\nresidualSigmaFEI4y:" << offsetFEI4y_
            << "\nnStrips:" << nStrips_
            << "\npitchDUT:" << pitchDUT_
            << std::endl;
}

void BeamAnaBase::beginJob(){
  if( setInputFile(iFilename_) == 0 ) {
    std::cout << "Empty Chain!!";
    exit(1);
  }
  hout_ = new Histogrammer(outFilename_);
}
bool BeamAnaBase::setInputFile(const std::string& fname) {
  fin_ = TFile::Open(fname.c_str());
  if(!fin_)    {
    std::cout <<  "File " << fname << " could not be opened!!" << std::endl;
    return false;
  }
  analysisTree_ = dynamic_cast<TTree*>(fin_->Get("analysisTree"));
  if(analysisTree_)    return true;
  return false; 
}

void BeamAnaBase::setTelMatching(const bool mtel) {
  doTelMatching_ = mtel;
}

void BeamAnaBase::bookHistograms() {
  hout_->bookEventHistograms();
  hout_->bookDUTHistograms("det0");
  hout_->bookDUTHistograms("det1");
  hout_->bookStubHistograms();
  hout_->bookCorrelationHistograms();
}

void BeamAnaBase::fillCommonHistograms() {
      const auto& d0c1 = *det0C1();
      const auto& d1c0 = *det1C0();
      const auto& d1c1 = *det1C1();      
      //Fill histo for det0
      hout_->fillHist1D("det0","chsizeC0", dut0_chtempC0_->size());
      //hout_->fillHist1D("det0","chsizeC1", dut0_chtempC1_->size());
      hout_->fillHistofromVec(*dut0_chtempC0_,"det0","hitmapC0");
      //hout_->fillHistofromVec(dut0_chtempC1_,"det0","hitmapC1");
      hout_->fill2DHistofromVec(*dut0_chtempC0_,*dut0_chtempC1_,"det0","hitmapfull");
      hout_->fillClusterHistograms("det0",dutRecoClmap_->at("det0C0"),"C0");
      //hout_->fillClusterHistograms("det0",dutRecoClmap_->at("dut0_chtempC1_"),"C1");
      hout_->fillHist2D("det0","nhitvsnclusC0", dut0_chtempC0_->size(), dutRecoClmap_->at("det0C0").size());
      for(const auto& h: *dut0_chtempC0_) {
        int minposdiff = 255;
        for(const auto& cl:dutRecoClmap_->at("det0C0")) {
          if(std::abs(cl.x-h) < minposdiff)   minposdiff = std::abs(cl.x-h);
        }
        hout_->fillHist2D("det0","nhitvsHitClusPosDiffC0", dut0_chtempC0_->size(), minposdiff);
      }


      //Fill histo for det1
      hout_->fillHist1D("det1","chsizeC0", dut1_chtempC0_->size());
      //hout_->fillHist1D("det1","chsizeC1", dut1_chtempC1_->size());
      hout_->fillHistofromVec(*dut1_chtempC0_,"det1","hitmapC0");
      //hout_->fillHistofromVec(*dut1_chtempC1_,"det1","hitmapC1");
      hout_->fill2DHistofromVec(*dut1_chtempC0_,*dut1_chtempC1_,"det1","hitmapfull");
      hout_->fillClusterHistograms("det1",dutRecoClmap_->at("det1C0"),"C0");
      //hout_->fillClusterHistograms("det1",dutRecoClmap_->at("det1C1"),"C1");
      hout_->fillHist2D("det1","nhitvsnclusC0", dut1_chtempC0_->size(), dutRecoClmap_->at("det1C0").size());
      for(const auto& h: *dut1_chtempC0_) {
        int minposdiff = 255;
        for(const auto& cl:dutRecoClmap_->at("det1C0")) {
          if(std::abs(cl.x-h) < minposdiff)   minposdiff = std::abs(cl.x-h);
        }
        hout_->fillHist2D("det1","nhitvsHitClusPosDiffC0", dut1_chtempC0_->size(), minposdiff);
      }
      
      if(dut0_chtempC0_->size() && !dut1_chtempC0_->size()) hout_->fillHist1D("Correlation","cor_hitC0", 1);
      if(!dut0_chtempC0_->size() && dut1_chtempC0_->size()) hout_->fillHist1D("Correlation","cor_hitC0", 2);
      if(dut0_chtempC0_->size() && dut1_chtempC0_->size()) hout_->fillHist1D("Correlation","cor_hitC0", 3);
      if(!dut0_chtempC0_->size() && !dut1_chtempC0_->size()) hout_->fillHist1D("Correlation","cor_hitC0", 4);
      hout_->fillHist1D("Correlation","nclusterdiffC0", std::abs(dutRecoClmap_->at("det1C0").size() - 
                                                        dutRecoClmap_->at("det1C0").size())); 

      int totStubReco = dutEv_->stubs.size();
      int nstubrecoSword = nStubsrecoSword_;
      int nstubscbcSword = nStubscbcSword_;
      hout_->fillHist1D("StubInfo","nstubRecoC0", dutRecoStubmap_->at("C0").size());      
      hout_->fillHist1D("StubInfo","nstubsFromReco",totStubReco);
      hout_->fillHist1D("StubInfo","nstubsFromCBCSword",nstubrecoSword);
      hout_->fillHist1D("StubInfo","nstubsFromRecoSword",nstubscbcSword);
      for(auto& c : *recostubChipids_)  
        hout_->fillHistofromVec(c.second,"StubInfo","recoStubWord");
      for(auto& c : *cbcstubChipids_)  
        hout_->fillHistofromVec(c.second,"StubInfo","cbcStubWord");

      if (!nstubrecoSword && !nstubscbcSword) hout_->fillHist1D("StubInfo","stubMatch", 1);
      if (!nstubrecoSword && nstubscbcSword)  hout_->fillHist1D("StubInfo","stubMatch", 2);
      if (nstubrecoSword && !nstubscbcSword)  hout_->fillHist1D("StubInfo","stubMatch", 3);
      if (nstubrecoSword && nstubscbcSword)   hout_->fillHist1D("StubInfo","stubMatch", 4);
      hout_->fillHist1D("StubInfo","nstubsdiffSword",nstubrecoSword - nstubscbcSword);      
      hout_->fillHist1D("StubInfo","nstubsdiff",totStubReco - nstubscbcSword);  
}

void BeamAnaBase::setChannelMasking(const std::string cFile) {
  readChannelMaskData(cFile);
}
bool BeamAnaBase::branchFound(const string& b)
{
  TBranch* branch = analysisTree_->GetBranch(b.c_str());
  if (!branch) {
    cout << ">>> SetBranchAddress: <" << b << "> not found!" << endl;
    return false;
  }
  cout << ">>> SetBranchAddress: <" << b << "> found!" << endl;
  hasTelescope_ = true;
  return true;
}


void BeamAnaBase::setAddresses() {
  //set the address of the DUT tree
  if(branchFound("DUT"))    analysisTree_->SetBranchAddress("DUT", &dutEv_);
  if(branchFound("Condition"))    analysisTree_->SetBranchAddress("Condition", &condEv_);
  if(branchFound("TelescopeEvent"))    analysisTree_->SetBranchAddress("TelescopeEvent",&telEv_);
  if(branchFound("Fei4Event"))     analysisTree_->SetBranchAddress("Fei4Event",&fei4Ev_);
  if(branchFound("periodicityFlag"))    analysisTree_->SetBranchAddress("periodicityFlag",&periodcictyF_);
  if(branchFound("goodEventFlag"))    analysisTree_->SetBranchAddress("goodEventFlag",&isGood_);
  analysisTree_->SetBranchStatus("*",1);
}

void BeamAnaBase::setDetChannelVectors() {
  if(doChannelMasking_) {
    if( dutEv_->dut_channel.find("det0") != dutEv_->dut_channel.end() )
      Utility::getChannelMaskedHits(dutEv_->dut_channel.at("det0"), dut_maskedChannels_->at("det0")); 
    if( dutEv_->dut_channel.find("det1") != dutEv_->dut_channel.end() )
      Utility::getChannelMaskedHits(dutEv_->dut_channel.at("det1"), dut_maskedChannels_->at("det1")); 
    if( dutEv_->clusters.find("det1") != dutEv_->clusters.end() )  
      Utility::getChannelMaskedClusters(dutEv_->clusters.at("det0"), dut_maskedChannels_->at("det0"));
    if( dutEv_->clusters.find("det1") != dutEv_->clusters.end() )
      Utility::getChannelMaskedClusters(dutEv_->clusters.at("det1"), dut_maskedChannels_->at("det1"));
    //stub seeding layer os det1
    Utility::getChannelMaskedStubs(dutEv_->stubs,dut_maskedChannels_->at("det1"));
  }
  //std::cout << "setP1" << std::endl;
  if( dutEv_->dut_channel.find("det0") != dutEv_->dut_channel.end() ) {
      for( unsigned int j = 0; j<(dutEv_->dut_channel.at("det0")).size(); j++ ) {
        int ch = (dutEv_->dut_channel.at("det0")).at(j);
	if( ch <= 1015 )  dut0_chtempC0_->push_back(ch);
	else dut0_chtempC1_->push_back(ch-1016);
      }
  }
  if( dutEv_->dut_channel.find("det1") != dutEv_->dut_channel.end() ) {
      for( unsigned int j = 0; j<(dutEv_->dut_channel.at("det1")).size(); j++ ) {
        int ch = (dutEv_->dut_channel.at("det1")).at(j);
        if( ch <= 1015 )  dut1_chtempC0_->push_back(ch);
        else  dut1_chtempC1_->push_back(ch-1016);
      }
  }
  //std::cout << "setP2" << std::endl;
  for(auto& cl : (dutEv_->clusters)){
    std::string ckey = cl.first;//keys are det0 and det1
    for(auto& c : cl.second)  {
      if(c->x <= 1015)  dutRecoClmap_->at(ckey +"C0").push_back(*c);
      else {
        auto ctemp = *c;
        ctemp.x -= 1016;//even for column 1 we fill histograms between 0 and 1015 
        dutRecoClmap_->at( ckey + "C1").push_back(ctemp);
      }
    }    
  }
  //std::cout << "setP3" << std::endl;
  for(auto& s : dutEv_->stubs) {
    tbeam::stub st = *s;
    if(st.x <= 1015)   dutRecoStubmap_->at("C0").push_back(st);
    else dutRecoStubmap_->at("C1").push_back(st);
  }
  
  //for(auto& t:*recostubChipids_) std::cout << t.first << ",";
  //for(auto& t:*cbcstubChipids_) std::cout << t.first << ",";
  nStubsrecoSword_ = Utility::readStubWord(*recostubChipids_,dutEv_->stubWordReco);
  nStubscbcSword_ = Utility::readStubWord(*cbcstubChipids_,dutEv_->stubWord);
  //std::cout << "Leaving set" << std::endl;
}


void BeamAnaBase::getCbcConfig(uint32_t cwdWord, uint32_t windowWord){
  sw_ = windowWord >>4;
  offset1_ = (cwdWord)%4;
  if ((cwdWord>>2)%2) offset1_ = -offset1_;
  offset2_ = (cwdWord>>3)%4;
  if ((cwdWord>>5)%2) offset2_ = -offset2_;
  cwd_ = (cwdWord>>6)%4;
}

bool BeamAnaBase::isTrkfiducial(const double xtrkPos, int& xtkdutStrip, const std::string det) {
  xtkdutStrip = (xtrkPos/0.09) + 127;
  if(doChannelMasking_ && (xtkdutStrip > 254 ||  xtkdutStrip < 127))   return false; 
  //else if(xtkdutStrip > 185 ||  xtkdutStrip < 83)   return false; 
  if(doChannelMasking_) {
    return std::find(dut_maskedChannels_->at(det).begin(), 
                     dut_maskedChannels_->at(det).end(), 
                     xtkdutStrip) == dut_maskedChannels_->at(det).end();
    
  }
  return true;
}

void BeamAnaBase::getExtrapolatedTracks(std::vector<double>& xTkdut0, std::vector<double>& xTkdut1) {

  for(unsigned int itrk = 0; itrk<telEv_->nTrackParams;itrk++) {
    //do track fei4 matching
    double tkX = -1.*telEv()->xPos->at(itrk);
    double tkY = telEv()->yPos->at(itrk);
    bool match = false;
    double minresx = 999.;
    double minresy = 999.;
    for (unsigned int i = 0; i < fei4Ev()->nPixHits; i++) {   
      //default pitch and dimensions of fei4 plane
      double xval = -9.875 + (fei4Ev()->col->at(i)-1)*0.250;
      double yval = -8.375 + (fei4Ev()->row->at(i)-1)*0.05;
      double xres = xval - tkX + offsetFEI4x_;
      double yres = yval - tkY + offsetFEI4y_;
      if(std::fabs(xres) < std::fabs(minresx))   minresx = xres;
      if(std::fabs(yres) < std::fabs(minresy))   minresy = yres;
    }
    if(std::fabs(minresx) < 4*residualSigmaFEI4x_ && std::fabs(minresy) < 4*residualSigmaFEI4y_) {
      hout_->fillHist1D("TrackMatch","deltaXPos_trkfei4", minresx);
      hout_->fillHist1D("TrackMatch","deltaYPos_trkfei4", minresy);
      match = true;
    }
    if(!match)   continue;
    double XTkatDUT0_itrk = telEv_->yPos->at(itrk) + 
                            (alPars_.d0_chi2_min_z-alPars_.FEI4_z)*telEv_->dydz->at(itrk);
    XTkatDUT0_itrk = -1.*XTkatDUT0_itrk + alPars_.d0_Offset_aligned;
    double XTkatDUT1_itrk = telEv_->yPos->at(itrk) + 
                            (alPars_.d1_chi2_min_z-alPars_.FEI4_z)*telEv_->dydz->at(itrk);
    XTkatDUT1_itrk = -1.*XTkatDUT1_itrk + alPars_.d1_Offset_aligned;
    //check for duplicate tracks LOOSE MATCHING--We start with this.Then move to TIGHT CUT
    bool duplD0 = false, duplD1 = false;
    for(const auto& x0 : xTkdut0) {
      if(std::fabs(XTkatDUT0_itrk-x0) < 2*0.09) {
        duplD0=true;
        break;
      }
    }
    for(const auto& x1 : xTkdut1) {
      if(std::fabs(XTkatDUT1_itrk-x1) < 2*0.09) {
        duplD1=true;
        break;
      }
    }
    if(!duplD0 && !duplD1) {     
      xTkdut0.push_back(XTkatDUT0_itrk);
      xTkdut1.push_back(XTkatDUT1_itrk);
    }
  }


}

void BeamAnaBase::readChannelMaskData(const std::string cmaskF) {
  std::ifstream fin(cmaskF.c_str(),std::ios::in);
  if(!fin) {
    std::cout << "Channel Mask File could not be opened!!" << std::endl;
    return;
  }
  while(fin) {
    std::string line;
    std::getline(fin,line);
    //std::cout << "Line=" << line << ">>" << fin << std::endl;
    if(fin) {  
      if (line.substr(0,1) == "#" || line.substr(0,2) == "//") continue;
      std::vector<std::string> tokens;
      //first split against :
      Utility::tokenize(line,tokens,":");
      std::vector<std::string> maskedCh;
      //first split against , to get masked hits
      Utility::tokenize(tokens[1],maskedCh,",");
      int cbcid = std::atoi(tokens[0].c_str());
      for(auto& ch : maskedCh) {
        cbcMaskedChannelsMap_[cbcid].push_back(std::atoi(ch.c_str()));
      }
    }
  }
  fin.close();  
   
  std::cout << "Masked Channels List" << std::endl;
  dut_maskedChannels_->insert({("det0"),std::vector<int>()});
  dut_maskedChannels_->insert({("det1"),std::vector<int>()});
  for(auto& cbc : cbcMaskedChannelsMap_) {
    int cbcId = cbc.first;
    std::cout << "CBCid=" << cbcId << "  MaskedCh>>";
    int hitposX = -1;
    for(auto& ch : cbc.second) {
      std::cout << ch << ",";
      int ichan = ch / 2;
      if(cbcId <= 7) {
	hitposX = 127*cbcId + ichan;
      } else {
	  hitposX = 2032 - (127*cbcId + ichan);  
      }
      if( ch % 2 == 0 ) 
        for(int ic = hitposX-2; ic <= hitposX+2; ic++)
          dut_maskedChannels_->at("det1").push_back(ic);
      else 
        for(int ic = hitposX-2; ic <= hitposX+2; ic++)
          dut_maskedChannels_->at("det0").push_back(ic);
    }
    std::cout << std::endl;
  }
  
  std::cout << "Masked Channels Unfolded>>" << std::endl;
  for( auto& d : *dut_maskedChannels_) {
    std::cout << "DET=" << d.first << "  Masked Channels>>";
    for(auto& ch : d.second) 
      std::cout << ch << ",";
    std::cout << std::endl;
  } 
}

void readAlignmentConstant(const std::string& aFname) {
  std::ifstream fin(aFname.c_str(),std::ios::in);
  if(!fin) {
    std::cout << "Channel Mask File could not be opened!!" << std::endl;
    return;
  }
  fin.close();
}

void BeamAnaBase::endJob() {
  
}
void BeamAnaBase::clearEvent() {
  dut0_chtempC0_->clear();
  dut0_chtempC1_->clear();
  dut1_chtempC0_->clear();
  dut1_chtempC1_->clear();
  for(auto& c: *dutRecoClmap_)
    c.second.clear();
  for(auto& s: *dutRecoStubmap_)
    s.second.clear();
  for(auto& rs : *recostubChipids_)
    rs.second.clear();
  for(auto& rs : *cbcstubChipids_)
    rs.second.clear();
  nStubsrecoSword_ = 0;
  nStubscbcSword_ = 0;
}

BeamAnaBase::~BeamAnaBase() {
}
