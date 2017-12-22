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
#include <algorithm>
#include <stdlib.h>
int aa=5;
BeamAnaBase::BeamAnaBase() :
  fin_( nullptr ),
  analysisTree_(nullptr),
  event_( new tbeam::Event() ),
  hasTelescope_(false),
  doTelMatching_(false),
  doChannelMasking_(false) {
  vmod = new std::vector<tbeam::Module>();
}

bool BeamAnaBase::readJob(const std::string jfile) {
  std::ifstream jobcardFile(jfile.c_str());
  if (!jobcardFile) {
    std::cerr << "Input File: " << " could not be opened!" << std::endl;
    return false;
  }
  std::string line;
  std::string alignParfile;
  bool ralignmentFromfile;
  int run;
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
      else if(key=="Run")  run = atoi(value.c_str());
      else if(key=="fei4Z")  alPars_.FEI4z(std::atof(value.c_str()));
      else if(key=="readAlignmentFromfile")  ralignmentFromfile = (atoi(value.c_str()) > 0) ? true : false;
      else if(key=="alignmentOutputFile")  alignParfile = value;
      else if(key=="residualSigmaDUT")     residualSigmaDUT_ = std::atof(value.c_str());
      else if(key=="doTelescopeMatching") doTelMatching_ = (atoi(value.c_str()) > 0) ? true : false;
      else if(key=="doChannelMasking") doChannelMasking_ = (atoi(value.c_str()) > 0) ? true : false;
      else if(key=="channelMaskFile")  chmaskFilename_ = value;
      else if(key=="nStrips") nStrips_ = atoi(value.c_str());
      else if(key=="pitchDUT") pitchDUT_ = std::atof(value.c_str());
      else if(key=="maximumEVT") maxEvent_ = std::atoi(value.c_str());
      else if(key=="geomFile")  {
        readGeometry(value);
      }
    }
  }
  jobcardFile.close();
  std::cout << run << "::" << ralignmentFromfile << "::" << alignParfile << std::endl;
  if(ralignmentFromfile) {
    std::ifstream alf(alignParfile.c_str());
    if (!alf) {
      std::cerr << "Alignment File: " << alignParfile << " could not be opened!" << std::endl;
    }
    if(alf.is_open()) {
      while(std::getline(alf,line)) {
        // enable '#' and '//' style comments
        //if (line.substr(0,1) == "#" || line.substr(0,2) == "//") continue;
        std::cout << line << std::endl;
        std::vector<std::string> tokens;
        Utility::tokenize(line,tokens,":");
        std::cout << "Tokens size>>" << tokens.size() << std::endl;
        std::vector<std::string> rtemp;
        Utility::tokenize(tokens[0],rtemp,"=");
        std::cout << "Run number>>" << atoi(rtemp[1].c_str()) << std::endl;
        if(atoi(rtemp[1].c_str()) != run)  continue;
        for(unsigned int i = 1; i < tokens.size(); i++) {
          std::vector<std::string> vtemp;
          Utility::tokenize(tokens[i],vtemp,"=");
          std::string key = vtemp[0];
          std::string value = vtemp[1];
          std::cout << key << ":::" << value << std::endl;
          if(key=="offsetFEI4X") alPars_.offsetFEI4x(std::atof(value.c_str())); 
          else if(key=="offsetFEI4Y") alPars_.offsetFEI4y(std::atof(value.c_str())); 
          else if(key=="residualSigmaFEI4X") alPars_.residualSigmaFEI4x(std::atof(value.c_str())); 
          else if(key=="residualSigmaFEI4Y") alPars_.residualSigmaFEI4y(std::atof(value.c_str())); 
          else if(key=="zD0")  alPars_.d0Z(std::atof(value.c_str()));
          else if(key=="offsetD0")  alPars_.d0Offset(std::atof(value.c_str()));
          else if(key=="deltaZ")  alPars_.deltaZ(std::atof(value.c_str()));
          else if(key=="angle")  alPars_.theta(std::atof(value.c_str()));
        }        
      }
      alf.close();
    } else {
      std::cout << "Alignment File not found!!" << std::endl;
    }
    
  }
  alPars_.setD1parametersfromD0();
  std::cout << "Initialized with the following options::"
            << "Infile: " << iFilename_
            << "\nOutFile: " << outFilename_
            << "\ndoTelescopeMatching:" << doTelMatching_
            << "\ndoChannelMasking:" << doChannelMasking_
            << "\nchannelMaskFile:" << chmaskFilename_
            << "\nnStrips:" << nStrips_
            << "\npitchDUT:" << pitchDUT_
            << std::endl;
  std::cout << alPars_ << std::endl;
  if(doChannelMasking_)  setChannelMasking(chmaskFilename_);
}
bool BeamAnaBase::readGeometry(const std::string gfile) {
  std::string path = getenv ("WORKDIR");
  std::cout << "Reading geometry from file=" << (path +"/data/"+ gfile) << std::endl;
  std::ifstream geomFile((path +"/data/"+ gfile).c_str());
  json geom;
  geomFile >> geom;
  std::cout << std::setw(4) << geom << std::endl;
  // range-based for
  for (auto& element : geom["Module"]) {
    //std::cout << element << '\n';
    tbeam::Module tmod(element["name"], element["detidLower"], element["detidUpper"], element["Z"], element["Xrot"], element["Yrot"]);
    vmod->push_back(tmod);
  }
  geomFile.close();
  std::cout << "NModules from Geometry = " << vmod->size() << std::endl;
  
  return true;
}

