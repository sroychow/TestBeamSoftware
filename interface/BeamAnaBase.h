#ifndef BeamAnaBase_h
#define BeamAnaBase_h

//#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include "TSystem.h"

#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
#include<stdint.h>
#include <map>
#include <string>
#include <fstream>
#include "json.hpp"

//#include "DataFormats.h"
#include "Histogrammer.h"
#include "Event.h"
#include "AlignmentParameters.h"
#include "Module.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::cerr;
using json = nlohmann::json;

struct telescopePlane {
  std::string name;
  float z;//in cm
  float xrot;
  float yrot;
};
static constexpr unsigned int MASK_BITS_8 = 0xFF;
static constexpr unsigned int MASK_BITS_4 = 0xF;
class BeamAnaBase {

  public :
    BeamAnaBase();
    virtual ~BeamAnaBase();

    bool setInputFile(const std::string& fname);
    bool branchFound(const string& b);
    void setAddresses();
    void setDetChannelVectors();

    TTree* analysisTree() const{ return analysisTree_; }
    tbeam::OfflineEvent* event() const { return event_; }

    //int stubWindow()  const { return sw_;}
    //int cbcClusterWidth()  const { return cwd_;}
    //int cbcOffset1() const {return offset1_;}
    //int cbcOffset2() const {return offset2_;}


    bool hasTelescope() const { return hasTelescope_; }
    double resDUT() const { return residualSigmaDUT_; }
    double nstrips() const { return nStrips_; }
    double dutpitch() const { return pitchDUT_; }

    std::map<std::string, double> alignmentPars() const { return alPars_; }
    double offsetbottom()    const { return alPars_.at("offset_d0") ; }
    double zDUTbottom()      const { return alPars_.at("zDUT_d0")  ; }
    double shiftPlanes()     const { return alPars_.at("shiftPlanes"); }
    double dutangle()        const { return alPars_.at("theta") ; }
    double sensordeltaZ()    const { return alPars_.at("deltaZ"); }
    double bottomsensorPhi() const { return alPars_.at("phi_d0"); }
    double topsensorPhi()    const { return alPars_.at("phi_d1"); }

    virtual void beginJob();
    virtual void endJob();
    virtual void eventLoop() = 0;
    virtual void bookHistograms();
    virtual void clearEvent();
    virtual bool readJob(const std::string jfile);
    virtual bool readGeometry(const std::string gfile);

    void getCbcConfig(uint32_t cwdWord, uint32_t windowWord);
    //void getExtrapolatedTracks(std::vector<tbeam::OfflineTrack>& fidTkColl);
    void readChannelMaskData(const std::string cmaskF);
    void setTelMatching(const bool mtel);
    void setChannelMasking(const std::string cFile);
    bool doTelMatching() const { return doTelMatching_;}
    bool doChannelMasking() const { return doChannelMasking_;}

    //std::map<std::string,std::vector<int> >* getMaskedChannelMap() const {return dut_maskedChannels_;}
    void readAlignmentConstant(const std::string& aFname);

    //bool isTrkfiducial(const double xtrk0Pos, const double xtrk1Pos, const double ytrk0Pos, const double ytrk1Pos);
    Histogrammer* outFile() { return hist_; }

    void fillCommonHistograms();
    std::map<std::string,std::string> jobCardmap() const { return jobCardmap_;}
    std::string inFile() { return iFilename_; }
    unsigned long int getMaxEvt() { return maxEvent_; }
    std::unique_ptr<std::vector<tbeam::Module>>& modVec() { return vmod_; }
    std::string workDir() const { return workDir_; }
    /*
      bool CheckFiducial( double xTrk , int pStripMin , int pStripMax ,  const std::string det)
      {
      int iStrip = xTrk/dutpitch() + nstrips()/2.0;
      bool isFiducial = true;
      if( iStrip > nstrips() || iStrip < pStripMin || iStrip > pStripMax ) isFiducial = false;
      //if( iStrip > nstrips() ) isFiducial = false;
      if( doChannelMasking_ ) isFiducial =  std::find(dut_maskedChannels_->at(det).begin(), dut_maskedChannels_->at(det).end(), iStrip) ==    dut_maskedChannels_->at(det).end();
      return isFiducial;
      }*/

 private :
    std::string iFilename_;
    std::string outFilename_;
    std::string chmaskFilename_;
    TFile* fin_;
    TTree *analysisTree_;
    tbeam::OfflineEvent* event_;

    bool hasTelescope_;
    bool doTelMatching_;
    bool doChannelMasking_;
    unsigned long int maxEvent_=0;

    std::map<string,double>  alPars_;
    std::map<std::string,std::string> jobCardmap_;
    std::map<std::string,std::vector<int> >* dut_maskedChannels_;

    Histogrammer* hist_;
    double residualSigmaDUT_;
    double residualSigmaFEI4x_;
    double residualSigmaFEI4y_;
    double offsetFEI4x_;
    double offsetFEI4y_;

    int nStrips_;
    double pitchDUT_;
    std::string workDir_;
  protected:
    std::unique_ptr<std::vector<tbeam::Module>> vmod_;
    //telescopePlane telPlaneref_;
    telescopePlane telPlaneprev_;
    telescopePlane telPlanenext_;
};
#endif
