#ifdef __CINT__
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
#pragma link C++ class std::vector<unsigned short>+;
#pragma link C++ class std::vector<double>+;

#pragma link C++ class std::map< std::string,std::vector<int> >+;
//#pragma link C++ class std::pair< std::string,std::vector<int> >+;

#pragma link C++ class std::map< std::string,std::vector<unsigned short> >+;
//#pragma link C++ class std::pair< std::string,std::vector<unsigned short> >+;

#pragma link C++ class std::map< std::string,std::vector<double> >+;
#pragma link C++ class std::pair< std::string,std::vector<double> >+;

#pragma link C++ class std::map<std::string,int>+;
//#pragma link C++ class std::pair<std::string,int>+;
#endif
