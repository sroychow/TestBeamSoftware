## Beam Test Data Analysis Software
##FOR NOV17 FNAL testbeam
## Set up gcc and ROOT(works with ROOT 6, requires ROOT to be compiled with libminut2)
##On lxplus: you can setup gcc and root as follows
cmsrel CMSSW_9_3_0

cd CMSSW_9_3_0/src

cmsenv

This will set the required gcc and ROOT required.

##Fetch the software and compile
git clone https://github.com/sroychow/TestBeamSoftware.git

cd TestBeamSoftware

git checkout nov17

setenv  SOURCE_DIR $PWD

make cint

make

**Set SOURCE_DIR environment variable:Should point to the directory where TestBeamSoftware 
is checked out. This must be set evrtytime a new shell is opened. 

**For mac users with Xcode 9, change the compiler in MakeFile to clag++ instead of g++

##Basic usage :-
./<application name> <jobcard-name>

## Usage
##Running on tuple

#Step1: Alignment Analysis

./alignmentReco \<jobcardName\>

Apart from a root file with all the histograms, the application also produces a text file with all the alignment parameters.*** 
This application can be run in "production" or "new" mode(can be set in the job card). If you run in production mode, the 
alignment output parameter file will be opened in append mode. It can be used for analysing many runs and dunping the 
output in the same file. In the "new" mode, a new alignment output file will be created and if a file exists with the same name,
 it will be overwritten.

#JobCard Description

Only the parameters required for this application are described.

Run=\<RUN-NUMBER\>

inputFile=AnalysisTree_\<RUN-NUMBER\>.root

outputFile=\<output-file name\>

fei4Z=\<FEI4-distance\>

doTelescopeMatching=1 #set to true; will look for telescope data; if =0, this step will not work

doChannelMasking=0 #=0 if no channel masked; =1 if channel masking required

channelMaskFile=\<filename\> #set name of file with channel mask; required if doChannelMasking=1 

nStrips=254 # No. of strips

pitchDUT=0.09 # Pitch

alignmentOutputFile=\<filename\> #Filename where the alignment parameters will be written

isProductionmode=0 #if =0 new mode; =1 append mode 
#Step2: Baseline Analysis to study detector performance

Only the parameters required for this application are described.

Run=\<RUN-NUMBER\>

inputFile=AnalysisTree_\<RUN-NUMBER\>.root

outputFile=\<output-file name\>

fei4Z=\<FEI4-distance\>

readAlignmentFromfile=1 #if=1, alignment parameters will be read from alignmentOutputFile; if=0, telescope matching will not work(or you have to hardcode values in your code! Not recommended!!)

doTelescopeMatching=1 #set to true; will look for telescope data; if =0, detector only performance

doChannelMasking=0 #=0 if no channel masked; =1 if channel masking required

channelMaskFile=\<filename\> #set name of file with channel mask; required if doChannelMasking=1 

nStrips=254 # No. of strips

pitchDUT=0.09 # Pitch

residualSigmaDUT=0.026 #residual(4times this value) in mm used for matching DUT hit/cluster/stub to Track

alignmentOutputFile=\<filename\> #Filename from where the alignment parameters will be read

#Alignment Paremter file format

Each line in the file corresponds to a Run and is a ":" separated list of all alignment parameters required by our analysis written out in the following order.

Run:offsetFEI4X:offsetFEI4Y:residualSigmaFEI4X:residualSigmaFEI4Y:zD0:offsetD0:deltaZ:angle

Run=\<Run Number\>

offsetFEI4X/Y=Offset of track with respect to FeI4 hits in X and Y

residualSigmaFEI4X/Y=Pitch at FeI4 plane obtained from telescope-Fei4 residual fits

zD0=Position of DUT0 w.r.t FeI4

offsetD0=Offset of track-DUT hits obtained from residual fit at DUT planr

deltaZ=difference in Z pos between DUT0 and DUT1

angle=DUT angle w.r.t beam

**If alignment parameters are read from file, the code searches for the Run Number and takes the alignment parameters from that line.
***For TDR, use the alignment parameters under data directory
