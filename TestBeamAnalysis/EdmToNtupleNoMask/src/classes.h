#include "TBeamAnalysis/TreeMaker/interface/AnalysisObjects.h"

#include <vector>
namespace {
struct dictionary {

 tbeam::Event rv1;
 std::vector<tbeam::Event> vrv1;

 std::vector<int> vrvi;
 std::vector<unsigned short> vrvs;
 std::map< std::string,std::vector<int> >  msvi;
 std::map< std::string,std::vector<unsigned short> > msvs;
};
}
