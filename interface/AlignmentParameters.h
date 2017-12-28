#ifndef AlignmentParameters_h
#define AlignmentParameters_h
#include<TMath.h>
#include<ostream>
namespace tbeam {
  class alignmentPars {
   public:
     alignmentPars(){
       //FEI4_z = 0.;
       sensorId_ = "NONE";
       d0_chi2_min_z = 0.;
       d1_chi2_min_z = 0.;
       d0_Offset_aligned = 0.;
       d1_Offset_aligned = 0.;
       deltaz = 0.;
       angle = 0.;
       //resSigmaFEI4X = 0.;
       //resSigmaFEI4Y = 0.;
       //offFEI4X = 0.;
       //offFEI4Y = 0.;
     }

     alignmentPars(const std::string& seId, const double off_d0,
                   const double zDUT_d0, const double dZ, const double t) {
      //FEI4_z = fei4Z;
      sensorId_ = seId;
  	  d0_chi2_min_z = zDUT_d0;
 	    d0_Offset_aligned = off_d0;
  	  deltaz = dZ;
  	  angle = TMath::Pi()*t/180.;
  	  d1_chi2_min_z = d0_chi2_min_z + deltaz*TMath::Cos(angle);
  	  d1_Offset_aligned = d0_Offset_aligned + TMath::Sin(angle)*deltaz;
  	//resSigmaFEI4X = resfei4X;
  	//resSigmaFEI4Y = resfei4Y;
  	//offFEI4X = offxfei4;
  	//offFEI4Y = offyfei4;
     }

     ~alignmentPars(){}
     //double residualSigmaFEI4x() const { return resSigmaFEI4X; }
     //void residualSigmaFEI4x(const double fx){ resSigmaFEI4X = fx; }

     //double residualSigmaFEI4y() const { return resSigmaFEI4Y; }
     //void residualSigmaFEI4y(const double fy) { resSigmaFEI4Y = fy; }

     //double offsetFEI4x() const { return offFEI4X; }
     //void offsetFEI4x(const double ofx) { offFEI4X = ofx; }

     //double offsetFEI4y() const { return offFEI4Y; }
     //void offsetFEI4y(const double ofy) { offFEI4Y = ofy; }

     //double FEI4z() const { return FEI4_z; }
     //void FEI4z(const double fz) { FEI4_z = fz; }

     std::string sensorId() const { return sensorId_; }
     void sensorId(const std::string& seId) { sensorId_ = seId; }

     double d0Z() const { return d0_chi2_min_z; }
     void d0Z(const double dz) { d0_chi2_min_z = dz; }

     double d1Z() const { return d1_chi2_min_z; }
     void d1Z(const double dz) { d1_chi2_min_z = dz; }

     double d0Offset() const { return d0_Offset_aligned; }
     void d0Offset(const double doff) { d0_Offset_aligned = doff; }

     double d1Offset() const { return d1_Offset_aligned; }
     void d1Offset(const double doff) { d1_Offset_aligned = doff; }

     double deltaZ() const { return deltaz; }
     void deltaZ(const double dz) { deltaz = dz; }

     double theta() const { return angle;}
     void theta(const double t) { angle = TMath::Pi()*t/180.; }

     void setD1parametersfromD0() {
      d1_chi2_min_z = d0_chi2_min_z + deltaz*TMath::Cos(angle);
      d1_Offset_aligned = d0_Offset_aligned + TMath::Sin(angle)*deltaz;

     }
     friend std::ostream& operator<< ( std::ostream& out, const tbeam::alignmentPars& a ) {
       out << "----Alignment Parameters----\n";
       out << "SensorId=" << a.sensorId()
           //<< "Fei4Z=" << a.FEI4z()
           << "\nd0_chi2_min_z=" << a.d0Z()
           << "\nd0_Offset_aligned=" << a.d0Offset()
           << "\nd1_chi2_min_z=" << a.d1Z()
           << "\nd1_Offset_aligned=" << a.d1Offset()
           << "\ndeltaZ=" << a.deltaZ()
           << "\nangle(rad)=" << a.theta()
           //<< "\nresidualSigmaFEI4x=" << a.residualSigmaFEI4x()
           //<< "\nresidualSigmaFEI4y=" << a.residualSigmaFEI4y()
           //<< "\noffsetFEI4x=" << a.offsetFEI4x()
           //<< "\noffsetFEI4y=" << a.offsetFEI4y()
           << std::endl;
       return out;
     }
   private:
     std::string sensorId_;
     double d0_chi2_min_z;
     double d1_chi2_min_z;
     double d0_Offset_aligned;
     double d1_Offset_aligned;
     double deltaz;
     double angle;
     //double FEI4_z;
     //double resSigmaFEI4X;
     //double resSigmaFEI4Y;
     //double offFEI4X;
     //double offFEI4Y;
  };
}
#endif
