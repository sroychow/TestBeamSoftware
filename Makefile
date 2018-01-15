UNAME    = $(shell uname)
EXE      = baselineReco alignmentReco 
#deltaClusAnalysis 
 
VPATH  = .:./interface
vpath %.h ./interface

CSUF   = cc
HSUF   = h
DICTC  = Dict.$(CSUF)
DICTH  = $(patsubst %.$(CSUF),%.h,$(DICTC))

SRCS   = src/Event.cc src/BeamAnaBase.cc src/Utility.cc src/Histogrammer.cc   
OBJS   = $(patsubst %.$(CSUF), %.o, $(SRCS))


LDFLAGS  = -g 
SOFLAGS  = -shared 
CXXFLAGS = -I./interface -I./  

#CXX       = clang++
CXX       = g++

HDRS_DICT = interface/Hit.h interface/Cluster.h interface/Cbc.h interface/Stub.h interface/Track.h interface/Event.h interface/LinkDef.h

bin: baselineReco alignmentReco 
#deltaClusAnalysis

all: 
	gmake cint 
	gmake bin 
cint: $(DICTC) 

$(DICTC): $(HDRS_DICT)
	@echo "Generating dictionary $(DICTC) with rootcling ..."
	rootcling -f $@ -rmf interface/TestBeamAnalysis_xr.rootmap -c $^ 
	mv $(DICTC) src/
	mv Dict_rdict.pcm src/

Event.o : src/Event.cc
	$(CXX)  $(CXXFLAGS) `root-config --cflags` -o $@ -c $<
	mv $@ ../src/

BaselineAnalysis.o : src/BaselineAnalysis.cc
	$(CXX)  $(CXXFLAGS) `root-config --cflags` -o $@ -c $<
	mv $@ ../src/

AlignmentMultiDimAnalysis.o : src/AlignmentMultiDimAnalysis.cc
	$(CXX)  $(CXXFLAGS) -Wdeprecated-declarations `root-config --cflags` -o $@ -c $<
	mv $@ ../src/


baselineReco:   src/baselineReco.cc $(OBJS) src/BaselineAnalysis.o src/Dict.o
	$(CXX) $(CXXFLAGS) `root-config --cflags` $(LDFLAGS) $^ -o $@ $(LIBS) `root-config --libs`

alignmentReco:   src/alignmentReco.cc $(OBJS) src/AlignmentMultiDimAnalysis.o src/Dict.o
	$(CXX) $(CXXFLAGS) `root-config --cflags` $(LDFLAGS) $^ -o $@ $(LIBS) `root-config --libs ` -lMinuit2

#deltaClusAnalysis: src/dclusAnalysis.cc $(OBJS) src/DeltaClusterAnalysis.o src/Dict.o
#	$(CXX) $(CXXFLAGS) `root-config --cflags` $(LDFLAGS) $^ -o $@ $(LIBS) `root-config --libs`

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
	@-rm $(OBJS) $(EXE) src/$(DICTC) src/Dict_rdict.pcm 

