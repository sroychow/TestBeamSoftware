#include "PlotMakerBase.h"
#include "../interface/Utility.h"
namespace tBeamPlotMaker {

void plot_latency() {

  std::vector<std::pair<float, float> > lat_values;
  lat_values.push_back(std::pair<float, float>(18.,8.));
  lat_values.push_back(std::pair<float, float>(18.,7.));
  lat_values.push_back(std::pair<float, float>(19.,7.));
  lat_values.push_back(std::pair<float, float>(20.,7.));
  lat_values.push_back(std::pair<float, float>(19.,6.));
  lat_values.push_back(std::pair<float, float>(20.,6.));
  lat_values.push_back(std::pair<float, float>(19.,5.));
  lat_values.push_back(std::pair<float, float>(20.,5.));

  TString tag = "00";
  int indx = 1;

  TH2F* th2_hit = new TH2F("trigVSstubLat_hit", "Trigger Lat vs Stub Lat (Hits)",   3, 17.5, 20.5, 4, 4.5, 8.5);
  TH2F* th2_stub = new TH2F("trigVSstubLat_stub", "Trigger Lat vs Stub Lat (Stub)", 3, 17.5, 20.5, 4, 4.5, 8.5);
  for (std::vector<std::pair<float,float> >::iterator it = lat_values.begin(); it != lat_values.end(); it++) {

    indx++;
    TString fname = "run00";
    fname += indx;
    fname += "_dqm.root"; 
    TFile* file = new TFile(fname);
    if (!file) continue;
    TH1I* th_hit = (TH1I*) file->Get("tot_hits");
    TH1I* th_stub = (TH1I*) file->Get("tot_stubs");
    float wt_hit = 0;
    float wt_stub = 0;
    if (th_hit) {
      for (int ibin = 1; ibin < th_hit->GetNbinsX(); ibin++) if (th_hit->GetBinContent(ibin)) wt_hit += ibin* th_hit->GetBinContent(ibin);
      std::cout << wt_hit << std::endl;
      wt_hit = wt_hit/ 300000.;
    }
    if (th_stub) {
      for (int ibin = 2; ibin < th_stub->GetNbinsX(); ibin++) if (th_stub->GetBinContent(ibin)) wt_stub += ibin* th_stub->GetBinContent(ibin);
      std::cout << wt_stub << std::endl;
      wt_stub = wt_stub/ 300000.;
    }
    th2_hit->Fill(it->first, it->second, wt_hit);
    th2_stub->Fill(it->first, it->second, wt_stub);
    std::cout << fname << " " << it->first << " " << it->second << " " << wt_hit << " " << wt_stub <<std::endl;
  } 
  TCanvas* tc1 = new TCanvas("Test1", "Test1", 600., 600);
  TCanvas* tc2 = new TCanvas("Test2", "Test2", 600., 600);
  tc1->cd();
  th2_hit->SetStats(0);
  th2_hit->Draw("boxtext");
  tc2->cd();
  th2_stub->SetStats(0);
  th2_stub->Draw("boxtext");
}
void  plot_StubParameter_Scan(std::string text_file, std::string xtit, std::string ytit, int nbin, float xmin, float xmax, std::string column) {
  std::vector<std::string> lines;
  read_lines(text_file, lines);
  std::map<std::string, TH1F*> histos;
  for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
    std::vector<std::string> tokens;
    Utility::tokenize((*it) , tokens, ":");
    TFile* f = TFile::Open(tokens[0].c_str());
    if (!f) continue;

    float xval       = atof(tokens[1].c_str());
    std::string type = tokens[2]; 
    int   icol       = atoi(tokens[3].c_str());
    
    TString hname;
    hname = ytit.c_str();
    hname += "_";
    hname += column.c_str();
    hname += "_";
    hname += type.c_str();

    TH1F* th = 0;
    std::map<std::string, TH1F*>::iterator ih = histos.find(type);
    if (ih !=  histos.end()) {
      th = (*ih).second;
    } else {
      th =  new TH1F (hname, hname, nbin, xmin, xmax);
      th->SetDirectory(0);
      histos.insert(std::pair<std::string, TH1F*>(type, th));  
    }

    float yval = -999.9;
    float err = -999.9;
    
    getMeanAndErr(f, ytit, yval, err);
    std::cout << " Hname " << th->GetTitle() << " Type " << type  << " Xval " << xval << " Yval " << yval << " Err " << err << std::endl;
    th->SetBinContent(th->FindBin(xval), yval);
    th->SetBinError(th->FindBin(xval), err);

    set_hstyle(th, xtit, ytit, icol);
    f->Close();
    f->Delete(); 
    delete f;
  }
  TLegend* tl = new TLegend(0.50,0.20,0.85,0.4);
  int indx = 0;
  TCanvas* cCanvas = new TCanvas( "c1", "c1", 800, 600);
  for (std::map<std::string, TH1F*>::iterator im = histos.begin(); im != histos.end(); im++) {
    TH1F* th = im->second;
    TString tag = im->first.c_str();
    tl->AddEntry(th, tag, "P");
    if (indx == 0) {
      std::string htitle = ytit;
      htitle += " Vs ";
      htitle += xtit;  
      th->SetTitle(htitle.c_str());
      th->Draw("e");
    } else th->Draw("esame");
    indx++;
  }
  tl->Draw();   
  cCanvas->SaveAs("StubParameter_ScanPlot.png");
  cCanvas->DrawClone();
}
void  plot_ClusterParameter_Scan(std::string text_file, std::string xtit, std::string ytit, int nbin, float xmin, float xmax, std::string det, std::string column) {
  std::vector<std::string> lines;
  read_lines(text_file, lines);

  std::map<std::string, TH1F*> histos;
  for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
    std::vector<std::string> tokens;
    Utility::tokenize((*it) , tokens, ":");
    TFile* f = new TFile(tokens[0].c_str());
    if (!f) continue;

    float xval       = atof(tokens[1].c_str());
    int   icol       = atoi(tokens[1].c_str());
    std::string type = tokens[2]; 
    
    TString hname;
    hname = ytit.c_str();
    hname += "_";
    hname += column.c_str();
    hname += "_";
    hname += det.c_str();
    hname += "_";
    hname += type.c_str();
    
    TH1F* th = 0;
    std::map<std::string, TH1F*>::iterator ih = histos.find(type);
    if (ih !=  histos.end()) {
      th = (*ih).second;
    } else {
      th =  new TH1F (hname, hname, nbin, xmin, xmax);
      th->SetDirectory(0);
      histos.insert(std::pair<std::string, TH1F*>(type, th));  
    }

    float yval = -999.9;
    float err  = -999.9;
    
    getMeanAndErr(f, det, column, ytit, yval, err);
      
    if (yval == -999.9 || err == -999.9) continue;
        
    th->SetBinContent(th->FindBin(xval), yval);
    th->SetBinError(th->FindBin(xval), err);
    std::cout << " Hname " << th->GetTitle() << " Type " << type << " Xval " << xval << " Yval " << yval << " Yerr " << err << std::endl;
    set_hstyle(th, xtit, ytit, icol);
    f->Close();
    f->Delete(); 
  }
  TLegend* tl = new TLegend(0.50,0.20,0.85,0.4);
  int indx = 0;
  for (std::map<std::string, TH1F*>::iterator im = histos.begin(); im != histos.end(); im++) {
    TH1F* th = im->second;
    TString tag = im->first.c_str();
    tl->AddEntry(th, tag, "P");
    if (indx == 0) {
      std::string htitle = ytit;
      htitle += " Vs ";
      htitle += xtit;  
      th->SetTitle(htitle.c_str());
      th->Draw("e");
    } else th->Draw("esame");
    indx++;
  }
  tl->Draw();    
}
void plot_pedNoiseVsTPulse(std::string text_file) {
  std::vector<std::string> lines;
  read_lines(text_file, lines);
  TH2F* th1 = new TH2F ("PedVsTPulse","Pedestal with Test Pulse", 40, -0.5, 40.5, 25, -0.5, 149.5);  
  th1->GetXaxis()->SetTitle("Test Pulse in I2C unit");
  th1->GetYaxis()->SetTitle("Pedestal");

  TH2F* th2 = new TH2F ("NoiseVsTPulse","Noise with Test Pulse", 40, -0.5, 40.5, 40, -0.5, 3.5);
  th2->GetXaxis()->SetTitle("Test Pulse in I2C unit");
  th2->GetYaxis()->SetTitle("Noise");

  for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
    std::vector<std::string> tokens;
    Utility::tokenize((*it) , tokens, ":");
    TFile* f = new TFile(tokens[0].c_str());

    float pulse = atof(tokens[1].c_str());    
    if (f) {
      for (unsigned int ic = 0; ic < 8; ic++) {
        TString name = "Fe0CBC";
        name += ic;

        TString hname1 = name;
        TString hname2 = name;
    
        hname1 += "_Pedestal";
        hname2 += "_Noise";
        
        TH1F* t1 = (TH1F*) f->Get(hname1);
        TH1F* t2 = (TH1F*) f->Get(hname2);

        std::cout << " Pulse " << pulse << " CBC #" << ic << " Hitogram Means " << t1->GetMean() << " " << t2->GetMean() << std::endl; 
        if (t1 && t2) {
          th1->Fill(pulse, t1->GetMean());  
          th2->Fill(pulse, t2->GetMean());
        }  
      }
      f->Close();
      f->Delete();
    }
  }
  TCanvas* tc1 = new TCanvas("Pedestal", "Pedestal", 400., 400.);
  TCanvas* tc2 = new TCanvas("Noise", "Noise", 400., 400.);
  th1->SetMarkerColor(2);
  th2->SetMarkerColor(4);
  th1->SetMarkerStyle(20);
  th2->SetMarkerStyle(20);
  th1->SetStats(0);
  th2->SetStats(0);
  
  tc1->cd();th1->Draw();  
  tc2->cd();th2->Draw();
}

