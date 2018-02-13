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
  event_( new tbeam::OfflineEvent() ),
  hasTelescope_(false),
  doTelMatching_(false),
  doChannelMasking_(false) {
  //vmod_ = new std::vector<tbeam::Module>();
  vmod_ =  std::unique_ptr<std::vector<tbeam::Module>>(new std::vector<tbeam::Module>());
  //telPlanes_ = new std::map<std::string, telescopePlane>();
  workDir_ = getenv ("SOURCE_DIR");
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
  if(ralignmentFromfile) 
    readAlignmentConstant(jobCardmap_.at("alignmentOutputFile"));

  std::cout << "Initialized with the following options::"
            << "Infile: " << iFilename_
            << "\nOutFile: " << outFilename_
            << "\ndoTelescopeMatching:" << doTelMatching_
            << "\ndoChannelMasking:" << doChannelMasking_
            << "\nchannelMaskFile:" << chmaskFilename_
            << "\nnStrips:" << nStrips_
            << "\npitchDUT:" << pitchDUT_
            << std::endl;
  if(doChannelMasking_)  setChannelMasking(chmaskFilename_);
}
bool BeamAnaBase::readGeometry(const std::string gfile) {

  std::cout << "Reading geometry from file=" << (workDir_ +"/data/"+ gfile) << std::endl;
  std::ifstream geomFile((workDir_ +"/data/"+ gfile).c_str());
  json geom;
  geomFile >> geom;
  std::cout << std::setw(4) << geom << std::endl;
  // look for modules
  for (auto& element : geom["Module"]) {
    //std::cout << element << '\n';
    tbeam::Module tmod(element["name"], element["detidbottom"], element["detidtop"],
                       element["Z"], element["Xrot"], element["Yrot"],
                       element["ncbc"], element["nstrips"], element["pitch"]);
    std::cout << "READ Module with #strips=" << tmod.nstrips_ << std::endl;
    vmod_->push_back(tmod);
  }
  // look for Telescope planes
  telPlaneprev_ = {"Previous", -999., -999., -999.};
  telPlanenext_ = {"Next", -999., -999., -999.};
  for (auto& element : geom["Telescope"]) {
    if(element["name"] == "Previous")
      telPlaneprev_ = {element["name"], element["Z"], element["Xrot"], element["Yrot"]};
    else if (element["name"] == "Next")
      telPlanenext_ = {element["name"], element["Z"], element["Xrot"], element["Yrot"]};
   }

  geomFile.close();
  std::cout << "NModules from Geometry = " << vmod_->size() << std::endl;
  std::cout << "Previous plane at Z=" << telPlaneprev_.z << " cm" << std::endl;
  std::cout << "Next plane at Z="     << telPlanenext_.z << " cm" << std::endl;
  return true;
}

void BeamAnaBase::beginJob(){
  if( setInputFile(iFilename_) == 0 ) {
    std::cout << "Empty Chain!!" << std::endl;
    exit(1);
  }
  hist_ = new Histogrammer(outFilename_);
  //  cout<<hist_->hfile()<<endl;
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
  hist_->bookEventHistograms();
  for(auto& m : *modVec()){
    hist_->bookDUTHistograms(m.hdirbottom_);
    hist_->bookDUTHistograms(m.hdirtop_);
    hist_->bookStubHistograms(m.name);
    hist_->bookCorrelationHistograms(m.name);
  }
  //book common histograms of track propoerties
  hist_->bookTrackCommonHistograms();
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
  for(auto& m : *vmod_) {
    if(event_->dutHits.find(m.detidbottom_) != event_->dutHits.end() && event_->dutHits.find(m.detidtop_) != event_->dutHits.end())  {
      m.bottomHits = event_->dutHits.at(m.detidbottom_);
      m.topHits = event_->dutHits.at(m.detidtop_);
    }
    if(event_->cbcClusters.find(m.detidbottom_) != event_->cbcClusters.end() && event_->cbcClusters.find(m.detidtop_) != event_->cbcClusters.end())  {
      m.bottomCbcCls = event_->cbcClusters.at(m.detidbottom_);//only in sparsified mode
      m.topCbcCls = event_->cbcClusters.at(m.detidtop_);//only in sparsified mode
    }
    if(event_->offlineClusters.find(m.detidbottom_) != event_->offlineClusters.end() && event_->offlineClusters.find(m.detidtop_) != event_->offlineClusters.end())  {
      m.bottomOfflineCls = event_->offlineClusters.at(m.detidbottom_);//only in unsparsified mode
      m.topOfflineCls = event_->offlineClusters.at(m.detidtop_);//only in unsparsified mode
    }
    if(event_->cbcStubs.find(m.detidbottom_) != event_->cbcStubs.end()) m.cbcStubs = event_->cbcStubs.at(m.detidbottom_);//
    if(event_->offlineStubs.find(m.detidbottom_) != event_->offlineStubs.end()) m.offlineStubs = event_->offlineStubs.at(m.detidbottom_);
    //std::cout << "Set Module with #strips=" << m.nstrips_ << std::endl;
  }

}

