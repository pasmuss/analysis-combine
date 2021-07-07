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

void biasSR2(){
  // Load the combine Library
  gSystem->Load("libHiggsAnalysisCombinedLimit.so");

  double normSR = 130176;
  // A search in a mbb tail, define mbb as our variable
  RooRealVar mbb("mbb","m_{12}",260,785);
  RooArgList vars(mbb);

  vector<string> masses = {"400","450","500","600","700"};
  vector<double> lumiscalefactors = {75.24,75.45,64.51,77.85,70.42};

  //Input files, Data CR
  TFile* f_bgfit = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_9_2_15/src/Analysis/Models/results/BG-CR-3j_SR1p5/SuperNovoEffProd1_limited/260to785/workspace/FitContainer_workspace.root","READ");
  TFile* f_cr_in = new TFile("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/rereco/rereco-CDEF-deep-CR-3j.root","READ");
  TH1F* h_cr_in = (TH1F*)f_cr_in->Get("m12_SR2_1GeV");
  h_cr_in -> SetName("h_cr_in");
  RooDataHist RDHCR("RDHCR","CR",vars,h_cr_in);
  double normCR = RDHCR.sumEntries();
  cout << "norm CR: " << normCR << endl;

  for (int mass = 0; mass < masses.size(); mass++){
    cout << "mass " << masses[mass] << endl;

    //Signal
    TFile* f_signal_in = new TFile(("/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/test/Configs_diffBTags_allmedium/rootfiles_4med_asympT_onlMC_triggersfMC_Nov12-19/rootfiles_May11-20_properSubranges_inclMC/rootfiles_Nov19-20_inclSystVariations/central/mcsig/mc-sig-" + masses[mass]  + "-NLO-deep-SR-3j.root").c_str(),"READ");
    TH1F* sighist = (TH1F*)f_signal_in->Get("m12_SR2_1GeV");
    sighist -> Scale(1./lumiscalefactors[mass]);
    RooDataHist RDHSig("RDHSig","Signal",vars,sighist);

    //CR parametrization
    RooWorkspace* w_bgfit = (RooWorkspace*)f_bgfit->Get("workspace");
    RooAbsPdf* background = w_bgfit -> pdf("background");
    background -> SetName("background");
    RooRealVar background_norm("background_norm","Number of background events",normCR,0,10000000);
    
    //Actually fitted TF: const // index 1
    RooRealVar offsetTF("offsetTF","offset of TF in y direction",0.1452);
    RooArgList varsTF(mbb,offsetTF);
    RooGenericPdf TF("TF","TF","offsetTF",varsTF);

    //alt. TF #1: lin // index 2
    RooRealVar linoffsetTF("linoffsetTF","offset of TF in y direction",0.1452,0.1,0.5);
    RooRealVar linslopeTF("linslopeTF","slope of TF",0,-0.00012,1);
    RooArgList varslinTF(mbb,linoffsetTF,linslopeTF);
    RooGenericPdf linTF("linTF","linTF","linoffsetTF+(linslopeTF*mbb)",varslinTF);

    //alt. TF #2: quad // index 3
    RooRealVar quadoffsetTF("quadoffsetTF","offset of TF in y direction",0.1452,0.1,0.5);
    RooRealVar quadslopeTF("quadslopeTF","x^1 parameter",0,-0.00012,1);
    RooRealVar quadquadslopeTF("quadquadslopeTF","x^2 parameter",0,-3e-8,1);
    RooArgList varsquadTF(mbb,quadoffsetTF,quadslopeTF,quadquadslopeTF);
    RooGenericPdf quadTF("quadTF","quadTF","quadoffsetTF+(quadslopeTF*mbb)+(quadquadslopeTF*mbb*mbb)",varsquadTF);

    //Alternative TF for bias study: ext. erf. // index 0
    RooRealVar altoffsetTF("altoffsetTF","offset of TF in x direction",93,50,150);
    RooRealVar altsteepnessTF("altsteepnessTF","Steepness of rise in TF",0.0075,0.001,0.03);
    RooRealVar altlinpartTF("altlinpartTF","Linear decrease",0.00015,0.00001,0.0005);
    RooArgList altvarsTF(mbb,altoffsetTF,altsteepnessTF,altlinpartTF);
    RooGenericPdf altTF("altTF","alternative TF","TMath::Erf(altsteepnessTF*(mbb-altoffsetTF))*(1-altlinpartTF*mbb)",altvarsTF);

    //Alternative TF for bias study: ext. mod. logistic // index 4
    RooRealVar x0eml("x0eml","sigmoid's midpoint in x direction",174,150,250);
    RooRealVar keml("keml","growth rate",0.07,0.001,0.5);
    RooRealVar emlalpha("emlalpha","alpha",-0.12,-0.5,2.0);
    RooRealVar emllin("emllin","linear decrease",2.41e-5,1e-5,5e-5);
    RooArgList varsemlTF(mbb,keml,x0eml,emlalpha,emllin);
    RooGenericPdf emlTF("emlTF","modified logistic TF","(1+emlalpha*TMath::Exp(-keml*(mbb-x0eml)))/(1+TMath::Exp(-keml*(mbb-x0eml)))*(1-emllin*mbb)",varsemlTF);

    RooRealVar signalregion_norm("signalregion_norm","SR normalization",normSR,0,1000000);//index 1
    RooRealVar altsr_norm("altsr_norm","alternative SR normalization",normSR,0,1000000);//index 0
    RooRealVar linaltsr_norm("linaltsr_norm","alternative SR normalization, linear TF",normSR,0,1000000);//index 2
    RooRealVar quadaltsr_norm("quadaltsr_norm","alternative SR normalization, quad TF",normSR,0,1000000);//index 3
    RooRealVar emlaltsr_norm("emlaltsr_norm","alternative SR normalization, ext mod logistic TF",normSR,0,1000000);//index 4
  
    //Output
    TFile *fOut = new TFile(("ws_analysis_SR2_toySR_" + masses[mass]  + "GeV.root").c_str(),"RECREATE");
    RooWorkspace wspace("wspace","wspace");
    wspace.import(RDHCR);
    wspace.import(RDHSig);
    wspace.import(*background);
    wspace.import(background_norm);

    wspace.import(TF);//index 1
    wspace.import(altTF);//index 0
    wspace.import(linTF);//index 2
    wspace.import(quadTF);//index 3
    wspace.import(emlTF);//index 4

    //Generate SR histogram // index 1
    wspace.factory("PROD::signalregion(background,TF)");
    wspace.import(signalregion_norm);

    //Generate alternative SR histogram // index 0
    wspace.factory("PROD::altsr(background,altTF)");
    wspace.import(altsr_norm);

    //Generate alternative SR histogram, lin TF // index 2
    wspace.factory("PROD::linaltsr(background,linTF)");
    wspace.import(linaltsr_norm);

    //Generate alternative SR histogram, quad TF // index 3
    wspace.factory("PROD::quadaltsr(background,quadTF)");
    wspace.import(quadaltsr_norm);

    //Generate alternative SR histogram, quad TF // index 4
    wspace.factory("PROD::emlaltsr(background,emlTF)");
    wspace.import(emlaltsr_norm);

    //Get pointers for (alternative) SR pdf
    RooAbsPdf *signalregion = wspace.pdf("signalregion");//index 1
    RooAbsPdf *altsr = wspace.pdf("altsr");//index 0
    RooAbsPdf *linaltsr = wspace.pdf("linaltsr");//index 2
    RooAbsPdf *quadaltsr = wspace.pdf("quadaltsr");//index 3
    RooAbsPdf *emlaltsr = wspace.pdf("emlaltsr");//index 4
    
    //Generate RDH // index 0
    RooDataSet *dataaltSR = (*altsr).generate(mbb,normSR);
    dataaltSR -> SetName("dataaltSR");
    wspace.import(*dataaltSR);
    TH1F* h_altsr = (TH1F*)dataaltSR -> createHistogram("h_altsr",mbb,Binning(525,260,785));
    RooDataHist RDHaltSR_0("RDHaltSR_0","RooDataHist for alternative SR",vars,h_altsr);
    wspace.import(RDHaltSR_0);

    //Generate RDH // index 1
    RooDataSet *datasignalregion = (*signalregion).generate(mbb,normSR);
    datasignalregion -> SetName("datasignalregion");
    wspace.import(*datasignalregion);
    TH1F* h_signalregion = (TH1F*)datasignalregion -> createHistogram("h_signalregion",mbb,Binning(525,260,785));
    RooDataHist RDHaltSR_1("RDHaltSR_1","RooDataHist for alternative SR",vars,h_signalregion);
    wspace.import(RDHaltSR_1);

    //Generate RDH // index 2
    RooDataSet *datalinaltSR = (*linaltsr).generate(mbb,normSR);
    datalinaltSR -> SetName("datalinaltSR");
    wspace.import(*datalinaltSR);
    TH1F* h_linaltsr = (TH1F*)datalinaltSR -> createHistogram("h_linaltsr",mbb,Binning(525,260,785));
    RooDataHist RDHaltSR_2("RDHaltSR_2","RooDataHist for alternative SR",vars,h_linaltsr);
    wspace.import(RDHaltSR_2);

    //Generate RDH // index 3
    RooDataSet *dataquadaltSR = (*quadaltsr).generate(mbb,normSR);
    dataquadaltSR -> SetName("dataquadaltSR");
    wspace.import(*dataquadaltSR);
    TH1F* h_quadaltsr = (TH1F*)dataquadaltSR -> createHistogram("h_quadaltsr",mbb,Binning(525,260,785));
    RooDataHist RDHaltSR_3("RDHaltSR_3","RooDataHist for alternative SR",vars,h_quadaltsr);
    wspace.import(RDHaltSR_3);

    //Generate RDH // index 4
    RooDataSet *dataemlaltSR = (*emlaltsr).generate(mbb,normSR);
    dataemlaltSR -> SetName("dataemlaltSR");
    wspace.import(*dataemlaltSR);
    TH1F* h_emlaltsr = (TH1F*)dataemlaltSR -> createHistogram("h_emlaltsr",mbb,Binning(525,260,785));
    RooDataHist RDHaltSR_4("RDHaltSR_4","RooDataHist for alternative SR",vars,h_emlaltsr);
    wspace.import(RDHaltSR_4);

    RooCategory cat("pdf_index","Index of Pdf which is active");
    RooArgList mypdfs;
    mypdfs.add(*altsr);//alternative, index 0, ext erf
    mypdfs.add(*signalregion);//actual, index 1, const
    mypdfs.add(*linaltsr);//alternative, linear TF, index 2
    mypdfs.add(*quadaltsr);//alternative, quad TF, index 3
    mypdfs.add(*emlaltsr);//alternative, ext mod logistic TF, index 4

    RooMultiPdf multipdf("multipdf","All Pdfs",cat,mypdfs);
    RooRealVar multipdf_norm("multipdf_norm","Number of background events",normSR,0,1000000);

    wspace.import(cat);
    wspace.import(multipdf);
    wspace.import(multipdf_norm);
    wspace.Write();
    fOut -> Close();
  }
}
