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
      detidbottom_ = "";
      detidtop_ = "";
      z = 0.;
      xrot = 0.;
      yrot = 0.;
      hdirbottom_ = "";
      hdirtop_ = "";
      ncbc_ = 0;
      nstrips_ = 0;
      pitch_ = 0.;
    }
    Module(const std::string n, const std::string dL, const std::string dU,
           const float zPos, const float xRot, const float yRot,
           const int nCBC = 2, const int nStrips = 254, const float p = 90) {
      name = n;
      detidbottom_ = dL;
      detidtop_ = dU;
      z = zPos;
      xrot = xRot;
      yrot = yRot;
      hdirbottom_ = name + "/" + detidbottom_;
      hdirtop_ = name + "/" + detidtop_;
      ncbc_ = nCBC;
      nstrips_ = nStrips;
      pitch_ = p/1000.;//take input in microns convert to mm
    }
    ~Module() {}
    std::string name;
    std::string detidbottom_;
    std::string detidtop_;
    std::string hdirbottom_;
    std::string hdirtop_;
    unsigned int ncbc_;
    unsigned int nstrips_;
    float pitch_;
    float z;
    float xrot;
    float yrot;
    std::vector<tbeam::hit> bottomHits;//
    std::vector<tbeam::hit> topHits;
    std::vector<tbeam::cluster> bottomCbcCls;//only in sparsified mode
    std::vector<tbeam::cluster> topCbcCls;//only in sparsified mode
    std::vector<tbeam::cluster>  bottomOfflineCls;//only in unsparsified mode
    std::vector<tbeam::cluster>  topOfflineCls;//only in unsparsified mode
    std::vector<tbeam::stub> cbcStubs;//
    std::vector<tbeam::stub> offlineStubs;
  };
}
#endif