void BeamAnaBase::fillCommonHistograms() {
  //fill sensor hits, cluster
  for(auto& m : *modVec()) {
    //Fill hit size
    hist_->fillHist1D(m.hdirbottom_,"chsizeC0", m.bottomHits.size());
    hist_->fillHist1D(m.hdirtop_,"chsizeC0", m.topHits.size());
    //Fill bottom sensor hit info
    for(auto& h: m.bottomHits) {
      hist_->fillHist1D(m.hdirbottom_ ,"hitmapC0", h.strip());//put check to fill c1 histograms for full module
      hist_->fillHist1D(m.hdirbottom_ ,"hitmapXposC0", (h.strip() - m.nstrips_/2.)*m.pitch_);
      //std::cout << "Hit (bottom) =" << h.strip() << " in mm=" << (float(h.strip()) - 127.)*0.09 << "\n";
      for(auto& hup: m.topHits)  hist_->fillHist2D(m.name+"/Correlation", "hitposcorrelationC0", h.strip(), hup.strip());
    }
    //Fill top sensor hit info
    for(auto& h: m.topHits) {
      hist_->fillHist1D(m.hdirtop_ ,"hitmapC0", h.strip());//put check to fill c1 histograms for full module
      //std::cout << "Hit (top) =" << h.strip() << " in mm=" << (float(h.strip()) - 127)*0.09<< "\n";
    }
    //Offline clusters only
    //bottom hits
    hist_->fillHist1D( m.hdirbottom_, "nclusterC0", m.bottomOfflineCls.size() );
    for( auto& cl : m.bottomOfflineCls) {
      hist_->fillHist1D( m.hdirbottom_, "clusterWidthC0", cl.size() );
      hist_->fillHist1D( m.hdirbottom_, "clusterPosC0", cl.center() );
      hist_->fillHist1D( m.hdirbottom_, "clusterXPosC0", (cl.center() - m.nstrips_/2.)*m.pitch_ );
      hist_->fillHist1D( m.hdirbottom_, "clusterWidthVsPosProfC0", cl.center(), cl.size() );
      hist_->fillHist2D( m.hdirbottom_, "clusterWidthVsPos2DC0", cl.center(), cl.size() );
    }
    //top hits
    hist_->fillHist1D( m.hdirtop_, "nclusterC0", m.topOfflineCls.size() );
    for( auto& cl : m.topOfflineCls) {
      hist_->fillHist1D( m.hdirtop_, "clusterWidthC0", cl.size() );
      hist_->fillHist1D( m.hdirtop_, "clusterPosC0", cl.center() );
      hist_->fillHist1D( m.hdirtop_, "clusterXPosC0", (cl.center() - m.nstrips_/2.)*m.pitch_ );
      hist_->fillHist1D( m.hdirtop_, "clusterWidthVsPosProfC0", cl.center(), cl.size() );
      hist_->fillHist2D( m.hdirtop_, "clusterWidthVsPos2DC0", cl.center(), cl.size() );
    }
    hist_->fillHist2D(m.hdirbottom_,"nhitvsnclusC0", m.bottomHits.size(), m.bottomOfflineCls.size());
    hist_->fillHist2D(m.hdirtop_,"nhitvsnclusC0", m.topHits.size(), m.topOfflineCls.size());
    //correlation histo for clusters
    for(auto& lcls : m.bottomOfflineCls) {
        //std::cout << "Offline clus pos(bottom) =" << lcls.center() << " in mm=" << (lcls.center() - 127)*0.09<< "\n";
      for(auto& ucls : m.topOfflineCls) {
         //std::cout << "Offline clus pos(top) =" << ucls.center() << " in mm=" << (ucls.center() - 127)*0.09 << "\n";
         hist_->fillHist2D(m.name+"/Correlation", "clusterposcorrelationC0", lcls.center(), ucls.center());
      }
    }
    //Fill stub histos
    std::string sdname = m.name + "/StubInfo";
    hist_->fillHist1D(sdname,"nstubsFromCBC",  m.cbcStubs.size());
    hist_->fillHist1D(sdname,"nstubsFromReco", m.offlineStubs.size());
    hist_->fillHist2D(sdname,"nstubMatch", m.offlineStubs.size(), m.cbcStubs.size());
    for(auto& os : m.offlineStubs) {
      hist_->fillHist1D(sdname,"offlinestubPosmap", os.positionX());
      //std::cout << "Offline stub pos =" << os.positionX() << " in mm=" << (os.positionX() - 127.)*0.09<< "\n";
      for(auto& cs : m.cbcStubs) {
        hist_->fillHist2D(sdname,"stubCorrelation", os.positionX(), cs.positionX());
      }
    }
    for(auto& cs : m.cbcStubs) {
      hist_->fillHist1D(sdname,"cbcstubPosmap", cs.positionX());
    }
  }
  //Fill common track histograms
  hist_->fillHist1D("TrackCommon","nTracks", event()->tracks.size());
  for(auto& tk: event()->tracks) {
    //std::cout << "XPos tk=" << tk.xPos() << std::endl;
    hist_->fillHist1D("TrackCommon","tkXPosref", tk.xPos());
    hist_->fillHist1D("TrackCommon","tkYPosref", tk.yPos());
    hist_->fillHist1D("TrackCommon","errtkXPosref", tk.xPosErr());
    hist_->fillHist1D("TrackCommon","errtkYPosref", tk.yPosErr());


    hist_->fillHist1D("TrackCommon","tkXPosprev", tk.xPosPrevHit());
    hist_->fillHist1D("TrackCommon","tkYPosprev", tk.yPosPrevHit());
    hist_->fillHist1D("TrackCommon","errtkXPosprev", tk.xPosErrsPrevHit());
    hist_->fillHist1D("TrackCommon","errtkYPosprev", tk.yPosErrsPrevHit());

    hist_->fillHist1D("TrackCommon","tkXPosnext", tk.xPosNextHit());
    hist_->fillHist1D("TrackCommon","tkYPosnext", tk.yPosNextHit());
    hist_->fillHist1D("TrackCommon","errtkXPosnext", tk.xPosErrNextHit());
    hist_->fillHist1D("TrackCommon","errtkYPosnext", tk.yPosErrNextHit());

    hist_->fillHist1D("TrackCommon","tkChi2", tk.chi2());
    hist_->fillHist1D("TrackCommon","tkdXdZ", tk.dxdz());
    hist_->fillHist1D("TrackCommon","tkdYdZ", tk.dydz());
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

//void BeamAnaBase::getExtrapolatedTracks(std::vector<tbeam::OfflineTrack>&  fidTkColl) {}

  //Think how to implement track extrapolation
  /*
  //Tk overlap removal
  //Match with FEI4
  //std::vector<tbeam::OfflineTrack>  tracks;
  for(unsigned int itrk = 0; itrk<event_->tracks.size();itrk++) {
  tbeam::OfflineTrack tk = event_->tracks[itrk];
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
  //tbeam::OfflineTrack temp(selectedTk[itrk]);
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
  // nStubscbcSword_ = 0;\

void BeamAnaBase::readAlignmentConstant(const std::string& aFname) {
  std::string alignparFile_ = workDir_ + "/data/" + aFname;
  std::cout << "Alignment parameters will be read from file:" << alignparFile_ << std::endl;
  std::ifstream fin(alignparFile_.c_str());
  if(!fin) {
    std::cout << "Alignment File could not be opened!! All alignment parameters will be set to ZERO!!" << std::endl;
    alPars_["offset_d0"]   =    0.;
    alPars_["zDUT_d0"]     =    0.;
    alPars_["deltaZ"]      =    0.;
    alPars_["theta"]       =    0.;
    alPars_["shiftPlanes"] =    0.;
    alPars_["phi_d0"]      =    0.;
    alPars_["phi_d1"]      =    0.;
    return;
  }
  json alignment;
  fin >> alignment;
  alPars_["offset_d0"]   =    alignment["offset_d0"];
  alPars_["zDUT_d0"]     =    alignment["zDUT_d0"];
  alPars_["deltaZ"]      =    alignment["deltaZ"];
  alPars_["theta"]       =    alignment["theta"];
  alPars_["shiftPlanes"] =    alignment["shiftPlanes"];
  alPars_["phi_d0"] =    alignment["phi_d0"];
  alPars_["phi_d1"] =    alignment["phi_d1"];
  fin.close();
  std::cout << "Alignment parameters read from file:" << alignparFile_ << std::endl;
  for(auto& ap : alPars_)
    std::cout << ap.first << ":" <<  ap.second << std::endl;

}

void BeamAnaBase::endJob() {}

void BeamAnaBase::clearEvent() {}

BeamAnaBase::~BeamAnaBase() {
  //delete vmod_;
}