void BeamAnaBase::beginJob(){
  if( setInputFile(iFilename_) == 0 ) {
    std::cout << "Empty Chain!!" << std::endl;
    exit(1);
  }
  hout_ = new Histogrammer(outFilename_);
  //  cout<<hout_->hfile()<<endl;
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
  /*********
	    Think of ways to do it automatically by reading a json/xml etc.
  *********/
  /*
  hout_->bookDUTHistograms("50025");
  hout_->bookDUTHistograms("50026");
  TString detid_lower = "50025";
  hout_->bookStubHistograms(detid_lower);*/
  //hout_->bookCorrelationHistograms();
  for(auto& m : *vmod) {
    hout_->bookDUTHistograms(m.hdirLower_);
    hout_->bookDUTHistograms(m.hdirUpper_);
    hout_->bookStubHistograms(m.name);
    hout_->bookCorrelationHistograms(m.name);
  }
  //book common histograms of track propoerties
  hout_->bookTrackCommonHistograms();
}


void BeamAnaBase::fillCommonHistograms() {
  //fill sensor hits, cluster
  /*
  for(auto& hitsmap: event_->dutHits){
    const std::string& detid = hitsmap.first;
    const std::vector<tbeam::hit>& hvec = hitsmap.second;
    hout_->fillHist1D(detid,"chsizeC0", hvec.size());
    for(auto& h: hvec)
      hout_->fillHist1D(detid ,"hitmapC0", h.strip());//put check to fill c1 histograms for full module
    const std::vector<tbeam::cluster>& cvec = event_->offlineClusters[detid];
    hout_->fillClusterHistograms(detid, cvec, "C0");
    hout_->fillHist2D(detid,"nhitvsnclusC0", hvec.size(), cvec.size());
  }*/
  for(auto& m : *vmod) {
    hout_->fillHist1D(m.hdirLower_,"chsizeC0", m.lowerHits.size());
    hout_->fillHist1D(m.hdirUpper_,"chsizeC0", m.upperHits.size());
    for(auto& h: m.lowerHits) {
      hout_->fillHist1D(m.hdirLower_ ,"hitmapC0", h.strip());//put check to fill c1 histograms for full module
      for(auto& hup: m.upperHits)  hout_->fillHist2D(m.name+"/Correlation", "hitposcorrelationC0", h.strip(), hup.strip());
    }
    for(auto& h: m.upperHits)
      hout_->fillHist1D(m.hdirUpper_ ,"hitmapC0", h.strip());//put check to fill c1 histograms for full module
    //Offline clusters only
    hout_->fillClusterHistograms(m.hdirLower_, m.lowerOfflineCls, "C0");
    hout_->fillHist2D(m.hdirLower_,"nhitvsnclusC0", m.lowerHits.size(), m.lowerOfflineCls.size());
    hout_->fillClusterHistograms(m.hdirUpper_, m.upperOfflineCls, "C0");
    hout_->fillHist2D(m.hdirUpper_,"nhitvsnclusC0", m.upperHits.size(), m.upperOfflineCls.size());
    //correlation histo for clusters
    for(auto& lcls : m.lowerOfflineCls) {
      for(auto& ucls : m.upperOfflineCls) {
         hout_->fillHist2D(m.name+"/Correlation", "clusterposcorrelationC0", lcls.center(), ucls.center());    
      }
    }
    //stub histos
    std::string sdname = m.name + "/StubInfo";
    hout_->fillHist1D(sdname,"nstubsFromCBC",  m.cbcStubs.size());
    hout_->fillHist1D(sdname,"nstubsFromReco", m.offlineStubs.size());
    hout_->fillHist2D(sdname,"nstubMatch", m.offlineStubs.size(), m.cbcStubs.size());
    for(auto& os : m.offlineStubs) {
      hout_->fillHist1D(sdname,"offlinestubPosmap", os.positionX());
      for(auto& cs : m.cbcStubs) {
        hout_->fillHist2D(sdname,"stubCorrelation", os.positionX(), cs.positionX());
      }
    }
    for(auto& cs : m.cbcStubs) {
      hout_->fillHist1D(sdname,"cbcstubPosmap", cs.positionX());
    }
  }
  //Fill common track histograms
  hout_->fillHist1D("TrackCommon","nTracks", event_->tracks.size());
  for(auto& tk: event_->tracks) {
    hout_->fillHist1D("TrackCommon","tkXPosref", tk.xPos());
    hout_->fillHist1D("TrackCommon","tkYPosref", tk.yPos());
    hout_->fillHist1D("TrackCommon","errtkXPosref", tk.xPosErr());
    hout_->fillHist1D("TrackCommon","errtkYPosref", tk.yPosErr());


    hout_->fillHist1D("TrackCommon","tkXPosprev", tk.xPosPrevHit());
    hout_->fillHist1D("TrackCommon","tkYPosprev", tk.yPosPrevHit());
    hout_->fillHist1D("TrackCommon","errtkXPosprev", tk.xPosErrsPrevHit());
    hout_->fillHist1D("TrackCommon","errtkYPosprev", tk.yPosErrsPrevHit());

    hout_->fillHist1D("TrackCommon","tkXPosnext", tk.xPosNextHit());
    hout_->fillHist1D("TrackCommon","tkYPosnext", tk.yPosNextHit());
    hout_->fillHist1D("TrackCommon","errtkXPosnext", tk.xPosErrNextHit());
    hout_->fillHist1D("TrackCommon","errtkYPosnext", tk.yPosErrNextHit());

    hout_->fillHist1D("TrackCommon","tkChi2", tk.chi2());
    hout_->fillHist1D("TrackCommon","tkdXdZ", tk.dxdz());
    hout_->fillHist1D("TrackCommon","tkdYdZ", tk.dydz());
  }
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
  if( branchFound("event") )    analysisTree_->SetBranchAddress("Event", &event_);
  analysisTree_->SetBranchStatus("*",1);
}

