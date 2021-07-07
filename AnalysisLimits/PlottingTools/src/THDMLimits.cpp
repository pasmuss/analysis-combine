/*
 * THDMLimits.cpp
 *
 *  Created on: 20 авг. 2017 г.
 *      Author: rostyslav
 */

#include "Analysis/MssmHbb/interface/THDMLimits.h"

namespace analysis {
  namespace mssmhbb {

    void THDMLimits::ReadCombineLimits(const std::string& file_name){
      /*
       * MEthod to read combine limits and safe results
       */
      HbbLimits::ReadCombineLimits(file_name);
      GxBr_limits_ = limits_;
    }

    TH3D THDMLimits::Get2HDM_GxBR_3D(const std::string& benchmark_path){
      /*
       * Method to get 2HDM 3D GxBR
       */
      //Output histo
      TH3D *hOut = nullptr;
      //map with histos:
      std::map<std::string,TH3D*> histos;
      //map with projections
      std::map<std::string,TH2*> proj;
      //Open TFile with 2HDM scans:
      TFile f(benchmark_path.c_str(),"READ");
      CheckZombie(f);
      SetTHDMHistos(f,histos);

      if (boson_ == "three"){
	histos["GxBR_A"] = (TH3D*) histos["xs_bbA"]->Clone();
        histos["GxBR_A"]->Multiply(histos["br_Abb"]);
        histos["GxBR_H"] = (TH3D*) histos["xs_bbH"]->Clone();
        histos["GxBR_H"]->Multiply(histos["br_Hbb"]);
	histos["GxBR_h"] = (TH3D*) histos["xs_bbh"]->Clone();
        histos["GxBR_h"]->Multiply(histos["br_hbb"]);

        //Add HxBR + AxBR + hxBR
        histos["GxBR_HandAandh"] = (TH3D*) histos["GxBR_A"]->Clone("GxBR_HandAandh");
        histos["GxBR_HandAandh"]->Add(histos["GxBR_H"],1.);
	histos["GxBR_HandAandh"]->Add(histos["GxBR_h"],1.);
        hOut = histos["GxBR_HandAandh"];
      }
      else if (boson_ == "both") {
	histos["GxBR_A"] = (TH3D*) histos["xs_bbA"]->Clone();
	histos["GxBR_A"]->Multiply(histos["br_Abb"]);
	histos["GxBR_H"] = (TH3D*) histos["xs_bbH"]->Clone();
	histos["GxBR_H"]->Multiply(histos["br_Hbb"]);

	//Add HxBR + AxBR
	histos["GxBR_HandA"] = (TH3D*) histos["GxBR_A"]->Clone("GxBR_HandA");
	histos["GxBR_HandA"]->Add(histos["GxBR_H"],1.);
	hOut = histos["GxBR_HandA"];
      }
      else{
	histos["GxBR_" + boson_] = (TH3D*) histos["xs_bb" + boson_]->Clone();
	histos["GxBR_" + boson_]->Multiply(histos["br_" + boson_ + "bb"]);
	hOut = histos["GxBR_" + boson_];
      }

      if(TEST_){
	std::cout<<" LimitsInterpretation::Get2HDM_GxBR_3D - DONE"<<std::endl;
      }

      //	gStyle->SetPadLeftMargin(0.08);
      //	gStyle->SetPadRightMargin(0.12);
      //	std::cout<<"REQUESTED value is : "<<hOut->GetBinContent(bin_mA, bin_tb, bin_cba)<<" interpolate : "<<hOut->Interpolate(400, 32.6, 0.1)<<std::endl;


      return *hOut;

    }

