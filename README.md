## Beam Test Data Analysis 
##Mering Code: Takes Ntuple from EDM, Telescope, DQM and merges into one Analysis Tree for user
The code updates the EDM tuple adding new branches for Telescope, goodEvent flags.
## Installation

Set up ROOT(works with ROOT 5)

make cint

make 

## Usage
##
./ntuplemerger \<EDMTupleName\> \<TelescopeTupleName\> \<DQMTupleName\> \<OutPutTupleName\>
