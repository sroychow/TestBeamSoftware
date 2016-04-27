#include "DataFormats.h"
namespace skbeam {
 tBeamBase::tBeamBase(){
   dut_channel = new map<string,vector<int> >();
   dut_row = new map<string,vector<int> >();
 }
 tBeamBase::~tBeamBase() {
   delete dut_channel;
   delete dut_row;
 }
 telescopeBase::telescopeBase() {
   xPos = new vector<double>();
   yPos = new vector<double>();
   dxdz = new vector<double>();
   dydz = new vector<double>();
   trackNum = new vector<int>();
   iden = new vector<int>();
   chi2 = new vector<double>();
   ndof = new vector<double>();
 }
 telescopeBase::~telescopeBase() {
   delete xPos;
   delete yPos;
   delete dxdz;
   delete dydz;
   delete trackNum;
   delete iden;
   delete chi2;
   delete ndof;
 }
 
 trawTupleBase::trawTupleBase() {
   dut0Ch0data = new vector<int>();
   dut0Ch1data = new vector<int>();
   dut1Ch0data = new vector<int>();
   dut1Ch1data = new vector<int>();
 }
 trawTupleBase::~trawTupleBase() {
   delete dut0Ch0data;
   delete dut0Ch1data;
   delete dut1Ch0data;
   delete dut1Ch1data;
 }
}
