#ifndef __PlotMakerBase_h_
#define __PlotMakerBase_h_
#include <Riostream.h>
#include <TROOT.h>
#include <TDirectory.h>
#include <TDirectoryFile.h>
#include <TFile.h>
#include <TKey.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TPaveText.h>
#include <TText.h>
#include <TLegend.h>
#include <TMath.h>
#include <string>
#include <vector>
#include <map>

namespace tBeamPlotMaker {
  void plot_latency();
  void read_lines(std::string tname, std::vector<std::string>& files);
  //void tokenize(const string& str, vector<string>& tokens, const string& delimiter);
  void plot_StubParameter_Scan(std::string text_file, std::string xtit, std::string ytit, int nbin, float xmin, float xmax, std::string col);
  void plot_ClusterParameter_Scan(std::string text_file, std::string xtit, std::string ytit, int nbin, float xmin, float xmax, std::string det, std::string col);
  void plot_pedNoiseVsTPulse(std::string text_file);
  void set_hstyle(TH1F* th, std::string xtit, std::string ytit, int col);
  void getMeanAndErr(TFile* file, std::string& ytit, float& y, float& e);
  void getMeanAndErr(TFile* file, std::string& det, std::string column, std::string& ytit, float& y, float& e);
}
#endif
