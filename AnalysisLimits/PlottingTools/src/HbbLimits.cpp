/*
 * HbbLimits.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/HbbLimits.h"

#include "TCanvas.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"

namespace analysis {
  namespace mssmhbb {

    HbbLimits::HbbLimits() :
      blindData_(true),
      TEST_(false),
      xMin_(250),
      xMax_(1700),
      yMin_(0.1),
      yMax_(30)
    {
    };
    HbbLimits::HbbLimits(const bool& blindData, const double& xMin, const double& xMax, const double& yMin, const double& yMax, const bool& test) :
      blindData_(blindData),
      TEST_(test),
      xMin_(xMin),
      xMax_(xMax),
      yMin_(yMin),
      yMax_(yMax) {
    };

    HbbLimits::~HbbLimits() {
      // TODO Auto-generated destructor stub
    }

    void HbbLimits::ReadCombineLimits(const std::string& file_name){
      /*
       * Method to read Limits calculated with Asymptotic method in the combine tool
       * and written to the file.
       */
      //	input stream:
      std::ifstream inputList(file_name);
      if(is_empty(inputList)) std::logic_error("ERROR in HbbLimits::ReadCombineLimits. WRONG INPUT FILE NAME");
      //	TFile name
      std::string tfileName;
      std::vector<Limit> GxBr_limits{};
      //	Loop over the lines in the input file:
      while (inputList >> tfileName) {
	Limit limit = ReadCombineLimit(tfileName,blindData_);
	GxBr_limits.push_back(limit);
      }
      setLimits(GxBr_limits);
    }

    void HbbLimits::LimitPlotter(
				 TLegend leg,
				 const std::string& output,
				 const std::string& Lumi,
				 const std::string& xtitle,
				 const std::string& ytitle,
				 const bool& logY){
      /*
       * Method to avoid null_vec creation by user
       */
      LimitsToCompare differ_limits;
      LimitPlotter(differ_limits,leg,output,Lumi,xtitle,ytitle,logY);
    }

    void HbbLimits::LimitPlotter(
				 const std::string& output,
				 const std::string& Lumi,
				 const std::string& xtitle,
				 const std::string& ytitle,
				 const bool& logY){
      /*
       * Method to avoid null_vec creation by user
       */
      LimitsToCompare differ_limits;
      //	TLegend leg(0.62,0.6,0.9,0.85);
      gStyle->SetPadLeftMargin(0.13);
      gStyle->SetPadRightMargin(0.05);
      TLegend &leg = *HbbStyle::legend("top,right",5);
      LimitPlotter(differ_limits,leg,output,Lumi,xtitle,ytitle,logY);
    }

    void HbbLimits::LimitPlotter(const LimitsToCompare& differ_limits,
				 TLegend leg,
				 const std::string& output,
				 const std::string& Lumi,
				 const std::string& xtitle,
				 const std::string& ytitle,
				 const bool& logY){


      if(limits_.size() == 0) {
	std::cerr<<"Error in HbbLimits::LimitPlotter: No limits with this name. Please check spelling";
	exit(-1);
      }
      HbbStyle style;
      //	gStyle->SetPadTopMargin   (0.08);
      gStyle->SetPadLeftMargin(0.13);
      gStyle->SetPadRightMargin(0.05);

      //Write value of limits in file
      //	CheckOutputDir(output);
      //	Write(output);

      int nPointsX = limits_.size();
      HbbLimits::DecomposedLimits decomposed = DecomposeLimits(limits_);

      TGraph * differ_exp = nullptr;
      bool compare_limits = false;
      if(differ_limits.limits.size() != 0) compare_limits = true;
      std::vector<double> X_2;
      std::vector<double> median_2;
      if(compare_limits){
	for(const auto& l: differ_limits.limits){
	  X_2.push_back(l.getX());
	  median_2.push_back(l.getMedian());
	}
	differ_exp = new TGraph(differ_limits.limits.size(),X_2.data(),median_2.data());
	differ_exp->SetLineWidth(3);
	differ_exp->SetLineColor(1);
	differ_exp->SetLineStyle(3);
      }

      TGraph * obsG = new TGraph(nPointsX, decomposed.X.data(), decomposed.obs.data());
      style.setObservedLimitsStyle(obsG);

      TGraph * expG = new TGraph(nPointsX, decomposed.X.data(), decomposed.median.data());
      style.setExpectedLimitsStyle(expG);

      TGraphAsymmErrors * innerBand = new TGraphAsymmErrors(nPointsX, decomposed.X.data(), decomposed.median.data(), decomposed.zero.data(), decomposed.zero.data(), decomposed.minus1.data(), decomposed.plus1.data());
      style.set1SigmaBandsStyle(innerBand);

      TGraphAsymmErrors * outerBand = new TGraphAsymmErrors(nPointsX, decomposed.X.data(), decomposed.median.data(), decomposed.zero.data(), decomposed.zero.data(), decomposed.minus2.data(), decomposed.plus2.data());
      style.set2SigmaBandsStyle(outerBand);

      TH2F frame("frame","",2,xMin_,xMax_,2,yMin_,yMax_);
      style.setFrameStyle(&frame);
      frame.GetXaxis()->SetTitle(xtitle.c_str());
      frame.GetYaxis()->SetTitle(ytitle.c_str());

      TCanvas *canv = new TCanvas("canv", "histograms", 600, 600);
      auto *main_pad = new TPad("main_pad","main_pad",0,0,1,1);
      main_pad->Draw();
      main_pad->cd();

      frame.Draw();

      style.setLegendStyle(&leg);

      outerBand->Draw("3same");
      innerBand->Draw("3same");
      expG->Draw("lsame");
      if(compare_limits) differ_exp->Draw("lsame");

      if(compare_limits) {
	leg.AddEntry(differ_exp,differ_limits.legend.c_str(),"l");
      }
      AddPlottingObjects(frame,leg,*obsG,*expG,*innerBand,*outerBand,*canv);
      //	TPad * pad = (TPad*)canv->GetPad(0);
      canv->RedrawAxis();
      main_pad->RedrawAxis();
      ////	leg.Draw();
      //style.drawStandardTitle();

      canv->cd();
      if(logY) main_pad->SetLogy();
      canv->Update();
      canv->Print( (output+".pdf").c_str() ,"Portrait pdf");
      canv->Print( (output+".png").c_str()) ;
      //	canv->Print( (output+".root").c_str()) ;
    }

    void HbbLimits::LimitPlotter(TLegend leg,
				 const std::string& differ_limits,
				 const std::string& output,
				 const std::string& Lumi,
				 const std::string& xtitle,
				 const std::string& ytitle,
				 const bool& logY){


      if(limits_.size() == 0) {
	std::cerr<<"Error in HbbLimits::LimitPlotter: No limits with this name. Please check spelling";
	exit(-1);
      }
      if(differ_limits != "1617CMS"){
	std::cerr<<"Calling this function with a string to compare is only designed for intput '1617CMS'";
	exit(-1);
      }
      HbbStyle style;
      //	gStyle->SetPadTopMargin   (0.08);
      gStyle->SetPadLeftMargin(0.13);
      gStyle->SetPadRightMargin(0.05);

      //Write value of limits in file
      //	CheckOutputDir(output);
      //	Write(output);

      int nPointsX = limits_.size();
      HbbLimits::DecomposedLimits decomposed = DecomposeLimits(limits_);

      TGraph * obsG = new TGraph(nPointsX, decomposed.X.data(), decomposed.obs.data());
      style.setObservedLimitsStyle(obsG);

      TGraph * expG = new TGraph(nPointsX, decomposed.X.data(), decomposed.median.data());
      style.setExpectedLimitsStyle(expG);

      TGraphAsymmErrors * innerBand = new TGraphAsymmErrors(nPointsX, decomposed.X.data(), decomposed.median.data(), decomposed.zero.data(), decomposed.zero.data(), decomposed.minus1.data(), decomposed.plus1.data());
      style.set1SigmaBandsStyle(innerBand);

      TGraphAsymmErrors * outerBand = new TGraphAsymmErrors(nPointsX, decomposed.X.data(), decomposed.median.data(), decomposed.zero.data(), decomposed.zero.data(), decomposed.minus2.data(), decomposed.plus2.data());
      style.set2SigmaBandsStyle(outerBand);

      TH2F frame("frame","",2,xMin_,xMax_,2,yMin_,yMax_);
      style.setFrameStyle(&frame);
      frame.GetXaxis()->SetTitle(xtitle.c_str());
      frame.GetYaxis()->SetTitle(ytitle.c_str());

      TCanvas *canv = new TCanvas("canv", "histograms", 600, 600);
      auto *main_pad = new TPad("main_pad","main_pad",0,0,1,1);
      main_pad->Draw();
      main_pad->cd();

      frame.Draw();

      style.setLegendStyle(&leg);

      outerBand->Draw("3same");
      innerBand->Draw("3same");
      expG->Draw("lsame");

      TGraph* art_17 = new TGraph();
      art_17 -> SetLineStyle(2);
      art_17 -> SetLineColor(kGray);

      TGraph* art_16 = new TGraph();
      art_16 -> SetLineStyle(2);
      art_16 -> SetLineColor(kBlack);

      leg.AddEntry(art_16,"2016 Exp.","l");
      leg.AddEntry(art_17,"2017 SL Exp.","l");
      
      AddPlottingObjects(frame,leg,*obsG,*expG,*innerBand,*outerBand,*canv);
      //	TPad * pad = (TPad*)canv->GetPad(0);
      canv->RedrawAxis();
      main_pad->RedrawAxis();
      ////	leg.Draw();
      //style.drawStandardTitle();

      canv->cd();
      if(logY) main_pad->SetLogy();
      canv->Update();
      canv->Print( (output+".pdf").c_str() ,"Portrait pdf");
      canv->Print( (output+".png").c_str()) ;
      //	canv->Print( (output+".root").c_str()) ;
    }

    void HbbLimits::AddPlottingObjects(TH2F &frame, TLegend &leg, TGraph& obs, TGraph& exp, TGraphAsymmErrors& inner_band, TGraphAsymmErrors& outer_band, TCanvas &can){
      /*
       * Virtual function that is actually plotting objects on the frame
       */
      frame.GetXaxis()->SetRangeUser(frame.GetXaxis()->GetXmin(), frame.GetXaxis()->GetXmax()-0.001);
      leg.SetHeader("95% CL upper limits");
      if (!blindData_){
	obs.Draw("lpsame");
	leg.AddEntry(&obs,"Observed","lp");
      }

      leg.AddEntry(&exp,"Median expected","l");
      leg.AddEntry(&inner_band,"68% expected","f");
      leg.AddEntry(&outer_band,"95% expected","f");
      leg.Draw();

      HbbStyle::drawStandardTitle("out");

    }

    const Limit HbbLimits::ReadCombineLimit(const std::string& tfile_name, const bool& blindData){
      /*
       * Method to get information about limit from root file
       */
      TFile file(tfile_name.c_str(),"read");
      //	Check whether file is Ok.
      CheckZombie(file);
      //	Get TTree from this file
      TTree& tree = *((TTree*)file.Get("limit"));
      double LIMIT;
      tree.SetBranchAddress("limit",&LIMIT);
      double MH;
      tree.SetBranchAddress("mh",&MH);
      //	Get iinformation from TTree:
      Limit limit;

      tree.GetEntry(0);
      limit.setX(double(MH));
      limit.setMinus2G(double(LIMIT));

      tree.GetEntry(1);
      limit.setMinus1G(double(LIMIT));

      tree.GetEntry(2);
      limit.setMedian(double(LIMIT));

      tree.GetEntry(3);
      limit.setPlus1G(double(LIMIT));

      tree.GetEntry(4);
      limit.setPlus2G(double(LIMIT));

      tree.GetEntry(5);
      limit.setObserved(double(LIMIT));
      //    if(blindData) limit.setObserved(limit.getExpected());

      return limit;
    }

    void HbbLimits::PlotSubRangeSteps(
				      const std::vector<Limit>& limits_sr1,
				      const std::vector<Limit>& limits_sr2,
				      const std::vector<Limit>& limits_sr3,
				      const std::vector<Limit>& limits_sr4,
				      const std::string& output,
				      const std::string& Lumi,
				      const std::string& xtitle,
				      const std::string& ytitle,
				      const bool& logY,
				      const bool& comp
				      ){
      /*
       * Method to plot sub-ranges step plots of GxBr
       */
      if(limits_sr1.size() == 0 || limits_sr2.size() == 0 || limits_sr3.size() == 0 || limits_sr4.size() == 0) {
	std::cerr<<"Error in HbbLimits::PlotSubRangeSteps: No limits with this name. Please check spelling";
	exit(-1);
      }
      HbbStyle style;

      //	gStyle->SetPadTopMargin(0.08);
      gStyle->SetPadLeftMargin(0.13);
      gStyle->SetPadRightMargin(0.05);

      //Sub-range 1
      int nPoints_sr1 = limits_sr1.size();
      HbbLimits::DecomposedLimits decomposed_sr1 = DecomposeLimits(limits_sr1);

      TGraph obsG_sr1(nPoints_sr1, decomposed_sr1.X.data(), decomposed_sr1.obs.data());
      style.setObservedLimitsStyle(&obsG_sr1);

      //Adjust style according to the Francisco's comments
      TGraph obsG_sr1_points(nPoints_sr1-1, decomposed_sr1.X.data(), decomposed_sr1.obs.data());	// Duplicate of the SR1 limits with N-1 points
      style.setObservedLimitsStyle(&obsG_sr1_points);

      TGraph expG_sr1(nPoints_sr1, decomposed_sr1.X.data(), decomposed_sr1.median.data());
      style.setExpectedLimitsStyle(&expG_sr1);

      TGraphAsymmErrors innerBand_sr1(nPoints_sr1, decomposed_sr1.X.data(), decomposed_sr1.median.data(), decomposed_sr1.zero.data(), decomposed_sr1.zero.data(), decomposed_sr1.minus1.data(), decomposed_sr1.plus1.data());
      style.set1SigmaBandsStyle(&innerBand_sr1);

      TGraphAsymmErrors outerBand_sr1(nPoints_sr1, decomposed_sr1.X.data(), decomposed_sr1.median.data(), decomposed_sr1.zero.data(), decomposed_sr1.zero.data(), decomposed_sr1.minus2.data(), decomposed_sr1.plus2.data());
      style.set2SigmaBandsStyle(&outerBand_sr1);

      //Sub-range 2
      int nPoints_sr2 = limits_sr2.size();
      HbbLimits::DecomposedLimits decomposed_sr2 = DecomposeLimits(limits_sr2);

      TGraph obsG_sr2(nPoints_sr2, decomposed_sr2.X.data(), decomposed_sr2.obs.data());
      style.setObservedLimitsStyle(&obsG_sr2);
      //	obsG_sr2.SetMarkerStyle(22);

      //Adjust style according to the Francisco's comments
      TGraph obsG_sr2_points(nPoints_sr2-1, decomposed_sr2.X.data(), decomposed_sr2.obs.data());	// Duplicate of the SR1 limits with N-1 points
      style.setObservedLimitsStyle(&obsG_sr2_points);

      TGraph expG_sr2(nPoints_sr2, decomposed_sr2.X.data(), decomposed_sr2.median.data());
      style.setExpectedLimitsStyle(&expG_sr2);

      TGraphAsymmErrors innerBand_sr2(nPoints_sr2, decomposed_sr2.X.data(), decomposed_sr2.median.data(), decomposed_sr2.zero.data(), decomposed_sr2.zero.data(), decomposed_sr2.minus1.data(), decomposed_sr2.plus1.data());
      style.set1SigmaBandsStyle(&innerBand_sr2);

      TGraphAsymmErrors outerBand_sr2(nPoints_sr2, decomposed_sr2.X.data(), decomposed_sr2.median.data(), decomposed_sr2.zero.data(), decomposed_sr2.zero.data(), decomposed_sr2.minus2.data(), decomposed_sr2.plus2.data());
      style.set2SigmaBandsStyle(&outerBand_sr2);

      //Sub-range 3
      int nPoints_sr3 = limits_sr3.size();
      HbbLimits::DecomposedLimits decomposed_sr3 = DecomposeLimits(limits_sr3);

      TGraph obsG_sr3(nPoints_sr3, decomposed_sr3.X.data(), decomposed_sr3.obs.data());
      style.setObservedLimitsStyle(&obsG_sr3);
      //	obsG_sr3.SetMarkerStyle(22);

      //Adjust style according to the Francisco's comments
      TGraph obsG_sr3_points(nPoints_sr3-1, decomposed_sr3.X.data(), decomposed_sr3.obs.data());	// Duplicate of the SR1 limits with N-1 points
      style.setObservedLimitsStyle(&obsG_sr3_points);

      TGraph expG_sr3(nPoints_sr3, decomposed_sr3.X.data(), decomposed_sr3.median.data());
      style.setExpectedLimitsStyle(&expG_sr3);

      TGraphAsymmErrors innerBand_sr3(nPoints_sr3, decomposed_sr3.X.data(), decomposed_sr3.median.data(), decomposed_sr3.zero.data(), decomposed_sr3.zero.data(), decomposed_sr3.minus1.data(), decomposed_sr3.plus1.data());
      style.set1SigmaBandsStyle(&innerBand_sr3);

      TGraphAsymmErrors outerBand_sr3(nPoints_sr3, decomposed_sr3.X.data(), decomposed_sr3.median.data(), decomposed_sr3.zero.data(), decomposed_sr3.zero.data(), decomposed_sr3.minus2.data(), decomposed_sr3.plus2.data());
      style.set2SigmaBandsStyle(&outerBand_sr3);

      //Sub-range 4
      int nPoints_sr4 = limits_sr4.size();
      HbbLimits::DecomposedLimits decomposed_sr4 = DecomposeLimits(limits_sr4);

      TGraph obsG_sr4(nPoints_sr4, decomposed_sr4.X.data(), decomposed_sr4.obs.data());
      style.setObservedLimitsStyle(&obsG_sr4);
      //	obsG_sr4.SetMarkerStyle(44);

      TGraph expG_sr4(nPoints_sr4, decomposed_sr4.X.data(), decomposed_sr4.median.data());
      style.setExpectedLimitsStyle(&expG_sr4);

      TGraphAsymmErrors innerBand_sr4(nPoints_sr4, decomposed_sr4.X.data(), decomposed_sr4.median.data(), decomposed_sr4.zero.data(), decomposed_sr4.zero.data(), decomposed_sr4.minus1.data(), decomposed_sr4.plus1.data());
      style.set1SigmaBandsStyle(&innerBand_sr4);

      TGraphAsymmErrors outerBand_sr4(nPoints_sr4, decomposed_sr4.X.data(), decomposed_sr4.median.data(), decomposed_sr4.zero.data(), decomposed_sr4.zero.data(), decomposed_sr4.minus2.data(), decomposed_sr4.plus2.data());
      style.set2SigmaBandsStyle(&outerBand_sr4);


      TH2F frame("frame","",2,xMin_,xMax_,2,yMin_,yMax_);
      style.setFrameStyle(&frame);
      frame.GetXaxis()->SetTitle(xtitle.c_str());
      frame.GetYaxis()->SetTitle(ytitle.c_str());

      TCanvas canv("canv", "histograms", 600, 600);

      frame.Draw();

      //TLegend leg(0.62,0.6,0.9,0.85);
      auto &leg = *HbbStyle::legend("top,right",8,0.45);
      style.setLegendStyle(&leg);
      leg.SetBorderSize(0);

      //Draw SR1
      outerBand_sr1.Draw("3same");
      innerBand_sr1.Draw("3same");
      expG_sr1.Draw("lsame");
      //Draw SR2
      outerBand_sr2.Draw("3same");
      innerBand_sr2.Draw("3same");
      expG_sr2.Draw("lsame");
      //Draw SR3
      outerBand_sr3.Draw("3same");
      innerBand_sr3.Draw("3same");
      expG_sr3.Draw("lsame");
      //Draw SR4
      outerBand_sr4.Draw("3same");
      innerBand_sr4.Draw("3same");
      expG_sr4.Draw("lsame");

      if (!blindData_){
	obsG_sr1.Draw("lsame");
	obsG_sr1_points.Draw("psame");
	obsG_sr2.Draw("lsame");
	obsG_sr2_points.Draw("psame");
	obsG_sr3.Draw("lsame");
	obsG_sr3_points.Draw("psame");
	obsG_sr4.Draw("lpsame");
	//		leg.AddEntry(&obsG_sr1," ","lp");
	leg.AddEntry(&obsG_sr2,"Observed","lp");
	//		leg.AddEntry(&obsG_sr3," ","lp");
      }

      leg.SetHeader("95% CL upper limits");
      //	leg.AddEntry(&expG_sr1,"Expected","l");
      leg.AddEntry(&expG_sr1,"Median expected","l");
      leg.AddEntry(&innerBand_sr1,"68% expected","f");
      leg.AddEntry(&outerBand_sr1,"95% expected","f");

      if(comp){
	std::cout << "Plotting comparisons" << std::endl;
	//2016 inclusinve
	const int npoints = 9;
	double x [] = {300,350,400,500,600,700,900,1100,1300};
	double y_exp [] = {19.7,11.7,6.7,3.1,1.9,1.3,0.8,0.68,0.68};
	TGraph* gr_exp = new TGraph(npoints,x,y_exp);
	gr_exp->SetLineStyle(2);
	gr_exp->SetLineWidth(2);
	gr_exp->SetLineColor(kCyan-3);
	gr_exp->Draw("lsame");
	leg.AddEntry(gr_exp,"2016 exp.","l");
	//std::array<double,npoints> y_obs = {{19.1,14.0,5.7,1.9,2.1,1.5,0.9,0.4,0.5}};
	//TGraph gr_obs(npoints,&x[0],&y_obs[0]);

	//2017 semi leptonic
	const int npoints_17 = 7;
	double x_17 [] = {300,350,400,450,500,600,700};
	double y_exp_17 [] = {28.59,15.81,11.53,7.47,6.28,3.95,3.32};
	TGraph* gr_exp_17 = new TGraph(npoints_17,x_17,y_exp_17);
	gr_exp_17->SetLineStyle(2);
	gr_exp_17->SetLineWidth(2);
	gr_exp_17->SetLineColor(kMagenta);
	gr_exp_17->Draw("lsame");
	leg.AddEntry(gr_exp_17,"2017SL exp.","l");
	//std::array<double,npoints_17> y_obs_17 = {{27.02,13.95,14.37,9.61,4.80,6.01,2.54}};
	//TGraph gr_obs_17(npoints_17,&x_17[0],&y_obs_17[0]);

	//ATLAS
	const int npoints_ATLAS = 17;
	std::array<double,npoints_ATLAS> x_ATLAS = {{450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1350,1400}};
	std::array<double,npoints_ATLAS> y_exp_ATLAS = {{3.30,2.79,2.15,1.60,1.32,1.17,1.07,1.01,0.90,0.89,0.86,0.76,0.77,0.61,0.56,0.56,0.52}};
	TGraph* gr_exp_ATLAS = new TGraph(17,&x_ATLAS[0],&y_exp_ATLAS[0]);
	gr_exp_ATLAS->SetLineStyle(2);
	gr_exp_ATLAS->SetLineWidth(2);
	gr_exp_ATLAS->SetLineColor(kBlue);
	gr_exp_ATLAS->Draw("lsame");
	leg.AddEntry(gr_exp_ATLAS,"ATLAS exp.","l");
	//std::array<double,npoints_17> y_obs_ATLAS = {{3.46,3.63,3.48,2.29,2.54,0.94,0.70,1.03,0.98,0.63,0.82,0.79,0.75,0.58,1.06,1.20,0.98}};
	//TGraph gr_obs_ATLAS(npoints_17,&x_ATLAS[0],&y_obs_ATLAS[0]);
      }


      //Draw Lines according to Francisco's comment
      //At 400 GeV
      TLine line_400(400,yMin_,400,12.9);
      line_400.SetLineStyle(2);
      line_400.Draw();

      //At 700 GeV
      TLine line_700(700,yMin_,700,2.5);
      line_700.SetLineStyle(2);
      line_700.Draw();

      //At 1000 GeV
      TLine line_1000(1000,yMin_,1000,1.0);
      line_1000.SetLineStyle(2);
      line_1000.Draw();

      TPad * pad = (TPad*)canv.GetPad(0);
      pad->RedrawAxis();
      leg.Draw();
      //style.drawStandardTitle("out");
      //style.drawStandardTitle();

      if(logY) canv.SetLogy();
      canv.Update();
      canv.Print( (output+".pdf").c_str() ,"Portrait pdf");
      canv.Print( (output+".png").c_str()) ;
    }

    void HbbLimits::Write(const std::string& name){
      /*
       * Method to write limits to a .txt file
       */
      std::fstream fs;
      fs.open( (name + ".txt").c_str(), std::fstream::out );
      fs<<"X     -2s     -1s  median     +1s     +2s     obs \n";
      for(const auto& limit: limits_){
	char strOut[400];
	//		sprintf(strOut,"%4f  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f",
	sprintf(strOut,"%4f & %6.2f & %6.2f & %6.2f & %6.2f & %6.2f & %6.2f",
		(limit.getX()),limit.getMinus2G(),limit.getMinus1G(),limit.getExpected(),limit.getPlus1G(),limit.getPlus2G(),limit.getObserved());
	fs<<strOut;
	fs<<"\n";
      }
      fs<<"\n\n";

      // Style for the latex tables
      for(const auto& limit: limits_){
	char strOut[400];
	sprintf(strOut,"%i & %6.1f & %6.1f & %6.1f & %6.1f & %6.1f & %6.1f \\",(int(limit.getX())),limit.getMinus2G(),limit.getMinus1G(),limit.getExpected(),limit.getPlus1G(),limit.getPlus2G(),limit.getObserved());
	fs<<strOut;
	fs<<"\n";
      }
      fs.close();
      std::cout<<"File: "<<name + ".txt"<<" has been written."<<std::endl;
    }

    HbbLimits::DecomposedLimits HbbLimits::DecomposeLimits(const std::vector<Limit>& v){
      /*
       * Method to ddecompose Vector of limits to vectorS of individual quantities
       */
      DecomposedLimits dec;
      for(const auto& l : v){
	dec.X.push_back(l.getX());
	dec.obs.push_back(l.getObserved());
	dec.median.push_back(l.getMedian());
	dec.minus2.push_back(l.getMedian() - l.getMinus2G());
	dec.minus1.push_back(l.getMedian() - l.getMinus1G());
	dec.plus2.push_back(l.getPlus2G() - l.getMedian());
	dec.plus1.push_back(l.getPlus1G() - l.getMedian());
	dec.zero.push_back(0);
      }
      return dec;
    }

  } /* namespace MssmHbb */
} /* namespace Analysis */

