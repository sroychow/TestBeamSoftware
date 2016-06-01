UNAME    = $(shell uname)
EXE      = ntupleMerger
 
VPATH  = .:./
vpath %.h ./

CSUF   = cc
HSUF   = h
DICTC  = Dict.$(CSUF)
DICTH  = $(patsubst %.$(CSUF),%.h,$(DICTC))

SRCS   = DataFormats.cc NtupleMerger.cc
OBJS   = $(patsubst %.$(CSUF), %.o, $(SRCS))

LDFLAGS  = -g
SOFLAGS  = -shared 
CXXFLAGS = -I./interface -I./  

CXX       = g++
CXXFLAGS += -g -std=c++11
#-Wall -Wno-deprecated -std=c++11


HDRS_DICT = DataFormats.h LinkDef.h

bin: ntupleMerger
all: 
	gmake cint 
	gmake bin 
cint: $(DICTC) 

$(DICTC): $(HDRS_DICT)
	@echo "Generating dictionary $(DICTC) and $(DICTH) ..."
	rootcint -f $@ -c $(CXXFLAGS) $^ 
	perl -pi -e 's/#include <math.h>/#include <math.h>\n#include <map>/' $(DICTH)

ntupleMerger:  $(OBJS) Dict.o
	$(CXX) $(CXXFLAGS) `root-config --cflags` $(LDFLAGS) $^ -o $@ $(LIBS) `root-config --libs`

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
	@-rm $(OBJS) $(EXE) $(DICTH) $(DICTC) *.o  

