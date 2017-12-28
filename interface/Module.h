#ifndef Module_h
#define Module_h
#include "Hit.h"
#include "Cluster.h"
#include "Stub.h"
#include<vector>
#include<string>
namespace tbeam{
  class Module {
  public:
    Module() {
      name = "";
      detidLower_ = "";
      detidUpper_ = "";
      z = 0.;
      xrot = 0.;
      yrot = 0.;
      hdirLower_ = "";
      hdirUpper_ = "";
      ncbc_ = 0;
      nstrips_ = 0;
      pitch_ = 0.;
    }
    Module(const std::string n, const std::string dL, const std::string dU,
           const float zPos, const float xRot, const float yRot,
           const int nCBC = 2, const int nStrips = 254, const float p = 90) {
      name = n;
      detidLower_ = dL;
      detidUpper_ = dU;
      z = zPos;
      xrot = xRot;
      yrot = yRot;
      hdirLower_ = name + "/" + detidLower_;
      hdirUpper_ = name + "/" + detidUpper_;
      ncbc_ = nCBC;
      nstrips_ = nStrips;
      pitch_ = p/1000.;//take input in microns convert to mm
    }
    ~Module() {}
    std::string name;
    std::string detidLower_;
    std::string detidUpper_;
    std::string hdirLower_;
    std::string hdirUpper_;
    unsigned int ncbc_;
    unsigned int nstrips_;
    float pitch_;
    float z;
    float xrot;
    float yrot;
    std::vector<tbeam::hit> lowerHits;//
    std::vector<tbeam::hit> upperHits;
    std::vector<tbeam::cluster> lowerCbcCls;//only in sparsified mode
    std::vector<tbeam::cluster> upperCbcCls;//only in sparsified mode
    std::vector<tbeam::cluster>  lowerOfflineCls;//only in unsparsified mode
    std::vector<tbeam::cluster>  upperOfflineCls;//only in unsparsified mode
    std::vector<tbeam::stub> cbcStubs;//
    std::vector<tbeam::stub> offlineStubs;
  };
}
#endif
