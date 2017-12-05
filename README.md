## Beam Test Data Analysis 
##Merging Code: Takes Ntuple from EDM, Telescope and merges into one Analysis Tree for user
The code fill the Track vector inside the EDM ntuple
## Installation

Set up ROOT(works with ROOT 6)

make cint

make 

## Usage
##
./ntuplemerger \<EDMTupleName\> \<TelescopeTupleName\> \<RunNumber\>

It produces 1 files AnalysisTree_RunNumber.root
