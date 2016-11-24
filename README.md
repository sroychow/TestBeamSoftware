## Beam Test Data Analysis Software
## Installation

Set up ROOT(works with ROOT 5, requires ROOT to be compiled with libminut2)

After checking out the code:-
make cint

make 

## Usage
##Running on tuple

#Step1: Alignment Analysis

./alignmentReco <jobcardName>

#JobCard Description

Run=\<RUN-NUMBER\>

inputFile=AnalysisTree_<RUN-NUMBER>.root

outputFile=<output-file name>

fei4Z=<FEI4-distance>

doTelescopeMatching=1 #set to true; will look for telescope data; if =0, this step will not work

doChannelMasking=0 #=0 if no channel masked; =1 if channel masking required

channelMaskFile=<filename> #set name of file with channel mask; required if doChannelMasking=1 

nStrips=254 # No. of strips

pitchDUT=0.09 # Pitch

alignmentOutputFile=<filename> #Filename where the alignment parameters will be written

isProductionmode=0 #if =0 a new alignmentOutputFile will be created. If a file of the same name exists,
it will overwrite its content.; =1 will open the alignmentOutputFile file in append mode, use this 
option when running over many runs