    TH2D THDMLimits::Get2HDM_GxBR_2D(const TH3& h_in, const double& var, const std::string& axis){
      /*
       * Method to get 2D 2HDM GxBR for particular value of variable VAR, that correspondse to the axis - axis:
       * X - mA
       * Y - tan(b)
       * Z - cos(b-a)
       */
      //Copy input h
      TH3D h = *(TH3D*) h_in.Clone();
      //Out h;
      TH2D *proj = nullptr;
      //Var Range has to be setuped
      int bin = -10;
      var_axis_ = axis;
      var_point_ = var;
      if(axis == "x" || axis == "X") {
	//x = mA
	bin = h.GetXaxis()->FindBin(var);
	h.GetXaxis()->SetRange(bin,bin);
	//Project
	proj = (TH2D*)  h.Project3D("yz");
	proj->GetXaxis()->SetRangeUser(-1.,1.);
      }
      if(axis == "y" || axis == "Y") {
	//tan\beta
	bin = h.GetYaxis()->FindBin(var);
	h.GetYaxis()->SetRange(bin,bin);
	//Project
	proj = (TH2D*)  h.Project3D("zx");
      }
      if(axis == "z" || axis == "Z") {
	// cos(b-a)
	bin = h.GetZaxis()->FindBin(var);
	h.GetZaxis()->SetRange(bin,bin);
	//Project
	proj = (TH2D*)  h.Project3D("yx");
      }

      if(TEST_){
	std::cout<<" LimitsInterpretation::Get2HDM_GxBR_2D - DONE"<<std::endl;
      }

      //Special Pad margins to plot 2D mu scans
      //	gStyle->SetPadTopMargin   (0.09);
      //	gStyle->SetPadLeftMargin(0.14);
      //	gStyle->SetPadRightMargin(0.15);

      return *proj;
    }

    TH2D THDMLimits::Get2HDM_GxBR_2D(const std::string& benchmark_path, const double& var, const std::string& axis){
      /*
       * Method to get 2D 2HDM GxBR for particular value of variable VAR, that correspondse to the axis - axis:
       * X - mA
       * Y - tan(b)
       * Z - cos(b-a)
       * According to a 2HDM scans: benchmark_path
       */
      //Get 3D limits
      auto h = Get2HDM_GxBR_3D(benchmark_path);
      //Call overloaded method to get 2D limits
      return Get2HDM_GxBR_2D(h,var,axis);
    }

    THDMLimits::THDMScan THDMLimits::Get2HDMmuScan(const TH2& GxBR_2hdm, const Limit& GxBR_95CL){
      /*
       * Method to make a Signal strength scan of 2HDM model for particular value of VAR
       */
      //Clone input histo
      TH2D exp = *(TH2D*) GxBR_2hdm.Clone();
      TH2D obs = *(TH2D*) GxBR_2hdm.Clone();
      double GxBR_exp  = GxBR_95CL.getMedian();
      double GxBR_obs  = GxBR_95CL.getObserved();

      //Expected
      exp.Scale(1./GxBR_exp);
      //Observed
      obs.Scale(1./GxBR_obs);

      exp.SetTitle("#mu = (#sigma x BR)_{theory} / (#sigma x BR)_{95%C.L.}");
      exp.SetLabelSize(0.050,"XYZ");
      exp.SetTitleSize(0.055,"XYZ");

      THDMScan out;
      out.expected = (TH2D) exp;
      out.observed = (TH2D) obs;

      return out;
    }

