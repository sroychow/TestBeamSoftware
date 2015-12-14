#include "PlotMakerBase.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "TROOT.h"
#include "argvparser.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace CommandLineProcessing;

int main( int argc,char* argv[] ){
  ArgvParser cmd;
  cmd.setIntroductoryDescription( "Offline Analysis PlotMaking Application" );
  cmd.addErrorCode( 0, "Success" );
  cmd.addErrorCode( 1, "Error" );
  cmd.defineOption( "iFile", "Input Data file name", ArgvParser::OptionRequiresValue);
  cmd.defineOptionAlternative( "iFile", "i" );
  cmd.defineOption( "oFile", "Output Root file name", ArgvParser::OptionRequiresValue);  
  cmd.defineOptionAlternative( "oFile", "o" );
  cmd.defineOption( "plotStub", "Scan stub parameter", ArgvParser::NoOptionAttribute);  
  cmd.defineOptionAlternative( "plotStub", "p" );
  cmd.defineOption( "det", "detector under study", ArgvParser::NoOptionAttribute);  
  cmd.defineOptionAlternative( "det", "d" );
  cmd.defineOption( "xTitle", "Scan Parameter", ArgvParser::OptionRequiresValue);  
  cmd.defineOptionAlternative( "xTitle", "x" );
  cmd.defineOption( "yTitle", "Variable under study", ArgvParser::OptionRequiresValue);  
  cmd.defineOptionAlternative( "yTitle", "y" );
  cmd.defineOption( "nBins", "Number of bins", ArgvParser::OptionRequiresValue);  
  cmd.defineOptionAlternative( "nBins", "b" );
  cmd.defineOption( "xmin", "Lower Range of x-Axis", ArgvParser::OptionRequiresValue);  
  //cmd.defineOptionAlternative( "xMinimum", "xmin" );
  cmd.defineOption( "xmax", "Upper Range of x-Axis", ArgvParser::OptionRequiresValue);  
  //cmd.defineOptionAlternative( "xMaximum", "xmax" );
  cmd.defineOption( "column", "Detector Column", ArgvParser::OptionRequiresValue);  
  cmd.defineOptionAlternative( "column", "c" );
  
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

  bool plotStub = ( cmd.foundOption( "plotStub" ) ) ? true:false;
  if( plotStub ) std::cout << "Stub specific plots requested!!" << std::endl;
  else std::cout << "Detector specific plots requested!!" << std::endl;
  std::string detector = ( cmd.foundOption( "det" ) ) ? cmd.optionValue( "det" ) : "";    
  if ( !plotStub && detector.empty() ) {
    std::cerr << "Error, specify detector name. Quitting" << std::endl;
    exit( 1 );
  }
  
  std::string xtit = ( cmd.foundOption( "xTitle" ) ) ? cmd.optionValue( "xTitle" ) : "";
  if ( xtit.empty() ) {
    std::cerr << "Error, no xtitle file provided. Quitting" << std::endl;
    exit( 1 );
  }

  std::string ytit = ( cmd.foundOption( "yTitle" ) ) ? cmd.optionValue( "yTitle" ) : "";
  if ( ytit.empty() ) {
    std::cerr << "Error, no ytitle file provided. Quitting" << std::endl;
    exit( 1 );
  }

  int nbin = ( cmd.foundOption( "nBins" ) ) ? std::stoi(cmd.optionValue( "nBins" ),nullptr,10) : 1001;

  float xmin = ( cmd.foundOption( "xmin" ) ) ? std::atof(cmd.optionValue( "xmin" ).c_str()) : -500.5;
  float xmax = ( cmd.foundOption( "xmax" ) ) ? std::atof(cmd.optionValue( "xmax" ).c_str()) : 500.5;

  std::string col = ( cmd.foundOption( "column" ) ) ? cmd.optionValue( "column" ) : "C0";

  //-i=AngularScan.dat -o=angularScan.root -p -xt=Angle -yt=StubEfficiency -nb=120 -xmin=-0.5 xmax=29.5 -c=C0
  if( plotStub )
    tBeamPlotMaker::plot_StubParameter_Scan(inFilename, xtit, ytit, nbin, xmin, xmax, col);
  else
    tBeamPlotMaker::plot_ClusterParameter_Scan(inFilename, xtit, ytit, nbin, xmin, xmax, detector, col);
  return 0;
}
