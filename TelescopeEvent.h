#ifndef __TelescopeEvent__h
#define __TelescopeEvent__h
#include<stdint.h>
#include<iostream>
#include<iomanip>
#include<vector>
using std::vector;
class event : public TObject {
  public :
  event()
    {
      trigger=0;
      runNumber=0;
      timestamp=0;
      xPos=0.;
      yPos=0.;
      dxdz=0.;
      dydz=0.;
      chi2ndf=0.;
      xPosErr=0.;
      yPosErr=0.;
      xPosPrevHit=0.;
      yPosPrevHit=0.;
      xPosErrsPrevHit=0.;
      yPosErrsPrevHit=0.;
      xPosNextHit=0.;
      yPosNextHit=0.;
      xPosErrNextHit=0.;
      yPosErrNextHit=0.;
    }
  event(const event& t)
    {
      trigger=t.trigger;
      runNumber=t.runNumber;
      timestamp=t.timestamp;
      xPos=t.xPos;
      yPos=t.yPos;
      dxdz=t.dxdz;
      dydz=t.dydz;
      chi2ndf=t.chi2ndf;
      xPosErr=t.xPosErr;
      yPosErr=t.yPosErr;
      xPosPrevHit=t.xPosPrevHit;
      yPosPrevHit=t.yPosPrevHit;
      xPosErrsPrevHit=t.xPosErrsPrevHit;
      yPosErrsPrevHit=t.yPosErrsPrevHit;
      xPosNextHit=t.xPosNextHit;
      yPosNextHit=t.yPosNextHit;
      xPosErrNextHit=t.xPosErrNextHit;
      yPosErrNextHit=t.yPosErrNextHit;
    }

    virtual ~event()
      {

      }
    //    int nTracks;// number of tracks in the telescope

    int trigger;// trigger/event number used for sync. with event from DUT
    int runNumber;
    long int timestamp;
    double xPos;//Track impact x-position at the reference plane
    double yPos;//Track impact y-position at the reference plane
    double dxdz;//x-Gradient along beam direction 
    double dydz;//y-Gradient along beam direction
    double chi2ndf;//Chi-square from track fitting
    //double ndof;//#degrees of freedom
    //optional requirement
    double xPosErr;//Track impact x-position error
    double yPosErr;//Track impact y-position error
    double xPosPrevHit;
    double yPosPrevHit;
    double xPosErrsPrevHit;
    double yPosErrsPrevHit;
    double xPosNextHit;
    double yPosNextHit;
    double xPosErrNextHit;
    double yPosErrNextHit;

    ClassDef(event,1)
      };
#endif
