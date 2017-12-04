#ifndef __TelescopeEvent__h
#define __TelescopeEvent__h
#include<stdint.h>
#include<iostream>
#include<iomanip>
#include<vector>
using std::vector;
namespace tbeam {
class TelescopeEvent : public TObject {
  public :
    TelescopeEvent();
    TelescopeEvent(const TelescopeEvent& t);
    virtual ~TelescopeEvent();
    int nTracks;// number of tracks in the telescope
    int event;// trigger/event number used for sync. with event from DUT
    std::vector<float> *xPos;//Track impact x-position at the reference plane
    std::vector<float> *yPos;//Track impact y-position at the reference plane
    std::vector<float> *dxdz;//x-Gradient along beam direction 
    std::vector<float> *dydz;//y-Gradient along beam direction
    std::vector<float> *chi2;//Chi-square from track fitting
    std::vector<float> *ndof;//#degrees of freedom
    //optional requirement
    std::vector<float> *xPosError;//Track impact x-position error
    std::vector<float> *yPosError;//Track impact y-position error

    ClassDef(TelescopeEvent,1)
      };
}
#endif
