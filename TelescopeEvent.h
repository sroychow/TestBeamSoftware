#ifndef __TelescopeEvent__h
#define __TelescopeEvent__h
#include<stdint.h>
#include<iostream>
#include<iomanip>
#include<vector>
using std::vector;

typedef struct{
    double dxdz;//x-Gradient along beam direction 
    double dydz;//y-Gradient along beam direction
    double xPos;//Track impact x-position at the reference plane
    double yPos;//Track impact y-position at the reference plane
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
    int trigger;// trigger/event number used for sync. with event from DUT
    int runNumber;
    long int timestamp;
} TelescopeEvent;

#endif
