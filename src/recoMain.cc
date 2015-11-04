#include <iostream>
#include "TROOT.h"
#include "TStopwatch.h"
#include "Reconstruction.h"
#include "ReconstructionFromRaw.h"

using std::cout;
using std::cerr;
using std::endl;
int main( int argc,char* argv[] ){
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " InputFile   OutputFile  Type(0 for EDMtuple 1 for RawTuple)" << endl;
    exit(0);
  }    
  cout << "InFile:" << argv[1] << endl;
  cout << "OutFile:" << argv[2] << endl;
  bool isRaw = std::atoi(argv[3]) > 0 ? true : false;
  cout << "Analyse raw tuple?:" << isRaw << std::endl; 
  //Let's roll
  TStopwatch timer;
  timer.Start();
  if(!isRaw) {
    Reconstruction r(argv[1],argv[2]);
    std::cout << "Event Loop start" << std::endl;
    r.Loop();
    r.endJob();
  } else {
    ReconstructionFromRaw r(argv[1],argv[2]);
    std::cout << "Event Loop start" << std::endl;
    r.Loop();
    r.endJob();
  }
  timer.Stop();
  cout << "Realtime/CpuTime = " << timer.RealTime() << "/" << timer.CpuTime() << endl;
  return 0;
}
