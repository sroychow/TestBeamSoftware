#ifdef __CINT__
#include "Event.h"
#include "TelescopeEvent.h"
#include <TROOT.h>
#include <vector>
#include <map>
#include <string>
#include <utility>

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

#pragma link C++ class std::vector<int>+;
#pragma link C++ class std::map< std::string,std::vector<int> >+;
#pragma link C++ class std::map< std::string,std::vector<double> >+;
#pragma link C++ class std::map<std::string,int>+;
//input formats
#pragma link C++ class tbeam::cluster+;
#pragma link C++ class tbeam::cbc+;
#pragma link C++ class tbeam::stub+;
#pragma link C++ class std::vector<tbeam::cluster >+;
#pragma link C++ class std::map<std::string,std::vector<tbeam::cluster > >+;
#pragma link C++ class std::map<std::string,std::vector<tbeam::cluster* > >+;
#pragma link C++ class std::vector<tbeam::stub>+;
#pragma link C++ class std::vector<tbeam::stub* >+;
#pragma link C++ class tbeam::Event+;
#pragma link C++ class tbeam::Track+;
#pragma link C++ struct TelescopeEvent+;
#pragma link C++ class tbeam::hit+;

#pragma link C++ class  std::vector<unsigned short>+;
#pragma link C++ class  std::map< std::string,std::vector<int> >+;
#pragma link C++ class  std::map< std::string,std::vector<unsigned short> >+;
#endif