    std::vector<Limit> THDMLimits::Get2HDM_Limits(const TH2& h_in, const Limit& GxBR_95CL, const double& min, const double& max){
      /*
       * Method to calculate 2HDM Limits
       * and plot them
       */

      if(TEST_){
	std::cout<<"START LimitsInterpretation::Get2HDM_Limits"<<std::endl;
      }

      //Clone input h
      TH2D GxBR_2hdm = *(TH2D*) h_in.Clone();
      GxBR_2hdm.GetXaxis()->SetRangeUser(min,max);
      double Limit_95CL[6];

      Limit_95CL[0] = GxBR_95CL.getObserved();
      Limit_95CL[1] = GxBR_95CL.getMinus2G();
      Limit_95CL[2] = GxBR_95CL.getMinus1G();
      Limit_95CL[3] = GxBR_95CL.getMedian();
      Limit_95CL[4] = GxBR_95CL.getPlus1G();
      Limit_95CL[5] = GxBR_95CL.getPlus2G();

      TCanvas canva("canva","",800,600);
      GxBR_2hdm.DrawCopy("colz");
      GxBR_2hdm.SetContour(6,Limit_95CL);
      GxBR_2hdm.Draw("cont Z LIST");
      canva.Update();

      // Get Contours
      TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
      gROOT->GetListOfSpecials()->Print();

      if (conts == nullptr){
	std::logic_error("ERROR in LimitsInterpretation::Get2HDM_Limits: *** No Contours Were Extracted!");
      }

      TGraph *innerBand_up 	= nullptr;
      TGraph *innerBand_down = nullptr;
      TGraph *outerBand_up = nullptr;
      TGraph *outerBand_down = nullptr;
      TGraph *expected = nullptr;
      TGraph *observed = nullptr;

      outerBand_down	= (TGraph*) ((TList*)conts->At(1))->First();
      innerBand_down	= (TGraph*) ((TList*)conts->At(2))->First();
      expected		= (TGraph*) ((TList*)conts->At(3))->First();
      innerBand_up 	= (TGraph*) ((TList*)conts->At(4))->First();
      outerBand_up 	= (TGraph*) ((TList*)conts->At(5))->First();
      observed		= (TGraph*) ((TList*)conts->At(0))->First();

      //Test:
      for(int i = 0; i < 6; ++i){
	TCanvas canva("canva","",800,600);
	GxBR_2hdm.DrawCopy("colz");
	double val = Limit_95CL[i];
	GxBR_2hdm.SetContour(1,&val);
	GxBR_2hdm.Draw("cont Z LIST");
	canva.Update();

	conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
	if(i==0) observed		= (TGraph*) ((TList*)conts->At(0))->First()->Clone();
	else if (i == 1) outerBand_down	= (TGraph*) ((TList*)conts->At(0))->First()->Clone();
	else if (i == 2) innerBand_down	= (TGraph*) ((TList*)conts->At(0))->First()->Clone();
	else if (i == 3) expected	= (TGraph*) ((TList*)conts->At(0))->First()->Clone();
	else if (i == 4) innerBand_up 	= (TGraph*) ((TList*)conts->At(0))->First()->Clone();
	else if (i == 5) outerBand_up 	= (TGraph*) ((TList*)conts->At(0))->First()->Clone();
      }

      if(!outerBand_down || !outerBand_up ){
	std::logic_error("ERROR in LimitsInterpretation::Get2HDM_Limits: ***No 2G up or Down contours***");
      }
      if(!innerBand_down || !innerBand_up){
	std::logic_error("ERROR in LimitsInterpretation::Get2HDM_Limits: ***No 1G up or Down contours***");
      }
      //Define future binning:
      const int np = 201;

      double step = (max - min) / (np-1);
      //limit map:
      limits_.clear();
      // 	std::vector<Limit> limits_2hdm;
      //get values of limits:
      double in_up[np+1], in_down[np+1], exp[np+1], x[np+1], out_up[np+1], out_down[np+1], obs[np+1];
      for(int i = 0;i < np;++i) {
	x[i] = min + i*step;
	exp[i] = expected->Eval(x[i]);
	obs[i] = observed->Eval(x[i]);
	in_up[i] = innerBand_up->Eval(x[i]);
	in_down[i] = innerBand_down->Eval(x[i]);
	out_up[i] = outerBand_up->Eval(x[i]);
	out_down[i] = outerBand_down->Eval(x[i]);

	Limit l;
	l.setX(x[i]);
	l.setMedian(exp[i]);
	l.setObserved(obs[i]);
	l.setMinus1G(in_down[i]);
	l.setMinus2G(out_down[i]);
	l.setPlus1G(in_up[i]);
	l.setPlus2G(out_up[i]);
	limits_.push_back(l);

	//		std::cout<<"WTF: x = "<<x[i]<<" obs = "<<obs[i]<<" exp = "<<exp[i]<<" -1G = "<<in_down[i]<<" -2G = "<<out_down[i]<<std::endl;
      }

      if(TEST_){
	std::cout<<" LimitsInterpretation::Get2HDM_Limits - DONE"<<std::endl;
      }
      // 	limits_.clear();
      // 	limits_ = limits_2hdm;
      return limits_;
    }

    const std::vector<Limit> THDMLimits::Get2HDM_1D_Limits(const TH2& h_in){
      /*
       * Method to get 2HDM 1D interpretation of GxBR limits
       */
      //Clone input h
      TH2D GxBR_2hdm = *(TH2D*) h_in.Clone();
      // Clear limit vectors for safety
      limits_.clear();
      for(unsigned int i = 0; i != GxBr_limits_.size(); ++i){
	Limit gxbr_limit = GxBr_limits_.at(i);
	Limit tan_b_limit;
	//		Set Mass point
	tan_b_limit.setX(gxbr_limit.getX());
	//		Set limits
	tan_b_limit.setMinus2G(double(THDMTanBeta(GxBR_2hdm,tan_b_limit.getX(),gxbr_limit.getMinus2G())));
	tan_b_limit.setMinus1G(double(THDMTanBeta(GxBR_2hdm,tan_b_limit.getX(),gxbr_limit.getMinus1G())));
	tan_b_limit.setMedian(double(THDMTanBeta(GxBR_2hdm,tan_b_limit.getX(),gxbr_limit.getMedian())));
	tan_b_limit.setPlus1G(double(THDMTanBeta(GxBR_2hdm,tan_b_limit.getX(),gxbr_limit.getPlus1G())));
	tan_b_limit.setPlus2G(double(THDMTanBeta(GxBR_2hdm,tan_b_limit.getX(),gxbr_limit.getPlus2G())));
	tan_b_limit.setObserved(double(THDMTanBeta(GxBR_2hdm,tan_b_limit.getX(),gxbr_limit.getObserved())));
	limits_.push_back(tan_b_limit);
	//		std::cout<<"m = "<<tan_b_limit.getX()<<" Observed: "<<tan_b_limit.getObserved()<<" Expected: "<<tan_b_limit.getMedian()<<std::endl;
      }
      return limits_;
    }

