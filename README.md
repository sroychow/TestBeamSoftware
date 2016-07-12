## Beam Test Data Analysis Software
## Installation

Set up ROOT(works with ROOT 5)

make cint

make 

## Usage
##Running on tuple
./baselineReco  --iFile=InputFileName --oFile==OutputFile Name  <other options from below if necessary>

##Available options
-----------------
-h, --help
    Print this help page

--iFile <value>
    Input Tree name

--oFile <value>
    Output file name

--telM
    Do telescope matching. Default=false

--chMaskF <value>
    Channel Mask file;Ch masking off by default

Return codes
-----------------
    0     Success
    1     Error

