#include <iostream>
#include <cstdlib>
#include <string>
#include "TROOT.h"
#include "TStopwatch.h"
#include "Reconstruction.h"
#include "ReconstructionFromRaw.h"
#include "argvparser.h"
using std::cout;
using std::cerr;
using std::endl;

using namespace CommandLineProcessing;

int main( int argc,char* argv[] ){
   
  ArgvParser cmd;
  cmd.setIntroductoryDescription( "Offline Analysis Application fro beam test data" );
  cmd.addErrorCode( 0, "Success" );
  cmd.addErrorCode( 1, "Error" );
  cmd.defineOption( "iFile", "Input RawTuple name", ArgvParser::OptionRequiresValue);
  cmd.defineOption( "oFile", "Output file name", ArgvParser::OptionRequiresValue);  
  cmd.defineOption( "raw", "Tuple type is Raw. Default is EDM", ArgvParser::NoOptionAttribute);  
  cmd.defineOption( "sw", "Stub Window. Default = 7", ArgvParser::NoOptionAttribute);
  cmd.defineOption( "png", "Produce png files for web. Default = false", ArgvParser::NoOptionAttribute); 
  int result = cmd.parse( argc, argv );
  if (result != ArgvParser::NoParserError)
  {
    cout << cmd.parseErrorDescription(result);
    exit(1);
  }

  std::string inFilename = ( cmd.foundOption( "iFile" ) ) ? cmd.optionValue( "iFile" ) : "";
  
  if ( inFilename.empty() ) {
    std::cerr << "Error, no input file provided. Quitting" << std::endl;
    exit( 1 );
  }
  std::string outFilename = ( cmd.foundOption( "oFile" ) ) ? cmd.optionValue( "oFile" ) : "";
  if ( outFilename.empty() ) {
    std::cerr << "Error, no output filename provided. Quitting" << std::endl;
    exit( 1 );
  }
  bool isRaw = ( cmd.foundOption( "raw" ) ) ? true:false;
  int stubWindow = ( cmd.foundOption( "sw" ) ) ? std::stoi(cmd.optionValue( "sw" ),nullptr,10) : 7;
  bool publishPng = ( cmd.foundOption( "png" ) ) ? true : false;
  
  std::cout << "isRaw=" << isRaw << std::endl;  
  //Let's roll
  TStopwatch timer;
  timer.Start();
  if(!isRaw) {
    Reconstruction r(inFilename,outFilename,stubWindow,publishPng);
    std::cout << "Event Loop start" << std::endl;
    r.Loop();
    r.endJob();
  } else {
    ReconstructionFromRaw r(inFilename,outFilename,stubWindow,publishPng);
    std::cout << "Event Loop start" << std::endl;
    r.Loop();
    r.endJob();
  }
  timer.Stop();
  cout << "Realtime/CpuTime = " << timer.RealTime() << "/" << timer.CpuTime() << endl;
  return 0;
}