    const std::vector<Limit> THDMLimits::Get2HDM_1D_Limits(const std::string& benchmark_path, const double& var, const std::string& axis){
      /*
       * Method to get 2HDM 1D interpretation of GxBR limits
       */
      //Get 3D limits
      auto h3D = Get2HDM_GxBR_3D(benchmark_path);
      //Call overloaded method to get 2D limits
      auto h2D =  Get2HDM_GxBR_2D(h3D,var,axis);
      //get 1D limits for VAR
      limits_ = Get2HDM_1D_Limits(h2D);
      return limits_;
    }

    double THDMLimits::THDMTanBeta(const TH2& h_in, double mA, double xsection){
      /*
       * Method to translate GxBR limits to tanBeta limits
       */
      //Clone histo
      TH2D GxBR_2hdm = *(TH2D*) h_in.Clone();
      double minimalDifference = 1e+10;
      bool rangeExceeded = true;
      double tanBetaTarget = -1;
      double xsecTarget    = -1;
      double maxTanBeta = 100;
      double minTanBeta = 1;
      int ibmax = 10*int(maxTanBeta-minTanBeta);
      for (int ib=0; ib<ibmax; ++ib) {

	double tanBeta = minTanBeta + 0.1*double(ib);

	//		std::cout<<""
	double totXSec = GxBR_2hdm.Interpolate(mA,tanBeta);
	double difference = TMath::Abs(totXSec-xsection);

	if (difference<minimalDifference) {
	  minimalDifference = difference;
	  tanBetaTarget = tanBeta;
	  xsecTarget = totXSec;
	}

	if (totXSec>xsection) {
	  rangeExceeded = false;
	  break;
	}

      }

      if (rangeExceeded)
	tanBetaTarget = tanBetaTarget*TMath::Sqrt(xsection/xsecTarget);

      return tanBetaTarget;
    }

    void THDMLimits::SetTHDMHistos(TFile& f,std::map<std::string,TH3D*>& histos){
      /*
       * Method to set map of 2HDM histograms according to boson name
       */
      if(boson_ == "both"){
	histos["xs_bbA"] = GetFromTFile<TH3D>(f,"xs_bbA");
	histos["br_Abb"] = GetFromTFile<TH3D>(f,"br_Abb");

	histos["xs_bbH"] = GetFromTFile<TH3D>(f,"xs_bbH");
	histos["br_Hbb"] = GetFromTFile<TH3D>(f,"br_Hbb");
      }
      else if (boson_ == "three"){
	histos["xs_bbA"] = GetFromTFile<TH3D>(f,"xs_bbA");
        histos["br_Abb"] = GetFromTFile<TH3D>(f,"br_Abb");

	histos["xs_bbH"] = GetFromTFile<TH3D>(f,"xs_bbH");
        histos["br_Hbb"] = GetFromTFile<TH3D>(f,"br_Hbb");

	histos["xs_bbh"] = GetFromTFile<TH3D>(f,"xs_bbh");
        histos["br_hbb"] = GetFromTFile<TH3D>(f,"br_hbb");
      }
      else{
	histos["xs_bb" + boson_] = GetFromTFile<TH3D>(f,"xs_bb" + boson_);
	histos["br_" + boson_ + "bb"] = GetFromTFile<TH3D>(f,"br_" + boson_ + "bb");	
      }
    }