void BeamAnaBase::setDetChannelVectors() {
  for(auto& m : *vmod) {
    if(event_->dutHits.find(m.detidLower_) != event_->dutHits.end() && event_->dutHits.find(m.detidUpper_) != event_->dutHits.end())  {
      m.lowerHits = event_->dutHits.at(m.detidLower_);
      m.upperHits = event_->dutHits.at(m.detidUpper_);
    } 
    if(event_->cbcClusters.find(m.detidLower_) != event_->cbcClusters.end() && event_->cbcClusters.find(m.detidUpper_) != event_->cbcClusters.end())  {
      m.lowerCbcCls = event_->cbcClusters.at(m.detidLower_);//only in sparsified mode
      m.upperCbcCls = event_->cbcClusters.at(m.detidUpper_);//only in sparsified mode
    }
    if(event_->offlineClusters.find(m.detidLower_) != event_->offlineClusters.end() && event_->offlineClusters.find(m.detidUpper_) != event_->offlineClusters.end())  {
      m.lowerOfflineCls = event_->offlineClusters.at(m.detidLower_);//only in unsparsified mode
      m.upperOfflineCls = event_->offlineClusters.at(m.detidUpper_);//only in unsparsified mode
    }
    if(event_->cbcStubs.find(m.detidLower_) != event_->cbcStubs.end()) m.cbcStubs = event_->cbcStubs.at(m.detidLower_);//
    if(event_->offlineStubs.find(m.detidLower_) != event_->offlineStubs.end()) m.offlineStubs = event_->offlineStubs.at(m.detidLower_);
  }
}

