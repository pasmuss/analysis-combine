#include "tdrstyle.C"
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>

#include "TH1.h"
#include "TStyle.h"
#include "TMath.h"
#include <iostream>

#include "RooWorkspace.h"

#include "HbbStylesNew.cc"

using namespace std;

int AnalysisWorkspace(){

  // As usual, load the combine library to get access to the RooParametricHist
  gSystem->Load("libHiggsAnalysisCombinedLimit.so");

  vector<double> lumiscalefactors = {142.9,69.29,75.24};//SR1
  //vector<double> lumiscalefactors = {75.24,75.45,64.51,77.85,70.42};//SR2
  //vector<double> lumiscalefactors = {70.42,72.21,75.66,74.27};//SR3
  //vector<double> lumiscalefactors = {74.27,71.75,73.48,75.7};//SR4
  vector<string> allmasses = {"300","350","400"};//SR1
  //vector<string> allmasses = {"400","450","500","600","700"};//SR2
  //vector<string> allmasses = {"700","800","900","1000"};//SR3
  //vector<string> allmasses = {"1000","1200","1400","1600"};//SR4

  if(!(lumiscalefactors.size() == allmasses.size())){
    cout << "Number of mass points and lumi scale factors does not agree. Please check what you provided." << endl;
    return -1;
  }
  map<string,double> assignedlumisf;
  for (unsigned int massvalue = 0; massvalue < allmasses.size(); massvalue++){
    assignedlumisf[allmasses[massvalue]] = 1./lumiscalefactors[massvalue];
  }

  // A search in a mbb tail, define mbb as our variable
  RooRealVar mbb("mbb","m_{12}",200,500);//SR 1: 300/350/400
  //RooRealVar mbb("mbb","m_{12}",260,785);//SR 2: 400/450/500/600/700
  //RooRealVar mbb("mbb","m_{12}",390,1270);//SR 3: 700/800/900/1000
  //RooRealVar mbb("mbb","m_{12}",500,2000);//SR 4: 1000/1200/1400/1600
  RooArgList vars(mbb);

  ///
  /// PART 1: GET SIGNAL HIST AND SCALE FROM 36020 PB (2017) TO 1 PB (FROM ANALYSIS MACRO)
  ///

  vector<string> srmasses = {"300","350","400"};
  //vector<string> srmasses = {"400","450","500","600","700"};
  //vector<string> srmasses = {"700","800","900","1000"};
  //vector<string> srmasses = {"1000","1200","1400","1600"};

  for (unsigned int mass = 0; mass < srmasses.size(); mass++){
    cout << "mass " << srmasses[mass] << endl;
    TFile* f_signal_in = new TFile(("/afs/desy.de/user/a/asmusspa/Documents/CMSSW_9_2_15/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");//SR (always), 3j (for now: inclusive)
    cout << ("/afs/desy.de/user/a/asmusspa/Documents/CMSSW_9_2_15/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str() << endl;
    TH1F* h_signal_in = (TH1F*)f_signal_in->Get("m12_aac");
    h_signal_in -> SetName("h_signal_in");
    double lumisf = assignedlumisf[srmasses[mass]];
    cout << "lumi sf " << lumisf << endl;
    h_signal_in -> Scale(lumisf);
    RooDataHist RDHSig("RDHSig","Signal",vars,h_signal_in);

    ///
    /// PART 2: GET DATA_OBS HISTS FOR CR/SR (CR FROM ANALYSIS MACRO, SR FOR NOW FROM TOYS)
    ///
    
    TFile* f_cr_in = new TFile("/afs/desy.de/user/a/asmusspa/Documents/CMSSW_9_2_15/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rereco/rereco-CDEF-deep-CR-3j.root","READ");//CR, 3j, full 2017
    TH1F* h_cr_in = (TH1F*)f_cr_in->Get("m12_aac");
    h_cr_in -> SetName("h_cr_in");
    RooDataHist RDHCR("RDHCR","CR",vars,h_cr_in);

    TFile* f_sr_in = new TFile("/afs/desy.de/user/a/asmusspa/Documents/CMSSW_9_2_15/src/Analysis/Tools/test/Outputdata/TF_bias/Pseudos1904.root","READ");
    TH1F* SRHist = (TH1F*)f_sr_in->Get("SRHist");//data_obs SR
    RooDataHist RDHSR("RDHSR","SR",vars,SRHist);
    
    ///
    /// PART 3: GET BG PARAMETRIZATION FROM ROOFIT
    ///
    
    TFile* f_bgfit = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_9_2_15/src/Analysis/Models/results/BG-CR-3j_SR1/BernsteinEffProd7-1/200to500/workspace/FitContainer_workspace.root","READ");
    //TFile* f_bgfit = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_9_2_15/src/Analysis/Models/results/BG-CR-3j_SR1p5/SuperNovoEffProd1/260to785/workspace/FitContainer_workspace.root","READ");//SR2
    //TFile* f_bgfit = new TFile("/afs/desy.de/user/a/asmusspa/Documents/CMSSW_9_2_15/src/Analysis/Models/results/BG-CR-3j_SR2/SuperNovo0/390to1270/workspace/FitContainer_workspace.root","READ");//SR3
    //TFile* f_bgfit = new TFile("/afs/desy.de/user/a/asmusspa/Documents/CMSSW_9_2_15/src/Analysis/Models/results/BG-CR-3j_SR4/SuperNovo0/500to2000/workspace/FitContainer_workspace.root","READ");//SR4
    RooWorkspace* w_bgfit = (RooWorkspace*)f_bgfit->Get("workspace");
    RooAbsPdf* background = w_bgfit -> pdf("background");
    background -> SetName("background");
    RooRealVar background_norm("background_norm","Number of background events",RDHCR.sumEntries(),0,1000000);
    background_norm.setConstant();

    ///
    /// PART 4: DEFINE SIGNAL AND TRANSFER FACTOR PDF(S) // TAKE CARE OF PROPER TF USAGE PER SR
    /// SRs 1/2: EXT ERF; SRs 3/4: LIN --> PERHAPS GO FOR OVERALL EXT ERF (P=54%, BETTER THAN INDIVIDUAL SRs)
    ///

    RooRealVar offsetTF("offsetTF","offset of TF in x direction",90,85,95);
    RooRealVar steepnessTF("steepnessTF","Steepness of rise in TF",6.9e-3,5e-3,9e-3);
    RooRealVar slopelinTF("slopelinTF","Slope of linear part of TF",-2.1e-4,-4e-4,-0.1e-4);
    RooArgList varsTF(mbb,offsetTF,steepnessTF,slopelinTF);
    RooGenericPdf TF("TF","TF","TMath::Erf(steepnessTF*(mbb-offsetTF))*(1+slopelinTF*mbb)",varsTF);
    RooRealVar signal_norm("signal_norm","Signal normalization",RDHSR.sumEntries(),0,1000000);
    
    //Output file
    TFile *fOut = new TFile(("ws_analysis_SR1_toySR_" + srmasses[mass]  + "GeV.root").c_str(),"RECREATE");
    RooWorkspace wspace("wspace","wspace");
    wspace.import(RDHCR);
    wspace.import(RDHSR);
    wspace.import(RDHSig);
    wspace.import(*background);
    wspace.import(background_norm);
    wspace.import(TF);
    wspace.factory("PROD::signal(background,TF)");
    wspace.import(signal_norm);
    
    TH1* h_sr_toy;
    h_sr_toy = wspace.pdf("signal") -> createHistogram("h_sr_toy",mbb,RooFit::Binning(h_cr_in->GetNbinsX(),mbb.getMin(),mbb.getMax()));
    h_sr_toy -> Scale(RDHSR.sumEntries()/h_sr_toy->Integral());
    RooDataHist RDHSRToy("RDHSRToy","Signal region toy",vars,h_sr_toy);
    wspace.import(RDHSRToy);

    wspace.Write();
    cout << ("File created: ws_analysis_SR1_toySR_" + srmasses[mass]  + "GeV.root").c_str() << endl;
    fOut -> Close();
  }  
  return 0;
}
