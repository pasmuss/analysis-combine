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

void biasSR1(){
  // Load the combine Library
  gSystem->Load("libHiggsAnalysisCombinedLimit.so");

  double normSR = 126510;
  // A search in a mbb tail, define mbb as our variable
  RooRealVar mbb("mbb","m_{12}",200,500);
  RooArgList vars(mbb);

  vector<string> masses = {"300","350","400"};
  vector<double> lumiscalefactors = {142.9,69.29,75.24};

  //Input files, Data CR
  TFile* f_bgfit = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_9_2_15/src/Analysis/Models/results/BG-CR-3j_SR1/BernEffProd7-2_limited/200to500/workspace/FitContainer_workspace.root","READ");
  TFile* f_cr_in = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/rereco/rereco-CDEF-deep-CR-3j.root","READ");
  TH1F* h_cr_in = (TH1F*)f_cr_in->Get("m12_SR1_1GeV");
  h_cr_in -> SetName("h_cr_in");
  RooDataHist RDHCR("RDHCR","CR",vars,h_cr_in);
  double normCR = RDHCR.sumEntries();
  cout << "norm CR: " << normCR << endl;

  for (int mass = 0; mass < masses.size(); mass++){
    cout << "mass " << masses[mass] << endl;

    //Signal
    TFile* f_signal_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/mcsig/mc-sig-" + masses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* sighist = (TH1F*)f_signal_in->Get("m12_SR1_1GeV");
    sighist -> Scale(1./lumiscalefactors[mass]);
    RooDataHist RDHSig("RDHSig","Signal",vars,sighist);

    //CR parametrization
    RooWorkspace* w_bgfit = (RooWorkspace*)f_bgfit->Get("workspace");
    RooAbsPdf* background = w_bgfit -> pdf("background");
    background -> SetName("background");
    RooRealVar background_norm("background_norm","Number of background events",normCR,0,10000000);
    
    //Alternative TF for bias study: ext. erf. // index 0
    RooRealVar altoffsetTF("altoffsetTF","offset of TF in x direction",93,50,150);
    RooRealVar altsteepnessTF("altsteepnessTF","Steepness of rise in TF",0.0075,0.001,0.03);
    RooRealVar altlinpartTF("altlinpartTF","Linear decrease",0.00015,0.00001,0.0005);
    RooArgList altvarsTF(mbb,altoffsetTF,altsteepnessTF,altlinpartTF);
    RooGenericPdf altTF("altTF","alternative TF","TMath::Erf(altsteepnessTF*(mbb-altoffsetTF))*(1-altlinpartTF*mbb)",altvarsTF);

    //Actually fitted TF: erf // index 1
    RooRealVar offsetTF("offsetTF","offset of TF in x direction",152.4,100,200);
    RooRealVar steepnessTF("steepnessTF","Steepness of rise in TF",0.016,0.001,0.05);
    RooArgList varsTF(mbb,offsetTF,steepnessTF);
    RooGenericPdf TF("TF","TF","TMath::Erf(steepnessTF*(mbb-offsetTF))",varsTF);  

    //Alternative #2: logistic // index 2
    RooRealVar x0log("x0log","sigmoid's midpoint in x direction",183,150,220);
    RooRealVar klog("klog","growth rate",0.05,0.001,0.5);
    RooArgList varslogTF(mbb,klog,x0log);
    RooGenericPdf logTF("logTF","logistic TF","1/(1+TMath::Exp(-klog*(mbb-x0log)))",varslogTF);

    //Alternative #3: extended logistic // index 3
    RooRealVar x0extlog("x0extlog","sigmoid's midpoint in x direction",182,150,220);
    RooRealVar kextlog("kextlog","growth rate",0.05,0.001,0.5);
    RooRealVar extloglinpartTF("extloglinpartTF","linear decrease",2.91e-5,1e-5,5e-5);
    RooArgList varsextlogTF(mbb,kextlog,x0extlog,extloglinpartTF);
    RooGenericPdf extlogTF("extlogTF","extended logistic TF","1/(1+TMath::Exp(-kextlog*(mbb-x0extlog)))*(1-extloglinpartTF*mbb)",varsextlogTF);

    //Alternative #4: modified logistic // index 4
    RooRealVar x0modlog("x0modlog","sigmoid's midpoint in x direction",200,150,250);
    RooRealVar kmodlog("kmodlog","growth rate",0.06,0.001,0.5);
    RooRealVar modlogalpha("modlogalpha","alpha",0.48,0.0,2.0);
    RooArgList varsmodlogTF(mbb,kmodlog,x0modlog,modlogalpha);
    RooGenericPdf modlogTF("modlogTF","modified logistic TF","(1+modlogalpha*TMath::Exp(-kmodlog*(mbb-x0modlog)))/(1+TMath::Exp(-kmodlog*(mbb-x0modlog)))",varsmodlogTF);

    //Alternative #5: extended modified logistic // index 5
    RooRealVar x0eml("x0eml","sigmoid's midpoint in x direction",195,150,250);
    RooRealVar keml("keml","growth rate",0.06,0.001,0.5);
    RooRealVar emlalpha("emlalpha","alpha",0.38,0.0,2.0);
    RooRealVar emllin("emllin","linear decrease",2.41e-5,1e-5,5e-5);
    RooArgList varsemlTF(mbb,keml,x0eml,emlalpha,emllin);
    RooGenericPdf emlTF("emlTF","modified logistic TF","(1+emlalpha*TMath::Exp(-keml*(mbb-x0eml)))/(1+TMath::Exp(-keml*(mbb-x0eml)))*(1-emllin*mbb)",varsemlTF);

    RooRealVar altsr_norm("altsr_norm","alternative SR (ext erf) normalization",normSR,0,1000000);//index 0
    RooRealVar signalregion_norm("signalregion_norm","SR (erf) normalization",normSR,0,1000000);//index 1
    RooRealVar logsr_norm("logsr_norm","alternative (logistic) SR normalization",normSR,0,1000000);//index 2
    RooRealVar extlogsr_norm("extlogsr_norm","alternative (ext logistic) SR normalization",normSR,0,1000000);//index 3
    RooRealVar modlogsr_norm("modlogsr_norm","alternative (mod logistic) SR normalization",normSR,0,1000000);//index 4
    RooRealVar extmodlogsr_norm("extmodlogsr_norm","alternative (ext mod logistic) SR normalization",normSR,0,1000000);//index 5
  
    //Output
    TFile *fOut = new TFile(("ws_analysis_SR1_toySR_" + masses[mass]  + "GeV.root").c_str(),"RECREATE");
    RooWorkspace wspace("wspace","wspace");
    wspace.import(RDHCR);
    wspace.import(RDHSig);
    wspace.import(*background);
    wspace.import(background_norm);

    wspace.import(altTF);//0
    wspace.import(TF);//1
    wspace.import(logTF);//2
    wspace.import(extlogTF);//3
    wspace.import(modlogTF);//4
    wspace.import(emlTF);//5

    //Generate alternative SR histogram
    wspace.factory("PROD::altsr(background,altTF)");//0
    wspace.import(altsr_norm);

    //Generate erf TF SR histogram
    wspace.factory("PROD::signalregion(background,TF)");//1
    wspace.import(signalregion_norm);

    //Generate alternative (logistic) SR histogram
    wspace.factory("PROD::logsr(background,logTF)");//2
    wspace.import(logsr_norm);

    //Generate alternative (logistic) SR histogram
    wspace.factory("PROD::extlogsr(background,extlogTF)");//3
    wspace.import(extlogsr_norm);

    //Generate alternative (logistic) SR histogram
    wspace.factory("PROD::modlogsr(background,modlogTF)");//4
    wspace.import(modlogsr_norm);

    //Generate alternative (logistic) SR histogram
    wspace.factory("PROD::extmodlogsr(background,emlTF)");//5
    wspace.import(extmodlogsr_norm);

    //Get pointers for (alternative) SR pdf
    RooAbsPdf *altsr = wspace.pdf("altsr");//0
    RooAbsPdf *signalregion = wspace.pdf("signalregion");//1
    RooAbsPdf *logsr = wspace.pdf("logsr");//2
    RooAbsPdf *extlogsr = wspace.pdf("extlogsr");//3
    RooAbsPdf *modlogsr = wspace.pdf("modlogsr");//4
    RooAbsPdf *extmodlogsr = wspace.pdf("extmodlogsr");//5
    
    //generate RDH for SR // index 0
    RooDataSet *dataaltSR = (*altsr).generate(mbb,normSR);
    dataaltSR -> SetName("dataaltSR");
    wspace.import(*dataaltSR);
    TH1F* h_altsr = (TH1F*)dataaltSR -> createHistogram("h_altsr",mbb,Binning(300,200,500));
    RooDataHist RDHaltSR_0("RDHaltSR_0","RooDataHist for alternative SR",vars,h_altsr);
    wspace.import(RDHaltSR_0);

    //generate RDH for SR // index 1
    RooDataSet *dataSR = (*signalregion).generate(mbb,normSR);
    dataSR -> SetName("dataSR");
    wspace.import(*dataSR);
    TH1F* h_sr = (TH1F*)dataSR -> createHistogram("h_sr",mbb,Binning(300,200,500));
    RooDataHist RDHaltSR_1("RDHaltSR_1","RooDataHist for SR",vars,h_sr);
    wspace.import(RDHaltSR_1);

    //generate RDH for SR // index 2
    RooDataSet *datalogSR = (*logsr).generate(mbb,normSR);
    datalogSR -> SetName("datalogSR");
    wspace.import(*datalogSR);
    TH1F* h_logsr = (TH1F*)datalogSR -> createHistogram("h_logsr",mbb,Binning(300,200,500));
    RooDataHist RDHaltSR_2("RDHaltSR_2","RooDataHist for alternative SR",vars,h_logsr);
    wspace.import(RDHaltSR_2);

    //generate RDH for SR // index 3
    RooDataSet *dataextlogSR = (*extlogsr).generate(mbb,normSR);
    dataextlogSR -> SetName("dataextlogSR");
    wspace.import(*dataextlogSR);
    TH1F* h_extlogsr = (TH1F*)dataextlogSR -> createHistogram("h_extlogsr",mbb,Binning(300,200,500));
    RooDataHist RDHaltSR_3("RDHaltSR_3","RooDataHist for extlogernative SR",vars,h_extlogsr);
    wspace.import(RDHaltSR_3);
 
    //generate RDH for SR // index 4
    RooDataSet *datamodlogSR = (*modlogsr).generate(mbb,normSR);
    datamodlogSR -> SetName("datamodlogSR");
    wspace.import(*datamodlogSR);
    TH1F* h_modlogsr = (TH1F*)datamodlogSR -> createHistogram("h_modlogsr",mbb,Binning(300,200,500));
    RooDataHist RDHaltSR_4("RDHaltSR_4","RooDataHist for modlogernative SR",vars,h_modlogsr);
    wspace.import(RDHaltSR_4);
    
    //generate RDH for SR // index 5
    RooDataSet *dataextmodlogSR = (*extmodlogsr).generate(mbb,normSR);
    dataextmodlogSR -> SetName("dataextmodlogSR");
    wspace.import(*dataextmodlogSR);
    TH1F* h_extmodlogsr = (TH1F*)dataextmodlogSR -> createHistogram("h_extmodlogsr",mbb,Binning(300,200,500));
    RooDataHist RDHaltSR_5("RDHaltSR_5","RooDataHist for alternative SR",vars,h_extmodlogsr);
    wspace.import(RDHaltSR_5);

    RooCategory cat("pdf_index","Index of Pdf which is active");
    RooArgList mypdfs;
    mypdfs.add(*altsr);//alternative, index 0
    mypdfs.add(*signalregion);//actual, index 1, erf
    mypdfs.add(*logsr);//actual, index 2, logistic
    mypdfs.add(*extlogsr);//actual, index 3, ext logistic
    mypdfs.add(*modlogsr);//actual, index 4, mod logistic
    mypdfs.add(*extmodlogsr);//actual, index 5, ext mod logistic

    RooMultiPdf multipdf("multipdf","All Pdfs",cat,mypdfs);
    RooRealVar multipdf_norm("multipdf_norm","Number of background events",normSR,0,1000000);

    wspace.import(cat);
    wspace.import(multipdf);
    wspace.import(multipdf_norm);
    wspace.Write();
    fOut -> Close();
  }
}
