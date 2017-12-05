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
    TelescopeEvent()
      {
	xPos = new vector<float>();
	yPos = new vector<float>();
	dxdz = new vector<float>();
	dydz = new vector<float>();
	chi2 = new vector<float>();
	ndof = new vector<float>();
	xPosError = new vector<float>();
	yPosError = new vector<float>();
      }
    TelescopeEvent(const TelescopeEvent& t)
      {
	nTracks = t.nTracks;
	event = t.event;
	xPos = new vector<float>(*t.xPos);
	yPos = new vector<float>(*t.yPos);
	dxdz = new vector<float>(*t.dxdz);
	dydz = new vector<float>(*t.dydz);
	chi2 = new vector<float>(*t.chi2);
	ndof = new vector<float>(*t.ndof);
	xPosError = new vector<float>(*t.xPosError);
	yPosError = new vector<float>(*t.yPosError);
      }
    virtual ~TelescopeEvent()
      {
	delete xPos;
	delete yPos;
	delete dxdz;
	delete dydz;
	delete chi2;
	delete ndof;
	delete xPosError;
	delete yPosError;
      }

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