    void THDMLimits::AddPlottingObjects(TH2F &frame, TLegend &leg, TGraph& obs, TGraph& exp, TGraphAsymmErrors& inner_band, TGraphAsymmErrors& outer_band, TCanvas &can){
      /*
       * Virtual function that is actually plotting objects on the frame
       */

      /*
       * Appearence of observed limits.
       * For the model dependent interpretation - there is a line, shows the central value
       * and shaded area with fully invisible border. Needed to not cover the axis on top
       * Also - an artificial TGraph is created to go the TLegend object with both - line and area
       * proper colored/painted
       */

      //Modify appearence of the observed data to match 8 TeV paper
      obs.SetLineColor(kRed);

      //Shaded area with transperent borders
      TGraph *obs_shaded = new TGraph(obs);
      obs_shaded->SetLineColor(kWhite);
      obs_shaded->SetLineWidth(0);
      obs_shaded->SetFillColor(kRed-9);
      obs_shaded->SetFillColorAlpha(kRed-9,0.35);
      obs_shaded->SetPoint(obs_shaded->GetN(),xMax_,yMax_);		//Needed to make a closed area to be filled
      obs_shaded->SetPoint(obs_shaded->GetN(),xMin_,yMax_);		//Needed to make a closed area to be filled

      //Artificial TGraph to be used in the TLegend
      TGraph *obs_legend = new TGraph();
      obs_legend->SetLineColor(obs.GetLineColor());
      obs_legend->SetLineWidth(obs.GetLineWidth());
      obs_legend->SetFillColorAlpha(obs_shaded->GetFillColor(), 0.35);


      //Modify appearence of the observed data to match 8 TeV paper
      //	obs.SetLineColor(kRed);
      //	obs.SetFillColorAlpha(kRed-9,0.35);
      //	obs.SetPoint(obs.GetN(),xMax_,yMax_);
      //	obs.SetPoint(obs.GetN(),xMin_,yMax_);

      if (!blindData_){
	obs_shaded->Draw("FLsame");
	//		obs.Draw("FLsame");
	obs.Draw("Lsame");
	leg.AddEntry(obs_legend,"Observed","lf");
      }
	
      //Expected limits style
      exp.SetLineColor(kBlue);

      //Legend style
      auto legendHeader = scenario_->getLabel();
      leg.SetHeader(legendHeader.c_str());
	
      if(compareWithPrevious_ != ""){
	//ATLAS Results
	scenario_->checkResultsToCompareWith(compareWithPrevious_);
	auto atlas_results = scenario_->getPreviousResults(var_axis_,compareWithPrevious_);
	std::vector<TGraph*> patlas_results;
	//Draw and add Legend
	for(auto & gr : atlas_results){
	  //ATLAS style
	  TGraph *p_gr = new TGraph(gr);
	  //p_gr->SetFillStyle(3002);
	  p_gr->SetFillColor(kAzure-4);
	  p_gr->SetFillColorAlpha(kAzure-4,0.35);
	  if(var_axis_=="z") p_gr->Draw("LFsame");//CFsame
	  else p_gr->Draw("LFsame");
	  patlas_results.push_back(p_gr);
			
	}
	leg.AddEntry(&inner_band,"68% expected","f");
	//leg.AddEntry(patlas_results.front(),"A#rightarrow Zh, ATLAS","f");
	leg.AddEntry(patlas_results.front(),"A#rightarrow Zh, CMS","f");
	leg.AddEntry(&exp,"Expected","l");
	leg.AddEntry(&outer_band,"95% expected","f");
      }
      else {
	leg.AddEntry(&inner_band,"68% expected","f");
	leg.AddEntry(&exp,"Expected","l");
	leg.AddEntry(&outer_band,"95% expected","f");
      }

      HbbStyle::drawLegendOnTopOfThePad(&leg,&can,0.25);
      //Get Legend position to not lost it
      if(legendHeader.length() > 20) {
	std::string specific_point = "A/H#rightarrow b#bar{b}, ";
	if(var_axis_=="x") specific_point += "m_{A/H} = " + std::to_string(int(var_point_)) + " GeV";
	else if(var_axis_ == "z") specific_point += "cos(#beta-#alpha) = " + to_string_with_precision(var_point_, 1);
	legendHeader += ", " + specific_point;
	leg.SetHeader(legendHeader.c_str());
	HbbStyle::drawLegendSplittedHeader(&leg,",");
      }
      can.cd();
      HbbStyle::drawStandardTitle("out");

      frame.GetYaxis()->SetMoreLogLabels();
    }


  } /* namespace mssmhbb */
} /* namespace analysis */
