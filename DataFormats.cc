#include "DataFormats.h"
ClassImp(tbeam::CondEvent)
ClassImp(tbeam::DutEvent)
ClassImp(tbeam::TelescopeEvent)
tbeam::cbc::cbc():
   pipelineAdd(0),
   status(0),
   error(0)
{
}

tbeam::cluster::cluster():
   x(0),
   size(0)
{
}

tbeam::cluster::~cluster(){
}

tbeam::stub::stub():
   x(0),
   direction(0)
{
   seeding=0;
   matched=0;
}

tbeam::CondEvent::CondEvent() :
   run(999999), 
   lumiSection(999999), 
   event(999999),  
   time(999999), 
   unixtime(999999), 
   tdcPhase(999999),
   HVsettings(999999),
   DUTangle(999999),
   window(999999),
   offset(999999),
   cwd(999999),
   tilt(999999),
   vcth(999999),
   stubLatency(999999),
   triggerLatency(999999),
   condData(9999),
   glibStatus(9999)
{
}
void tbeam::CondEvent::reset()
{
  cbcs.clear();
}

tbeam::DutEvent::DutEvent():
   stubWord(0),
   stubWordReco(0),
   isGood(0)
{

}
void tbeam::DutEvent::reset()
{
  dut_channel.clear();
  dut_row.clear();
  //std::map < std::string, std::vector <tbeam::cluster*> > 
  for(auto& d : clusters){
    for(auto p : d.second)     delete p;
    d.second.clear();
  } 
  clusters.clear();
  //std::vector <tbeam::stub*> 
  for(auto s : stubs)    delete s;
  stubs.clear(); 
}
tbeam::DutEvent::~DutEvent(){
   reset();
}


tbeam::TelescopeEvent::TelescopeEvent() 
{
   xPos = new vector<double>();
   yPos = new vector<double>();
   dxdz = new vector<double>();
   dydz = new vector<double>();
   trackNum = new vector<int>();
   iden = new vector<int>();
   chi2 = new vector<double>();
   ndof = new vector<double>();
}
tbeam::TelescopeEvent::TelescopeEvent(const TelescopeEvent& t) {
   nTrackParams = t.nTrackParams;
   euEvt = t.euEvt;
   xPos = new vector<double>(*t.xPos);
   yPos = new vector<double>(*t.yPos);
   dxdz = new vector<double>(*t.dxdz);
   dydz = new vector<double>(*t.dydz);
   trackNum = new vector<int>(*t.trackNum);
   iden = new vector<int>(*t.iden);
   chi2 = new vector<double>(*t.chi2);
   ndof = new vector<double>(*t.ndof);
 }
tbeam::TelescopeEvent::~TelescopeEvent() {
   delete xPos;
   delete yPos;
   delete dxdz;
   delete dydz;
   delete trackNum;
   delete iden;
   delete chi2;
   delete ndof;
}
