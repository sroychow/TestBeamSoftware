#include <iostream>
#include <cstdlib>
#include <string>
#include "TROOT.h"
#include "TStopwatch.h"
#include "DeltaClusterAnalysis.h"
//#include "ReconstructionFromRaw.h"
#include "argvparser.h"
using std::cout;
using std::cerr;
using std::endl;

using namespace CommandLineProcessing;

int main( int argc,char* argv[] ){
   
  ArgvParser cmd;
  cmd.setIntroductoryDescription( "Offline Analysis Application for beam test data" );
  cmd.setHelpOption( "h", "help", "Print this help page" );
  cmd.addErrorCode( 0, "Success" );
  cmd.addErrorCode( 1, "Error" );
  cmd.defineOption( "iFile", "Input Tree name", ArgvParser::OptionRequiresValue);
  cmd.defineOption( "oFile", "Output file name", ArgvParser::OptionRequiresValue);  
  cmd.defineOption( "telM", "Do telescope matching. Default=false", ArgvParser::NoOptionAttribute);  
  cmd.defineOption( "chMaskF", "Channel Mask file;Ch masking off by default", ArgvParser::OptionRequiresValue);

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
  
  bool telmatch = ( cmd.foundOption( "telM" ) ) ? true : false;
  bool dochMask = ( cmd.foundOption( "chMaskF" ) ) ? true : false;
  std::string cMaskFilename = ( cmd.foundOption( "chMaskF" ) ) ? cmd.optionValue( "chMaskF" ) : "";

  //Let's roll
  TStopwatch timer;
  timer.Start();
  DeltaClusterAnalysis r(inFilename,outFilename);
  //r.setTelMatching(telmatch);
  //r.setChannelMasking(dochMask, cMaskFilename);
  std::cout << "Event Loop start" << std::endl;
  r.eventLoop();
  r.endJob();
  timer.Stop();
  cout << "Realtime/CpuTime = " << timer.RealTime() << "/" << timer.CpuTime() << endl;
  return 0;
}