void read_lines(std::string tname, std::vector<std::string>& files) {
  static const int BUF_SIZE = 512;
  
  ifstream myTxtFile;
  myTxtFile.open(tname.c_str(), ios::in);
  if (!myTxtFile) {
    std::cerr << "Input File: " << tname << " could not be opened!" << std::endl;
    return;
  }
  
  char buf[BUF_SIZE];
  if (myTxtFile) {
    while (myTxtFile.good()) {
      if (!myTxtFile.eof()) {
        myTxtFile.getline(buf, BUF_SIZE, '\n'); 
        string line(buf);
        if (line.empty()) continue;
        if (line.substr(0,2) == "//") continue;
        if (line.substr(0,1) == "#") continue;
        files.push_back(line);
      }
    }
  }
}
/*void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters) {
  
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  
    // Find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);
  
    while (std::string::npos != pos || string::npos != lastPos)  {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
    
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
    
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
    }
  }
*/
void set_hstyle(TH1F* th, std::string xtit, std::string ytit, int col) {
  if (!th) return;
  th->GetXaxis()->SetTitle(xtit.c_str());
  th->GetYaxis()->SetTitle(ytit.c_str());
  th->SetStats(0);
  th->SetMarkerStyle(20);  
  th->SetMarkerColor(col);
  th->SetLineColor(col);
}
void getMeanAndErr(TFile* file, std::string& ytit, float& y, float& e) {
  TString path;
  if (ytit == "StubEfficiency") {
    path = "StubInfo/stubEff";
  } else if (ytit == "HitDifference") {
    path = "Correlation/diff";    
  } else if (ytit == "NumberOfStubs") {
    path  = "StubInfo/nstub";
  } else if (ytit == "NclusterDifference") {
    path = "StubInfo/nclusterdiffC0";
  } 
  TH1* t;
  file->GetObject(path+"C0", t);
  //  std::cout << path+"C0" << ": " << t << " hname: " << t->GetName() << std::endl;
  if (t) {
    y = t->GetMean();
    e = t->GetRMS() / t->GetEntries(); 
  }
  //std::cout << "exiting get..." << std::endl;
}
void getMeanAndErr(TFile* file, std::string& det, std::string column, std::string& ytit, float& y, float& e) {
  TString path = det.c_str();
  if (ytit == "ClusterWidth") {
    path += "/clusterWidth";
  } else if (ytit == "NumberOfCluster") {
    path += "/ncluster";
  } else if (ytit == "NCluster") {
    path += "/ncluster";
  } else if (ytit == "Occupancy") {
    path += "/chsize";
  }
  path += column.c_str();
  TH1* t = dynamic_cast<TH1*> (file->Get(path));
  if (t) {
    y = t->GetMean();
    e = t->GetRMS() / t->GetEntries(); 
  }
}
}
