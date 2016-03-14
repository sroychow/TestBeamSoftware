#ifndef DataFormats_h
#define DataFormats_h
#include<vector>
#include<map>
#include<string>
using std::vector;
using std::map;
using std::string;
namespace skbeam {
  class tBeamBase {
    public :
      UInt_t          run;
      UInt_t          lumiSection;
      UInt_t          event;
      Long64_t        time;
      Long64_t        unixtime;
      UInt_t          tdcPhase;
      UInt_t          hvSettings;
      UInt_t          dutAngle;
      UInt_t          stubWord;
      UInt_t          vcth;
      UInt_t          offset;
      UInt_t          window;
      UInt_t          cwd;
      UInt_t          tilt;
      Int_t           condData;
      Int_t           glibStatus;
      Int_t           cbc1Status;
      Int_t           cbc2Status;
      map<string,vector<int> > *dut_channel;
      map<string,vector<int> > *dut_row;
      tBeamBase(){}
      ~tBeamBase(){}
      void reset(){}
  };
  class telescopeBase {
    public :
      Int_t           nTrackParams;
      Int_t           euEvt;
      vector<double>  *xPos;
      vector<double>  *yPos;
      vector<double>  *dxdz;
      vector<double>  *dydz;
      vector<int>     *trackNum;
      vector<int>     *iden;
      vector<double>  *chi2;
      vector<double>  *ndof;
      telescopeBase(){}
      ~telescopeBase(){}
      void reset(){}
  };
  class trawTupleBase {
    public :
      Int_t           l1Accept;
      Int_t           tdcCounter;
      Int_t           totalHits;
      Int_t           totalStubs;
      vector<int>     *dut0Ch0data;
      vector<int>     *dut0Ch1data;
      vector<int>     *dut1Ch0data;
      vector<int>     *dut1Ch1data;
      trawTupleBase(){}
      ~trawTupleBase(){}
      void reset(){}
  };
}
#endif
