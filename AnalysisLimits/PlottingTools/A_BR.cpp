#include <TH3F.h>
#include <TH3.h>
#include <TGraph.h>
#include <TFile.h>
#include "TSystem.h"
#include <TCanvas.h>
#include <TLegend.h>


#include <string>
#include <vector>

// xsection estimation for MSSM
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.C"
// Tools
#include "Analysis/MssmHbb/interface/utilLib.h"

using namespace std;

const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

void CompareBrs(const vector<double>&, const vector<double>&, const double& , const vector<string>&, const string&, const string&);
double MssmBr(mssm_xs_tools& my,
	      const double& mA,
	      const double& tanBeta);
double THDMBr(map<string,TH3F*>& f,
	      const double& mA,
	      const double& tanBeta,
	      const double& cosB_A,
	      const string& name);
void DrawGraphs(vector<TGraph>& graphs2HDM, const vector<string> &BRs, const double& sinB_A,const string &type, const double& mA);
void GetTHDMHistos(TFile& name,map<string,TH3F*>&,const vector<string>&);

int main(){
  //	Comparison should be done
  //	Histo with 2HDM results:
  vector<string> types{"type2","type3"};
  //	Define mass points:
  vector<double> mAs{125,300,500};
  //	Define tnaBeta points:
  vector<double> tanBetas{10,30,60};
  //	Define cos(beta-alpha) points:
  vector<double> cosB_As;
  for(double i = -1.00; i<= 1.00; i+=0.02) cosB_As.push_back(i);
  for(const auto& type : types){
    string thdm_fname = "/nfs/dust/cms/user/asmusspa/public/CMSSW_9_2_15/src/Analysis/MssmHbb/SusHi/FullRun_100PerJob_AllTypesAndBosons/rootFiles/Histograms3D_" + type + "_mA.root";
    //string thdm_fname = "/nfs/dust/cms/user/asmusspa/public/CMSSW_9_2_15/src/Analysis/MssmHbb/SusHi/FullRun_100PerJob_AllTypesAndBosons/rootFiles/Histograms3D_" + type + "_mH.root";
    TFile f(thdm_fname.c_str(),"read");

    //	Define types of BRs:
    vector<string> BRs = {"br_Ass","br_Acc","br_Abb","br_Att","br_Atautau","br_AZh"};
    //vector<string> BRs = {"br_Hss","br_Hcc","br_Hbb","br_Htt","br_Htautau","br_Hhh","br_HZZ","br_HWW","br_Hgammagamma"};
    for (unsigned int m = 0; m < mAs.size(); m++){
      double mA = mAs.at(m);
      CompareBrs(cosB_As,tanBetas,mA,BRs,thdm_fname,type);
    }
  }
  return 0;
}

void CompareBrs(const vector<double> &cosB_As, const vector<double> &tanBetas, const double&mA, const vector<string>& BRs, const string &thdm_fname,const string &type){
  //	File for 2HDM scans:
  map<string,TH3F*> thdm_map;
  TFile f(thdm_fname.c_str(),"read");
  GetTHDMHistos(f,thdm_map,BRs);
  //	Start mA, tan(beta) and cos(beta-alpha) loops:
  for(const auto& tanBeta : tanBetas){
    // Prepare output TGraphs
    vector<TGraph> graphs2HDM(BRs.size());
    vector<TGraph> graphsMSSM(BRs.size());
    int i = 0;
    for(const auto& BR : BRs){
      TGraph& gr_2hdm = graphs2HDM.at(i);
      //			Set number of points for TGraph
      gr_2hdm.Set((int)cosB_As.size());
      ++i;
      int j =0;
      for(const auto& cosB_A : cosB_As){
	++j;
	//Calcualte SigmaBr and fill TGraphs:
	gr_2hdm.SetPoint(j,cosB_A,THDMBr(thdm_map,mA,tanBeta,cosB_A,BR));
      }
    }
    DrawGraphs(graphs2HDM,BRs,tanBeta,type,mA);
  }
}