//these should be available from Tracker header or Event
//void BeamAnaBase::getCbcConfig(uint32_t cwdWord, uint32_t windowWord){}
  //  sw_ = windowWord >>4;
  //  offset1_ = (cwdWord)%4;
  //  if ((cwdWord>>2)%2) offset1_ = -offset1_;
  //  offset2_ = (cwdWord>>3)%4;
  //  if ((cwdWord>>5)%2) offset2_ = -offset2_;
  //  cwd_ = (cwdWord>>6)%4;
//}

/*
bool BeamAnaBase::isTrkfiducial(const double xtrk0Pos, const double xtrk1Pos, const double ytrk0Pos, const double ytrk1Pos) {
  
  //DUT x acceptance
  if( (std::fabs(xtrk0Pos) > pitchDUT_*nStrips_/2.) 
      || (std::fabs(xtrk1Pos) > pitchDUT_*nStrips_/2.))  return false;
  //DUT y acceptance
  if(std::fabs(ytrk0Pos) > 25. || std::fabs(ytrk1Pos) > 25.)  return false; 
  /*think about this
    if(doChannelMasking_) {
    int xtkdutStrip0 = xtrk0Pos/pitchDUT_ + nStrips_/2; 
    int xtkdutStrip1 = xtrk1Pos/pitchDUT_ + nStrips_/2; 
    bool mtk = std::find(dut_maskedChannels_->at("det0").begin(), dut_maskedChannels_->at("det0").end(), xtkdutStrip0) == dut_maskedChannels_->at("det0").end();
    mtk = mtk && std::find( dut_maskedChannels_->at("det1").begin(), dut_maskedChannels_->at("det1").end(), xtkdutStrip1) == dut_maskedChannels_->at("det1").end();
    return mtk;
    }
  return true;
}
*/

//void BeamAnaBase::getExtrapolatedTracks(std::vector<tbeam::Track>&  fidTkColl) {}

  //Think how to implement track extrapolation 
  /*
  //Tk overlap removal
  //Match with FEI4
  //std::vector<tbeam::Track>  tracks;
  for(unsigned int itrk = 0; itrk<event_->tracks.size();itrk++) {
  tbeam::Track tk = event_->tracks[itrk];
  double YTkatDUT0_itrk = tk.yPos() + (alPars_.d0Z() - alPars_.FEI4z())*tk.dydz();
  double YTkatDUT1_itrk = tk.yPos() + (alPars_.d1Z() - alPars_.FEI4z())*tk.dydz();
  
  std::pair<double,double>  xtkdut = Utility::extrapolateTrackAtDUTwithAngles(tk, 
  alPars_.FEI4z(), alPars_.d0Offset(), alPars_.d0Z(), 
  alPars_.deltaZ(), alPars_.theta());
  //Selected tracks within DUT acceptance FEI4
  if(isTrkfiducial(xtkdut.first, xtkdut.second, YTkatDUT0_itrk, YTkatDUT1_itrk)) {
  selectedTk[itrk].xtkDut0 = xtkdut.first;
  selectedTk[itrk].xtkDut1 = xtkdut.second;
  selectedTk[itrk].ytkDut0 = YTkatDUT0_itrk;
  selectedTk[itrk].ytkDut1 = YTkatDUT1_itrk;
  //tbeam::Track temp(selectedTk[itrk]);
  fidTkColl.push_back(temp);
  } 
  }
  */
//}

void BeamAnaBase::readChannelMaskData(const std::string cmaskF) {

  /*
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
  */
}


  // nStubsrecoSword_ = 0;
  // nStubscbcSword_ = 0;
void readAlignmentConstant(const std::string& aFname) {
  std::ifstream fin(aFname.c_str(),std::ios::in);
  if(!fin) {
    std::cout << "Channel Mask File could not be opened!!" << std::endl;
    return;
  }
  fin.close();
}

void BeamAnaBase::endJob() {}

void BeamAnaBase::clearEvent() {}

BeamAnaBase::~BeamAnaBase() {
  delete vmod;
}
