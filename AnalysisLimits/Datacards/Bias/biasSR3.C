#include <iostream>
#include <cstring>
#include <string>
#include <cmath>

#include "TH1.h"
#include "TStyle.h"
#include "TMath.h"
#include <iostream>

#include "RooWorkspace.h"
#include "RooMultiPdf.h"
//#include ""

using namespace std;

void biasSR3(){
  // Load the combine Library
  gSystem->Load("libHiggsAnalysisCombinedLimit.so");

  double normSR = 47504;
  // A search in a mbb tail, define mbb as our variable
  RooRealVar mbb("mbb","m_{12}",390,1270);
  RooArgList vars(mbb);

  vector<string> masses = {"700","800","900","1000"};
  vector<double> lumiscalefactors = {70.42,72.21,75.66,74.27};

  //Input files, Data CR
  TFile* f_bgfit = new TFile("/afs/desy.de/user/a/asmusspa/Documents/CMSSW_9_2_15/src/Analysis/Models/results/BG-CR-3j_SR2/SuperNovo0_limfrozen/390to1270/workspace/FitContainer_workspace.root","READ");
  TFile* f_cr_in = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/rereco/rereco-CDEF-deep-CR-3j.root","READ");
  TH1F* h_cr_in = (TH1F*)f_cr_in->Get("m12_SR3_5GeV");
  h_cr_in -> SetName("h_cr_in");
  RooDataHist RDHCR("RDHCR","CR",vars,h_cr_in);
  double normCR = RDHCR.sumEntries();
  cout << "norm CR: " << normCR << endl;

  for (int mass = 0; mass < masses.size(); mass++){
    cout << "mass " << masses[mass] << endl;

    //Signal
    TFile* f_signal_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/mcsig/mc-sig-" + masses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* sighist = (TH1F*)f_signal_in->Get("m12_SR3_5GeV");
    sighist -> Scale(1./lumiscalefactors[mass]);
    RooDataHist RDHSig("RDHSig","Signal",vars,sighist);

    //CR parametrization
    RooWorkspace* w_bgfit = (RooWorkspace*)f_bgfit->Get("workspace");
    RooAbsPdf* background = w_bgfit -> pdf("background");
    background -> SetName("background");
    RooRealVar background_norm("background_norm","Number of background events",normCR,0,10000000);
    
    //Actually fitted TF: lin //index 1
    RooRealVar offsetTF("offsetTF","offset of TF in y direction",0.15,0.1,0.3);
    RooRealVar slopelinTF("slopelinTF","Slope of linear part of TF",-1.27e-5,-1.1e-4,-1.24e-6);
    RooArgList varsTF(mbb,offsetTF,slopelinTF);
    RooGenericPdf TF("TF","TF","slopelinTF*mbb+offsetTF",varsTF);

    //Alternative TF for bias study: ext. erf. //index 0 // old parametrization
    RooRealVar altoffsetTF("altoffsetTF","offset of TF in x direction",-361,-750,250);
    RooRealVar altsteepnessTF("altsteepnessTF","Steepness of rise in TF",0.0016,0.0005,0.03);
    RooRealVar altlinpartTF("altlinpartTF","Linear decrease",1.96e-4,0.0,3e-4);
    RooArgList altvarsTF(mbb,altoffsetTF,altsteepnessTF,altlinpartTF);
    RooGenericPdf altTF("altTF","alternative TF","TMath::Erf(altsteepnessTF*(mbb-altoffsetTF))*(1-altlinpartTF*mbb)",altvarsTF);

    //Alternative #2: extended modified logistic (200-2000) // index 2
    RooRealVar x0eml("x0eml","sigmoid's midpoint in x direction",-60,-200,200);
    RooRealVar keml("keml","growth rate",0.0052,0.001,0.5);
    RooRealVar emlalpha("emlalpha","alpha",2e-9,0.0,5);
    RooRealVar emllin("emllin","linear decrease",1.96e-4,0.0,3e-4);
    RooArgList varsemlTF(mbb,keml,x0eml,emlalpha,emllin);
    RooGenericPdf emlTF("emlTF","modified logistic TF","(1+emlalpha*TMath::Exp(-keml*(mbb-x0eml)))/(1+TMath::Exp(-keml*(mbb-x0eml)))*(1-emllin*mbb)",varsemlTF);

    RooRealVar signalregion_norm("signalregion_norm","SR normalization",normSR,0,1000000);//index 1
    RooRealVar altsr_norm("altsr_norm","alternative SR normalization",normSR,0,1000000);//index 0
    RooRealVar emlsr_norm("emlsr_norm","alternative SR normalization",normSR,0,1000000);//index 2
  
    //Output
    TFile *fOut = new TFile(("ws_analysis_SR3_toySR_" + masses[mass]  + "GeV.root").c_str(),"RECREATE");
    RooWorkspace wspace("wspace","wspace");
    wspace.import(RDHCR);
    wspace.import(RDHSig);
    wspace.import(*background);
    wspace.import(background_norm);
    wspace.import(TF);//index 1
    wspace.import(altTF);//index 0
    wspace.import(emlTF);//index 2

    //Generate SR histogram //index 1
    wspace.factory("PROD::signalregion(background,TF)");
    wspace.import(signalregion_norm);

    //Generate alternative SR histogram //index 0
    wspace.factory("PROD::altsr(background,altTF)");
    wspace.import(altsr_norm);

    //Generate alternative SR histogram //index 2
    wspace.factory("PROD::emlsr(background,emlTF)");
    wspace.import(emlsr_norm);

    //Get pointers for (alternative) SR pdf
    RooAbsPdf *signalregion = wspace.pdf("signalregion");//index 1
    RooAbsPdf *altsr = wspace.pdf("altsr");//index 0
    RooAbsPdf *emlsr = wspace.pdf("emlsr");//index 2
    
    //RDH alternative SR //index 0
    RooDataSet *dataaltSR = (*altsr).generate(mbb,normSR);
    dataaltSR -> SetName("dataaltSR");
    wspace.import(*dataaltSR);
    TH1F* h_altsr = (TH1F*)dataaltSR -> createHistogram("h_altsr",mbb,Binning(176,390,1270));
    RooDataHist RDHaltSR_0("RDHaltSR_0","RooDataHist for alternative SR",vars,h_altsr);
    wspace.import(RDHaltSR_0);

    //RDH alternative SR //index 1
    RooDataSet *datasignalregion = (*signalregion).generate(mbb,normSR);
    datasignalregion -> SetName("datasignalregion");
    wspace.import(*datasignalregion);
    TH1F* h_signalregion = (TH1F*)datasignalregion -> createHistogram("h_signalregion",mbb,Binning(176,390,1270));
    RooDataHist RDHaltSR_1("RDHaltSR_1","RooDataHist for alternative SR",vars,h_signalregion);
    wspace.import(RDHaltSR_1);

    //RDH alternative SR //index 2
    RooDataSet *dataemlsr = (*emlsr).generate(mbb,normSR);
    dataemlsr -> SetName("dataemlsr");
    wspace.import(*dataemlsr);
    TH1F* h_emlsr = (TH1F*)dataemlsr -> createHistogram("h_emlsr",mbb,Binning(176,390,1270));
    RooDataHist RDHaltSR_2("RDHaltSR_2","RooDataHist for alternative SR",vars,h_signalregion);
    wspace.import(RDHaltSR_2);

    RooCategory cat("pdf_index","Index of Pdf which is active");
    RooArgList mypdfs;
    mypdfs.add(*altsr);//alternative, index 0
    mypdfs.add(*signalregion);//actual, index 1
    mypdfs.add(*emlsr);//alternative, index 2

    RooMultiPdf multipdf("multipdf","All Pdfs",cat,mypdfs);
    RooRealVar multipdf_norm("multipdf_norm","Number of background events",normSR,0,1000000);

    wspace.import(cat);
    wspace.import(multipdf);
    wspace.import(multipdf_norm);
    wspace.Write();
    fOut -> Close();
  }
}
