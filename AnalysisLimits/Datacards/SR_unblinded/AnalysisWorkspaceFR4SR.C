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

int AnalysisWorkspaceFR4SR(){

  int normCR = 110873;//FR4
  int normSR = 19613;//FR4
  int rebin = 1;

  // As usual, load the combine library to get access to the RooParametricHist
  gSystem->Load("libHiggsAnalysisCombinedLimit.so");

  vector<double> lumiscalefactors = {74.27,71.75,73.48,75.7};//SR4
  vector<string> allmasses = {"1000","1200","1400","1600"};//SR4

  if(!(lumiscalefactors.size() == allmasses.size())){
    cout << "Number of mass points and lumi scale factors does not agree. Please check what you provided." << endl;
    return -1;
  }
  map<string,double> assignedlumisf;
  for (unsigned int massvalue = 0; massvalue < allmasses.size(); massvalue++){
    assignedlumisf[allmasses[massvalue]] = 1./lumiscalefactors[massvalue];
  }

  // A search in a mbb tail, define mbb as our variable
  RooRealVar mbb("mbb","m_{12}",500,2000);//SR 4: 1000/1200/1400/1600
  RooArgList vars(mbb);

  ///
  /// PART 1: GET SIGNAL HIST AND SCALE FROM 36020 PB (2017) TO 1 PB (FROM ANALYSIS MACRO)
  ///

  vector<string> srmasses = {"1000","1200","1400","1600"};

  for (unsigned int mass = 0; mass < srmasses.size(); mass++){
    cout << "mass " << srmasses[mass] << endl;
    TFile* f_signal_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");//SR (always), 3j (for now: inclusive)
    cout << ("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str() << endl;
    TH1F* h_signal_in = (TH1F*)f_signal_in->Get("m12_SR4_10GeV");
    h_signal_in -> SetName("h_signal_in");
    h_signal_in -> Rebin(rebin);
    double lumisf = assignedlumisf[srmasses[mass]];
    cout << "lumi sf " << lumisf << endl;
    h_signal_in -> Scale(lumisf);
    RooDataHist signal_bias("signal_bias","Signal bias histogram",vars,h_signal_in);
    RooDataHist signal("signal","Signal",vars,h_signal_in);

    ///
    /// PART 1A: GET SYSTEMATIC UNCERTAINTY VARIATIONS
    /// FOR JER/JES: NEED RESPECTIVE FILES
    /// NAMING: RDHSig_${uncertainty_name}Up (or Down)
    ///

    /// JER UP
    TFile* f_JERUp_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/JERup/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* h_JERUp_in = (TH1F*)f_JERUp_in->Get("m12_SR4_10GeV");
    h_JERUp_in -> SetName("h_JERUp_in");
    h_JERUp_in -> Rebin(rebin);
    h_JERUp_in -> Scale(lumisf);
    RooDataHist signal_JERUp("signal_JERUp","JER variation +1 sigma",vars,h_JERUp_in);
    RooDataHist signal_bias_JERUp("signal_bias_JERUp","JER variation +1 sigma",vars,h_JERUp_in);
    /// JER DOWN
    TFile* f_JERDown_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/JERdown/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* h_JERDown_in = (TH1F*)f_JERDown_in->Get("m12_SR4_10GeV");
    h_JERDown_in -> SetName("h_JERDown_in");
    h_JERDown_in -> Rebin(rebin);
    h_JERDown_in -> Scale(lumisf);
    RooDataHist signal_JERDown("signal_JERDown","JER variation -1 sigma",vars,h_JERDown_in);
    RooDataHist signal_bias_JERDown("signal_bias_JERDown","JER variation -1 sigma",vars,h_JERDown_in);
    /// JES UP
    TFile* f_JESUp_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/JESup/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* h_JESUp_in = (TH1F*)f_JESUp_in->Get("m12_SR4_10GeV");
    h_JESUp_in -> SetName("h_JESUp_in");
    h_JESUp_in -> Rebin(rebin);
    h_JESUp_in -> Scale(lumisf);
    RooDataHist signal_JESUp("signal_JESUp","JES variation +1 sigma",vars,h_JESUp_in);
    RooDataHist signal_bias_JESUp("signal_bias_JESUp","JES variation +1 sigma",vars,h_JESUp_in);
    /// JES DOWN
    TFile* f_JESDown_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/JESdown/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* h_JESDown_in = (TH1F*)f_JESDown_in->Get("m12_SR4_10GeV");
    h_JESDown_in -> SetName("h_JESDown_in");
    h_JESDown_in -> Rebin(rebin);
    h_JESDown_in -> Scale(lumisf);
    RooDataHist signal_JESDown("signal_JESDown","JES variation -1 sigma",vars,h_JESDown_in);
    RooDataHist signal_bias_JESDown("signal_bias_JESDown","JES variation -1 sigma",vars,h_JESDown_in);

    ///
    /// PART 2: GET DATA_OBS HISTS FOR CR/SR (CR FROM ANALYSIS MACRO, SR FOR NOW FROM TOYS)
    ///
    
    TFile* f_cr_in = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/rereco/rereco-CDEF-deep-CR-3j.root","READ");//CR, 3j, full 2017
    TH1F* h_cr_in = (TH1F*)f_cr_in->Get("m12_SR4_10GeV");
    h_cr_in -> SetName("h_cr_in");
    h_cr_in -> Rebin(rebin);
    RooDataHist RDHCR("RDHCR","CR",vars,h_cr_in);

    TFile* f_sr_in = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/rereco/rereco-CDEF-deep-SR-3j.root","READ");
    TH1F* SRHist = (TH1F*)f_sr_in->Get("m12_SR4_10GeV");//data_obs SR
    SRHist -> SetName("SRHist");
    SRHist -> Rebin(rebin);
    normSR = SRHist -> Integral();
    RooDataHist RDHSR("RDHSR","SR",vars,SRHist);
    
    ///
    /// PART 3: GET BG PARAMETRIZATION FROM ROOFIT
    ///
    
    TFile* f_bgfit = new TFile("/afs/desy.de/user/a/asmusspa/Documents/CMSSW_9_2_15/src/Analysis/Models/results/BG-CR-3j_SR4/SuperNovo0_limfrozen-2/500to2000/workspace/FitContainer_workspace.root","READ");//SR4
    RooWorkspace* w_bgfit = (RooWorkspace*)f_bgfit->Get("workspace");
    RooAbsPdf* background = w_bgfit -> pdf("background");
    background -> SetName("background");
    RooRealVar background_norm("background_norm","Number of background events",normCR,0,1000000);
    //background_norm.setConstant();

    ///
    /// PART 4: DEFINE SIGNAL AND TRANSFER FACTOR PDF(S) // TAKE CARE OF PROPER TF USAGE PER SR
    /// SRs 1/2: EXT ERF; SRs 3/4: LIN --> PERHAPS GO FOR OVERALL EXT ERF (P=54%, BETTER THAN INDIVIDUAL SRs)
    ///

    RooRealVar offsetTF("offsetTF","offset of TF in y direction",-590,-750,-250);
    RooRealVar slopelinTF("slopelinTF","Slope of linear part of TF",1.86e-4,-1e-4,3e-4);
    RooRealVar steepnessTF("steepnessTF","steepnes of erf",0.0037,0.0001,0.03);
    RooArgList varsTF(mbb,offsetTF,slopelinTF,steepnessTF);
    RooGenericPdf TF("TF","TF","TMath::Erf(steepnessTF*(mbb-offsetTF))*(1-slopelinTF*mbb)",varsTF);
    /*RooRealVar offsetTF("offsetTF","offset of TF in y direction",0.1660,0.1,0.5);
    RooRealVar slopelinTF("slopelinTF","Slope of linear part of TF",2.65e-5,0.5e-5,5e-5);
    RooArgList varsTF(mbb,offsetTF,slopelinTF);
    RooGenericPdf TF("TF","TF","offsetTF-slopelinTF*mbb",varsTF);*/
    RooRealVar signalregion_norm("signalregion_norm","Signal normalization",normSR,0.9*normSR,1.1*normSR);
    
    //Output file
    TFile *fOut = new TFile(("ws_analysis_SR4_toySR_" + srmasses[mass]  + "GeV.root").c_str(),"RECREATE");
    RooWorkspace wspace("wspace","wspace");
    wspace.import(RDHCR);
    wspace.import(RDHSR);
    wspace.import(signal);
    wspace.import(signal_bias);
    wspace.import(*background);
    wspace.import(background_norm);
    wspace.import(TF);
    wspace.factory("PROD::signalregion(background,TF)");
    wspace.import(signalregion_norm);

    wspace.import(signal_JERUp);
    wspace.import(signal_JERDown);
    wspace.import(signal_JESUp);
    wspace.import(signal_JESDown);
    
    TH1* h_sr_toy;
    h_sr_toy = wspace.pdf("signalregion") -> createHistogram("h_sr_toy",mbb,RooFit::Binning(h_cr_in->GetNbinsX(),mbb.getMin(),mbb.getMax()));
    h_sr_toy -> Scale(normSR/h_sr_toy->Integral());
    RooDataHist RDHSRToy("RDHSRToy","Signal region toy",vars,h_sr_toy);
    wspace.import(RDHSRToy);

    wspace.Write();
    cout << ("File created: ws_analysis_SR4_toySR_" + srmasses[mass]  + "GeV.root").c_str() << endl;
    fOut -> Close();
  }  
  return 0;
}
