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

int AnalysisWorkspaceSR2VR(){

  int normCR = 882853;//FR2
  int normSR = 160898;//FR2
  int smothtimes = 10;

  // As usual, load the combine library to get access to the RooParametricHist
  gSystem->Load("libHiggsAnalysisCombinedLimit.so");

  vector<double> lumiscalefactors = {75.24,75.45,64.51,77.85,70.42};//SR2
  vector<string> allmasses = {"400","450","500","600","700"};//SR2

  if(!(lumiscalefactors.size() == allmasses.size())){
    cout << "Number of mass points and lumi scale factors does not agree. Please check what you provided." << endl;
    return -1;
  }
  map<string,double> assignedlumisf;
  for (unsigned int massvalue = 0; massvalue < allmasses.size(); massvalue++){
    assignedlumisf[allmasses[massvalue]] = 1./lumiscalefactors[massvalue];
  }

  // A search in a mbb tail, define mbb as our variable
  RooRealVar mbb("mbb","m_{12}",260,785);//SR 2: 400/450/500/600/700
  RooArgList vars(mbb);

  ///
  /// PART 1: GET SIGNAL HIST AND SCALE FROM 36020 PB (2017) TO 1 PB (FROM ANALYSIS MACRO)
  ///

  vector<string> srmasses = {"400","450","500","600","700"};

  for (unsigned int mass = 0; mass < srmasses.size(); mass++){
    cout << "mass " << srmasses[mass] << endl;
    TFile* f_signal_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");//SR (always), 3j (for now: inclusive)
    cout << ("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str() << endl;
    TH1F* h_signal_in = (TH1F*)f_signal_in->Get("m12_SR2_1GeV");
    h_signal_in -> Smooth(smothtimes);
    h_signal_in -> SetName("h_signal_in");
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
    TH1F* h_JERUp_in = (TH1F*)f_JERUp_in->Get("m12_SR2_1GeV");
    //TFile* f_JER_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/OutputSystVar/Hist_new_uncert_Jan21-21/histograms_new_uncert_JER_" + srmasses[mass] + "GeV_SR2.root").c_str(),"READ");
    //TH1F* h_JERUp_in = (TH1F*)f_JER_in->Get("newup");
    h_JERUp_in -> Smooth(smothtimes);
    h_JERUp_in -> SetName("h_JERUp_in");
    h_JERUp_in -> Scale(lumisf);
    RooDataHist signal_JERUp("signal_JERUp","JER variation +1 sigma",vars,h_JERUp_in);
    RooDataHist signal_bias_JERUp("signal_bias_JERUp","JER variation +1 sigma",vars,h_JERUp_in);
    /// JER DOWN
    TFile* f_JERDown_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/JERdown/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* h_JERDown_in = (TH1F*)f_JERDown_in->Get("m12_SR2_1GeV");
    //TH1F* h_JERDown_in = (TH1F*)f_JER_in->Get("newdown");
    h_JERDown_in -> Smooth(smothtimes);
    h_JERDown_in -> SetName("h_JERDown_in");
    h_JERDown_in -> Scale(lumisf);
    RooDataHist signal_JERDown("signal_JERDown","JER variation -1 sigma",vars,h_JERDown_in);
    RooDataHist signal_bias_JERDown("signal_bias_JERDown","JER variation -1 sigma",vars,h_JERDown_in);
    /// JES UP
    TFile* f_JESUp_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/JESup/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* h_JESUp_in = (TH1F*)f_JESUp_in->Get("m12_SR2_1GeV");
    //TFile* f_JES_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/OutputSystVar/Hist_new_uncert_Jan21-21/histograms_new_uncert_JES_" + srmasses[mass] + "GeV_SR2.root").c_str(),"READ");
    //TH1F* h_JESUp_in = (TH1F*)f_JES_in->Get("newup");
    h_JESUp_in -> Smooth(smothtimes);
    h_JESUp_in -> SetName("h_JESUp_in");
    h_JESUp_in -> Scale(lumisf);
    RooDataHist signal_JESUp("signal_JESUp","JES variation +1 sigma",vars,h_JESUp_in);
    RooDataHist signal_bias_JESUp("signal_bias_JESUp","JES variation +1 sigma",vars,h_JESUp_in);
    /// JES DOWN
    TFile* f_JESDown_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/JESdown/mcsig/mc-sig-" + srmasses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* h_JESDown_in = (TH1F*)f_JESDown_in->Get("m12_SR2_1GeV");
    //TH1F* h_JESDown_in = (TH1F*)f_JES_in->Get("newdown");
    h_JESDown_in -> Smooth(smothtimes);
    h_JESDown_in -> SetName("h_JESDown_in");
    h_JESDown_in -> Scale(lumisf);
    RooDataHist signal_JESDown("signal_JESDown","JES variation -1 sigma",vars,h_JESDown_in);
    RooDataHist signal_bias_JESDown("signal_bias_JESDown","JES variation -1 sigma",vars,h_JESDown_in);

    ///
    /// PART 2: GET DATA_OBS HISTS FOR CR/SR (CR FROM ANALYSIS MACRO, SR FOR NOW FROM TOYS)
    ///
    
    TFile* f_cr_in = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/rereco/rereco-CDEF-deep-CR-3j.root","READ");//CR, 3j, full 2017
    TH1F* h_cr_in = (TH1F*)f_cr_in->Get("m12_SR2_1GeV");
    h_cr_in -> SetName("h_cr_in");
    RooDataHist RDHCR("RDHCR","CR",vars,h_cr_in);

    TFile* f_sr_in = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/rereco/rereco-CDEF-deep-VR-3j.root","READ");
    TH1F* SRHist = (TH1F*)f_sr_in->Get("m12_SR2_1GeV");//data_obs VR
    SRHist -> SetName("SRHist");
    normSR = SRHist -> Integral();
    RooDataHist RDHSR("RDHSR","SR",vars,SRHist);
    
    ///
    /// PART 3: GET BG PARAMETRIZATION FROM ROOFIT
    ///
    
    TFile* f_bgfit = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_9_2_15/src/Analysis/Models/results/BG-CR-3j_SR1p5/SuperNovoEffProd1_limited/260to785/workspace/FitContainer_workspace.root","READ");//SR2
    RooWorkspace* w_bgfit = (RooWorkspace*)f_bgfit->Get("workspace");
    RooAbsPdf* background = w_bgfit -> pdf("background");
    background -> SetName("background");
    RooRealVar background_norm("background_norm","Number of background events",normCR,0,1000000);
    //background_norm.setConstant();

    ///
    /// PART 4: DEFINE SIGNAL AND TRANSFER FACTOR PDF(S) // TAKE CARE OF PROPER TF USAGE PER SR
    /// SRs 1/2: EXT ERF; SRs 3/4: LIN --> PERHAPS GO FOR OVERALL EXT ERF (P=54%, BETTER THAN INDIVIDUAL SRs)
    /// APPROACH APR 28: USE FULL RANGE TF AS TEST
    ///
    
    RooRealVar alphaTF("alphaTF","for extended logistic: upwards or downwards",0.75,0.0,5);//p0 //alpha
    RooRealVar offsetTF("offsetTF","offset of TF in y direction",400,100,750);//p1 //x0 // lin: 0.15,0.1,0.5 // eml 400,100,750
    RooRealVar steepnessTF("steepnessTF","Steepness of rise in TF",0.0085,0.0001,0.1);//p2 //k
    RooRealVar slopelinTF("slopelinTF","Slope of linear part of TF",3.5e-4,-5e-5,8e-4);//p3 //lin: -1.55e-5,-4e-5,2e-5 // eml 2.58e-4,-2e-5,4e-4
    RooArgList varsTF(mbb,alphaTF,offsetTF,steepnessTF,slopelinTF);
    RooGenericPdf TF("TF","TF","(1+alphaTF*TMath::Exp(-steepnessTF*(mbb-offsetTF)))/(1+TMath::Exp(-steepnessTF*(mbb-offsetTF)))*(1-slopelinTF*mbb)",varsTF);
    //RooGenericPdf TF("TF","TF","offsetTF-mbb*slopelinTF",varsTF);
    RooRealVar signalregion_norm("signalregion_norm","Signal normalization",normSR,0.9*normSR,1.1*normSR);

    //Output file
    TFile *fOut = new TFile(("ws_analysis_SR2_toySR_" + srmasses[mass]  + "GeV.root").c_str(),"RECREATE");
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
    cout << ("File created: ws_analysis_SR2_toySR_" + srmasses[mass]  + "GeV.root").c_str() << endl;
    fOut -> Close();
  }  
  return 0;
}