void DrawGraphs(vector<TGraph>& graphs2HDM, const vector<string> &BRs, const double& tanBeta, const string &type, const double& mA){
  // setup canvas
  TCanvas can("can","can",600,600);
  can.SetLeftMargin(0.11);
  TLegend leg(0.11,0.755,0.9,0.9);
  leg.SetTextSize(0.03);
  leg.SetNColumns(3);
  leg.SetTextFont(62);
  string typechar;
  if (type == "type1") typechar = "type I";
  else if (type == "type2") typechar = "type II";
  else if (type == "type3") typechar = "flipped";
  else if (type == "type4") typechar = "lepton specific";
  leg.SetHeader( ("2HDM " + typechar + ", tan(#beta) = " + to_string_with_precision(tanBeta, 2)  + ", m_{A/H} = " + to_string_with_precision(mA,3) + ", m_{h} = 125 GeV").c_str() );
  vector<int> colours = {1,2,3,4,6,9,12,28,46};
  // Loop over the tanBetas
  for (unsigned int i = 0; i != BRs.size(); ++i){
    TGraph& thdm = graphs2HDM.at(i);
    auto br = BRs.at(i);
    string brstring = "";
    if (br.find("Ass") == 3) brstring = "A #rightarrow s#bar{s}";
    if (br.find("Acc") == 3) brstring = "A #rightarrow c#bar{c}";
    if (br.find("Abb") == 3) brstring = "A #rightarrow b#bar{b}";
    if (br.find("AZh") == 3) brstring = "A #rightarrow Zh";
    if (br.find("Att") == 3) brstring = "A #rightarrow t#bar{t}";
    if (br.find("Atautau") == 3) brstring = "A #rightarrow #tau^{+}#tau^{-}";
    if (br.find("Hss") == 3) brstring = "H #rightarrow s#bar{s}";
    if (br.find("Hcc") == 3) brstring = "H #rightarrow c#bar{c}";
    if (br.find("Hbb") == 3) brstring = "H #rightarrow b#bar{b}";
    if (br.find("Hhh") == 3) brstring = "H #rightarrow hh";
    if (br.find("Htt") == 3) brstring = "H #rightarrow t#bar{t}";
    if (br.find("Htautau") == 3) brstring = "H #rightarrow #tau^{+}#tau^{-}";
    if (br.find("Hgammagamma") == 3) brstring = "H #rightarrow #gamma#gamma";
    if (br.find("HZZ") == 3) brstring = "H #rightarrow ZZ";
    if (br.find("HWW") == 3) brstring = "H #rightarrow W^{+}W^{-}";
    //Cosmetics:
    thdm.SetMarkerStyle(20 + i);
    thdm.SetMarkerSize(1.02);
    thdm.SetMarkerColor(colours.at(i));
    thdm.SetTitle(";cos(#beta-#alpha); BR");

    //leg.AddEntry(&thdm,("2HDM , " + br).c_str(),"p");
    leg.AddEntry(&thdm, (brstring).c_str(), "p");

    if(i==0){
      thdm.GetYaxis()->SetRangeUser(5e-11,500);
      thdm.GetYaxis()->SetTitleOffset(1.3);
    }
    if(i==0) thdm.GetXaxis()->SetRangeUser(-1.,1.);

    if(i==0){
      thdm.Draw("AP");
    }
    else thdm.Draw("Psame");
    can.SetLogy();
    can.Update();
  }
  leg.Draw();
  can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/Antonio_20191113-1130/" + type + "_thdm_BRs_cosB_A_mA-" + to_string_with_precision(mA,3) + "_tanB-" + to_string_with_precision(tanBeta,2) + ".pdf").c_str() );
}

double MssmBr(mssm_xs_tools& my,
	      const double& mA,
	      const double& tanBeta) {
  double sigmaBBA = my.bbHSantander_A(mA,tanBeta);
  //    double sigmaBBH = my.bbHSantander_H(mA,tanBeta);

  double BrAbb = my.br_Abb(mA,tanBeta);
  //    double BrHbb = my.br_Hbb(mA,tanBeta);

  double totXSec = sigmaBBA*BrAbb;// + sigmaBBH*BrHbb;

  return totXSec;
}

double THDMBr(map<string,TH3F*>& f,
	      const double& mA,
	      const double& tanBeta,
	      const double& cosB_A,
	      const string& name){
  double BrAbb = f[name]->Interpolate(mA,tanBeta,cosB_A);

  return BrAbb;
}

void GetTHDMHistos(TFile& f, map<string,TH3F*>& histos, const vector<string>& BRs){
  CheckZombie(f);
  CheckZombieObjectInTFile(f,"xs_bbA");
  histos["xs_bbA"] = (TH3F*) f.Get("xs_bbA");
  //CheckZombieObjectInTFile(f,"xs_bbH");
  //histos["xs_bbH"] = (TH3F*) f.Get("xs_bbH");
  for(const auto& br: BRs){
    CheckZombieObjectInTFile(f,br);
    histos[br] = (TH3F*) f.Get(br.c_str());
  }

}
