UNAME    = $(shell uname)
EXE      = dutReco
 
VPATH  = .:./interface
vpath %.h ./interface

CSUF   = cc
HSUF   = h
DICTC  = Dict.$(CSUF)
DICTH  = $(patsubst %.$(CSUF),%.h,$(DICTC))

SRCS   = src/argvparser.cc src/DataFormats.cc src/BeamAnaBase.cc src/Reco.cc src/Utility.cc src/Histogrammer.cc src/DUTReconstruction.cc  src/dutReco.cc  
OBJS   = $(patsubst %.$(CSUF), %.o, $(SRCS))

#PLOTSRCS = tools/PlotMakerBase.cc tools/plotMakermain.cc
#PLOTOBJS   = $(patsubst %.$(CSUF), %.o, $(PLOTSRCS))

LDFLAGS  = -g
SOFLAGS  = -shared 
CXXFLAGS = -I./interface -I./  

CXX       = g++
CXXFLAGS += -g -std=c++11
#-Wall -Wno-deprecated -std=c++11


HDRS_DICT = interface/LinkDef.h

bin: dutReco 
all: 
	gmake cint 
	gmake bin 
cint: $(DICTC) 

$(DICTC): $(HDRS_DICT)
	@echo "Generating dictionary $(DICTC) and $(DICTH) ..."
	rootcint -f $@ -c $(CXXFLAGS) $^ 
	perl -pi -e 's#interface/##' $(DICTH) 
	perl -pi -e 's/#include <math.h>/#include <math.h>\n#include <map>/'  $(DICTH)
	mv $(DICTC) src/
	mv $(DICTH) interface/

dutReco:   $(OBJS) src/DUTReconstruction.o src/dutReco.o src/Dict.o
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS) `root-config --libs`

# Create object files
%.o : %.$(CSUF)
	$(CXX) $(CXXFLAGS) `root-config --cflags` -o $@ -c $<

# makedepend
depend: $(SRCS:.$(CSUF)=.$(CSUF).dep)
	@cat $(notdir $^) > Makefile.dep
	@-rm -f $(notdir $^) $(patsubst %,%.bak,$(notdir $^))
%.dep:
	@touch $(notdir $@)
	rmkdepend -f$(notdir $@) -- $(CXXFLAGS) `root-config --cflags` -- $*
include Makefile.dep

# Clean 
.PHONY   : clean 
clean : 
	@-rm $(OBJS) $(EXE) interface/$(DICTH) src/$(DICTC) src/Dict.o $(PLOTOBJS)

