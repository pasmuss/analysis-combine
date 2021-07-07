/*
 * LHCXSGScenarious.cpp
 *
 *  Created on: 18 Aug 2017
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/LHCXSGScenarious.h"
#include "TText.h"

namespace analysis {
  namespace mssmhbb {

    std::string AvailableScenariosToString(AvailableScenarios scenario){
      /*
       * Translate scenario code to a string
       */
      if(scenario == MHMODP_200) return "mhmodp_200";
      else if(scenario == LIGHT_STOP) return "light_stop";
      else if(scenario == LIGHT_STAU) return "light_stau";
      else if(scenario == HMSSM) return "hMSSM";
      else if(scenario == TAU_PHOBIC) return "tau_phobic";
      else if(scenario == TYPE2) return "type2";
      else if(scenario == TYPE1) return "type1";
      else if(scenario == LEPTON_SPECIFIC) return "lepton_specific";
      else if(scenario == FLIPPED) return "flipped";
      else if(scenario == MH125) return "mh125";
      else if(scenario == NEGMU1) return "neg_mu_1";
      else if(scenario == NEGMU2) return "neg_mu_2";
      else if(scenario == NEGMU3) return "neg_mu_3";
      else throw std::logic_error("ERROR at LHCXSGScenarious::AvailableScenariosToString. Wrong Scenario ");
    }

    AvailableScenarios AvailableScenariosFromString(const std::string& scenario_string){
      /*
       * Translate string to scenario
       */
	 
      if(findStrings(scenario_string,"mhmodp_200")) return MHMODP_200;
      else if(findStrings(scenario_string,"light_stop")) return LIGHT_STOP;
      else if(findStrings(scenario_string,"light_stau")) return LIGHT_STAU;
      else if(findStrings(scenario_string,"hMSSM")) return HMSSM;
      else if(findStrings(scenario_string,"tau_phobic")) return TAU_PHOBIC;
      else if(scenario_string == "type1") return TYPE1;
      else if(findStrings(scenario_string,"type2")) return TYPE2;
      else if(findStrings(scenario_string,"flipped")) return FLIPPED;
      else if(findStrings(scenario_string,"lepton_specific")) return LEPTON_SPECIFIC;
      else if(findStrings(scenario_string,"mh125")) return MH125;
      else if(findStrings(scenario_string,"neg_mu_1")) return NEGMU1;
      else if(findStrings(scenario_string,"neg_mu_2")) return NEGMU2;
      else if(findStrings(scenario_string,"neg_mu_3")) return NEGMU3;
      else throw std::logic_error("ERROR at LHCXSGScenarious::AvailableScenariosFromString. No scenario: " + scenario_string);
    }

    std::unique_ptr<Scenario> Scenario::Create(AvailableScenarios scenario){
      /*
       * Realisation of a factory pattern
       */
      if(scenario == MHMODP_200) return std::unique_ptr<mhmodp_200>(new mhmodp_200());
      else if(scenario == LIGHT_STOP) return std::unique_ptr<light_stop>(new light_stop());
      else if(scenario == LIGHT_STAU) return std::unique_ptr<light_stau>(new light_stau());
      else if(scenario == HMSSM) return std::unique_ptr<hMSSM>(new hMSSM());
      else if(scenario == TAU_PHOBIC) return std::unique_ptr<tau_phobic>(new tau_phobic());
      else if(scenario == TYPE2) return std::unique_ptr<type2>(new type2());
      else if(scenario == TYPE1) return std::unique_ptr<type1>(new type1());
      else if(scenario == LEPTON_SPECIFIC) return std::unique_ptr<lepton_specific>(new lepton_specific());
      else if(scenario == FLIPPED) return std::unique_ptr<flipped>(new flipped());
      else if(scenario == MH125) return std::unique_ptr<mh125>(new mh125());
      else if(scenario == NEGMU1) return std::unique_ptr<negmu1>(new negmu1());
      else if(scenario == NEGMU2) return std::unique_ptr<negmu2>(new negmu2());
      else if(scenario == NEGMU3) return std::unique_ptr<negmu3>(new negmu3());
      else throw std::logic_error("ERROR at LHCXSGScenarious::Create. Wrong Scenario: " + AvailableScenariosToString(scenario));
    }

    mhmodp_200::mhmodp_200(){
      std::vector<std::string> availableResults = {"CMS-AN-2013-229v10","HIG-16-018","2017-AN-118"};
      availableResults_ = availableResults;
    }

    light_stop::light_stop(){
      std::vector<std::string> availableResults = {"CMS-AN-2013-229v10"};
      availableResults_ = availableResults;
    }

    light_stau::light_stau(){
      std::vector<std::string> availableResults = {"CMS-AN-2013-229v10"};
      availableResults_ = availableResults;
    }

    tau_phobic::tau_phobic(){
      std::vector<std::string> availableResults = {"CMS-AN-2013-229v10"};
      availableResults_ = availableResults;
    }

    type2::type2(){
      std::vector<std::string> availableResults = {"ATLAS-CONF-2017-055","1502.04478","HIG-18-005"};
      availableResults_ = availableResults;
    }

    type1::type1(){
      std::vector<std::string> availableResults = {};
      availableResults_ = availableResults;
    }

    lepton_specific::lepton_specific(){
      std::vector<std::string> availableResults = {};
      availableResults_ = availableResults;
    }

    flipped::flipped(){
      std::vector<std::string> availableResults = {"ATLAS-CONF-2017-055","1502.04478","HIG-18-005"};
      availableResults_ = availableResults;
    }

    hMSSM::hMSSM(){
      std::vector<std::string> availableResults = {"HIG-16-018","2017-AN-118"};
      availableResults_ = availableResults;
    }

    mh125::mh125(){
      std::vector<std::string> availableResults = {};
      availableResults_ = availableResults;
    }

    negmu1::negmu1(){
      std::vector<std::string> availableResults = {};
      availableResults_ = availableResults;
    }

    negmu2::negmu2(){
      std::vector<std::string> availableResults = {};
      availableResults_ = availableResults;
    }

    negmu3::negmu3(){
      std::vector<std::string> availableResults = {};
      availableResults_ = availableResults;
    }

    void Scenario::checkResultsToCompareWith(const std::string& pas){
      /*
       * Set PAS/CONF-NOTE/ARXIV to compare with
       */
      if(std::find(availableResults_.begin(),availableResults_.end(),pas) != availableResults_.end()){
	//		results_to_compare = pas;
      }
      else{
	std::stringstream msg;
	msg << "Error in LHCXSGScenario::setResultsToCompareWith: no results from " << pas << " available. Please consider to use one of the pre-defined: "<<boost::algorithm::join(availableResults_,", ");
	throw std::logic_error(msg.str());
      }
    }

    std::vector<TGraph> mhmodp_200::getPreviousResults(const std::string& PAS) const{
      /*
       * Previous results from 2016 (full hadronic/inclusive) and 2017 semi leptonic
       */
      std::vector<TGraph> results;
      const int npoints = 7;
      //Expected 2016 full hadronic
      std::array<double,npoints> x = {{300,350,400,500,600,700,900}};
      std::array<double,npoints> y_exp = {{26,28,29,33,41,51,80}};
      TGraph gr_exp(7,&x[0],&y_exp[0]);
      gr_exp.SetLineStyle(2);
      results.push_back(gr_exp);
      //2017 semi leptonic
      const int npoints_17 = 6;
      std::array<double,npoints_17> x_17 = {{300,350,400,450,500,600}};
      std::array<double,npoints_17> y_exp_17 = {{31,32,37,40,50,62}};
      TGraph gr_exp_17(6,&x_17[0],&y_exp_17[0]);
      gr_exp_17.SetLineColor(kGray);
      gr_exp_17.SetLineStyle(2);
      results.push_back(gr_exp_17);

      //Observed
      //std::array<double,npoints> y_obs = {{25.5,31,27,27.5,42,55,85}};
      //TGraph gr_obs(npoints,&x[0],&y_obs[0]);
      //gr_obs.SetLineColor(kGray);
      //results.push_back(gr_obs);

      //std::array<double,npoints_17> y_obs_17 = {{30,30.5,42,45,58,100}};
      //TGraph gr_obs_17(npoints_17,&x_17[0],&y_obs_17[0]);
      //gr_obs.SetLineColor(kBlack);
      //results.push_back(gr_obs_17);

      return results;
    }

    TText mhmodp_200::getPreviousResultsLabel(const std::string& PAS) const{
      /*
       * Return a TText box with a sign for a previous results
       * when available
       */

      TText text(500,35,"");
      text.SetTextSize(gStyle->GetLabelSize("y")*0.7);
      text.SetTextAngle(52.5);

      return text;
    }

    std::vector<TGraph> light_stop::getPreviousResults(const std::string& PAS) const{
      /*
       * Previous results from 8 TeV according to AN
       */
      std::vector<TGraph> results;
      const int npoints = 7;
      //Expected
      std::array<double,npoints> x = {{100,140,160,200,300,350,400}};
      std::array<double,npoints> y_exp = {{24.7,24.3,22.1,24.2,32.7,41.4,58.6}};
      TGraph gr_exp(npoints,&x[0],&y_exp[0]);
      gr_exp.SetLineStyle(2);
      gr_exp.SetLineColor(kBlack);
      results.push_back(gr_exp);

      //Observed
      std::array<double,npoints> y_obs = {{16.5,29.1,22.4,17.3,56.8,62.5,51.7}};
      TGraph gr_obs(npoints,&x[0],&y_obs[0]);
      gr_obs.SetLineColor(kBlack);
      results.push_back(gr_obs);

      return results;
    }

    TText light_stop::getPreviousResultsLabel(const std::string& PAS) const{
      /*
       * Return a TText box with a sign for a previous results
       * when available
       */
      //	TText text(370,39.0,"7+8 TeV expected");
      //	TText text(352,45.0,"7+8 TeV exp.");
      TText text(352,45.0,"7 + 8 TeV");
      text.SetTextSize(gStyle->GetLabelSize("y")*0.7);
      double angle = atan( 10 *(58.6 - 41.4)/(400-350) + 0.5) * 180. / TMath::Pi();
      text.SetTextAngle(angle);
      return text;
    }

    std::vector<TGraph> light_stau::getPreviousResults(const std::string& PAS) const{
      /*
       * Previous results from 8 TeV according to AN
       */
      std::vector<TGraph> results;
      const int npoints = 7;
      //Expected
      std::array<double,npoints> x = {{100,140,160,200,300,350,400}};
      std::array<double,npoints> y_exp = {{22.2,22.,20.,21.4,27.6,33.1,42.4}};
      TGraph gr_exp(npoints,&x[0],&y_exp[0]);
      gr_exp.SetLineStyle(2);
      gr_exp.SetLineColor(kBlack);
      results.push_back(gr_exp);

      //Observed
      std::array<double,npoints> y_obs = {{15.4,25.7,20.2,15.9,41.4,44.1,39.0}};
      TGraph gr_obs(npoints,&x[0],&y_obs[0]);
      gr_obs.SetLineColor(kBlack);
      results.push_back(gr_obs);

      return results;
    }

    TText light_stau::getPreviousResultsLabel(const std::string& PAS) const{
      /*
       * Return a TText box with a sign for a previous results
       * when available
       */
      //	TText text(350,33.6,"7+8 TeV exp.");
      //	TText text(369,38,"7+8 TeV exp.");
      TText text(390,42,"7+8 TeV");
      text.SetTextSize(gStyle->GetLabelSize("y")*0.7);
      double angle = atan( 10 *(42.4 - 33.1)/(400-350) + 0.25) * 180. / TMath::Pi();
      text.SetTextAngle(angle);
      return text;
    }

    std::vector<TGraph> hMSSM::getPreviousResults(const std::string& PAS) const{
      /*
       * Previous results from 8 TeV according to AN
       */
      std::vector<TGraph> results;
      const int npoints = 7;
      //Expected 2016 full hadronic
      std::array<double,npoints> x = {{300,350,400,500,600,700,900}};
      std::array<double,npoints> y_exp = {{22.5,23,23,25,31,35,58}};
      TGraph gr_exp(7,&x[0],&y_exp[0]);
      gr_exp.SetLineStyle(2);
      results.push_back(gr_exp);

      std::cout << "hMSSM: getting previous results" << std::endl;

      //2017 semi leptonic
      const int npoints_17 = 7;
      std::array<double,npoints_17> x_17 = {{300,350,400,450,500,600,700}};
      std::array<double,npoints_17> y_exp_17 = {{26,26.5,30,32,37,45,61}};
      TGraph gr_exp_17(7,&x_17[0],&y_exp_17[0]);
      gr_exp_17.SetLineStyle(2);
      gr_exp_17.SetLineColor(kGray);
      results.push_back(gr_exp_17);

      //Observed
      //std::array<double,npoints> y_obs = {{22.5,25.5,22,20,32,39,63}};
      //TGraph gr_obs(npoints,&x[0],&y_obs[0]);
      //gr_obs.SetLineColor(kGray);
      //results.push_back(gr_obs);

      //std::array<double,npoints_17> y_obs_17 = {{25.5,25,34,35,42,56,53}};
      //TGraph gr_obs_17(npoints_17,&x_17[0],&y_obs_17[0]);
      //gr_obs.SetLineColor(kBlack);
      //results.push_back(gr_obs_17);

      return results;
    }

    TText hMSSM::getPreviousResultsLabel(const std::string& PAS) const{
      /*
       * Return a TText box with a sign for a previous results
       * when available
       */
      TText text(450,35,"");
      text.SetTextSize(gStyle->GetLabelSize("y")*0.7);
      text.SetTextAngle(52.5);

      return text;
    }
  
    std::vector<TGraph> tau_phobic::getPreviousResults(const std::string& PAS) const{
      /*
       * Previous results from 8 TeV according to AN
       */
      std::vector<TGraph> results;
      const int npoints = 5;

      //Expected
      std::array<double,npoints> x = {{100,140,160,200,300}};
      std::array<double,npoints> y_exp = {{25.2,33.0,25.3,28.9,43.8}};
      TGraph gr_exp(npoints,&x[0],&y_exp[0]);
      gr_exp.SetLineStyle(2);
      gr_exp.SetLineColor(kBlack);
      results.push_back(gr_exp);

      //Observed
      std::array<double,npoints> y_obs = {{16.4,66.5,25.7,19.3,82}};
      TGraph gr_obs(npoints,&x[0],&y_obs[0]);
      gr_obs.SetLineColor(kBlack);
      results.push_back(gr_obs);

      return results;
    }

    TText tau_phobic::getPreviousResultsLabel(const std::string& PAS) const{
      /*
       * Return a TText box with a sign for a previous results
       * when available
       */
      //	TText text(300,44.3,"7+8 TeV exp.");
      TText text(300,44.3,"7+8 TeV");
      text.SetTextSize(gStyle->GetLabelSize("y")*0.7);
      double angle = atan( 5 *(43.8 - 28.9)/(400-350)) * 180. / TMath::Pi();
      text.SetTextAngle(angle);
      return text;
    }

    std::vector<TGraph> type1::getPreviousResults(const std::string& var,const std::string& PAS) const{
      /*
       * ATLAS results according to the scenario.
       */
      std::vector<TGraph> gr;
      return gr;
    }

    std::vector<TGraph> lepton_specific::getPreviousResults(const std::string& var,const std::string& PAS) const{
      /*
       * ATLAS results according to the scenario.
       */
      std::vector<TGraph> gr;
      return gr;
    }

    std::vector<TGraph> mh125::getPreviousResults(const std::string& PAS) const{
      std::vector<TGraph> gr;
      return gr;
    }

    TText mh125::getPreviousResultsLabel(const std::string& PAS) const{
      TText text(500,35,"");
      text.SetTextSize(gStyle->GetLabelSize("y")*0.7);
      text.SetTextAngle(52.5);
      return text;
    }

    std::vector<TGraph> negmu1::getPreviousResults(const std::string& PAS) const{
      std::vector<TGraph> gr;
      return gr;
    }

    TText negmu1::getPreviousResultsLabel(const std::string& PAS) const{
      TText text(500,35,"");
      text.SetTextSize(gStyle->GetLabelSize("y")*0.7);
      text.SetTextAngle(52.5);
      return text;
    }

    std::vector<TGraph> negmu2::getPreviousResults(const std::string& PAS) const{
      std::vector<TGraph> gr;
      return gr;
    }

    TText negmu2::getPreviousResultsLabel(const std::string& PAS) const{
      TText text(500,35,"");
      text.SetTextSize(gStyle->GetLabelSize("y")*0.7);
      text.SetTextAngle(52.5);
      return text;
    }

    std::vector<TGraph> negmu3::getPreviousResults(const std::string& PAS) const{
      std::vector<TGraph> gr;
      return gr;
    }

    TText negmu3::getPreviousResultsLabel(const std::string& PAS) const{
      TText text(500,35,"");
      text.SetTextSize(gStyle->GetLabelSize("y")*0.7);
      text.SetTextAngle(52.5);
      return text;
    }

    std::vector<TGraph> type2::getPreviousResults(const std::string& var,const std::string& PAS) const{
      /*
       * ATLAS results according to the scenario and VAR (dedicated to the specific axis)
       */
      std::vector<TGraph> grs;
      std::string lowerCase_var = var;
      std::transform(lowerCase_var.begin(),lowerCase_var.end(),lowerCase_var.begin(),::tolower);
      /*
       * If 8 TeV Analysis
       */
      if(PAS == "1502.04478")
	{
	  if(lowerCase_var == "z"){
	    std::vector<std::pair<double,double>> lower_left= {
	      std::make_pair(220,0.5),
	      std::make_pair(225,1.21),
	      std::make_pair(230,1.68),
	      std::make_pair(235,1.95),
	      std::make_pair(240,2.18),
	      std::make_pair(245,2.27),
	      std::make_pair(250,2.36),
	      std::make_pair(255,2.50),
	      std::make_pair(260,2.59),
	      std::make_pair(265,2.64),
	      std::make_pair(270,2.68),
	      std::make_pair(275,2.77),
	      std::make_pair(280,2.82),
	      std::make_pair(285,2.86),
	      std::make_pair(290,2.91),
	      std::make_pair(295,2.95),
	      std::make_pair(300,3.00),
	      std::make_pair(305,3.10),
	      std::make_pair(310,3.12),
	      std::make_pair(315,3.19),
	      std::make_pair(320,3.25),
	      std::make_pair(325,3.38),
	      std::make_pair(330,3.50),
	      std::make_pair(335,3.63),
	      std::make_pair(337,3.69),
	      std::make_pair(340,3.50),
	      std::make_pair(345,3.13),
	      std::make_pair(350,0.5)
	    };

	    std::vector<double> cos_lower_left, tan_lower_left;
	    for(const auto& val : lower_left) {
	      cos_lower_left.push_back(val.first);
	      tan_lower_left.push_back(val.second);
	    }

	    TGraph fl_lower_left(cos_lower_left.size(),cos_lower_left.data(),tan_lower_left.data());
	    grs.push_back(fl_lower_left);
	  }
	  else if (lowerCase_var == "x"){
	    //LEFT
	    std::vector<std::pair<double,double>> curve_left= {
	      std::make_pair(-0.9,50), // ADDITIONAL POINT TO DRAW

	      std::make_pair(-0.35,50),
	      std::make_pair(-0.32,32),
	      std::make_pair(-0.35,15),
	      std::make_pair(-0.4,10),
	      std::make_pair(-0.42,8),
	      std::make_pair(-0.4,7),
	      std::make_pair(-0.35,6.2),
	      std::make_pair(-0.32,5.9),
	      std::make_pair(-0.3,5.7),
	      std::make_pair(-0.25,5.2),
	      std::make_pair(-0.2,4.8),
	      std::make_pair(-0.15,4.1),
	      std::make_pair(-0.1,3.5),
	      std::make_pair(-0.05,2.5),
	      std::make_pair(0.0,0.5),

	      std::make_pair(-0.9,0.5),	//ADDITIONAL POINT TO DRAW
	    };

	    std::vector<double> cos_left, tan_left;
	    for(const auto& val : curve_left) {
	      cos_left.push_back(val.first);
	      tan_left.push_back(val.second);
	    }

	    TGraph fl_left(cos_left.size(),cos_left.data(),tan_left.data());
	    grs.push_back(fl_left);

	    //LOWER RIGHT
	    std::vector<std::pair<double,double>> curve_lright = {
	      std::make_pair(0.0,0.5),		//ADDITIONAL POINT TO DRAW

	      std::make_pair(0.05,2.4),
	      std::make_pair(0.1,3.0),
	      std::make_pair(0.15,3.1),
	      std::make_pair(0.2,2.9),
	      std::make_pair(0.25,2.75),
	      std::make_pair(0.3,2.4),
	      std::make_pair(0.35,2.1),
	      std::make_pair(0.4,1.9),
	      std::make_pair(0.45,1.7),
	      std::make_pair(0.5,1.6),
	      std::make_pair(0.55,1.5),
	      std::make_pair(0.6,1.32),
	      std::make_pair(0.65,1.16),
	      std::make_pair(0.7,0.97),
	      std::make_pair(0.75,0.85),
	      std::make_pair(0.8,0.7),
	      std::make_pair(0.85,0.6),
	      std::make_pair(0.88,0.5),

	    };

	    std::vector<double> cos_lright, tan_lright;
	    for(const auto& val : curve_lright) {
	      cos_lright.push_back(val.first);
	      tan_lright.push_back(val.second);
	    }

	    TGraph fl_lright(cos_lright.size(),cos_lright.data(),tan_lright.data());
	    grs.push_back(fl_lright);

	    //UPPER RIGHT
	    std::vector<std::pair<double,double>> curve_hright= {
	      std::make_pair(0.9,50),		//ADDITIONAL POINT TO DRAW
	      std::make_pair(0.35,50),
	      std::make_pair(0.31,32),
	      std::make_pair(0.35,15),
	      std::make_pair(0.4,12),
	      std::make_pair(0.45,7.5),
	      std::make_pair(0.4,5.7),
	      std::make_pair(0.37,4.5),
	      std::make_pair(0.4,3.5),
	      std::make_pair(0.45,2.7),
	      std::make_pair(0.5,2.3),
	      std::make_pair(0.55,1.7),
	      std::make_pair(0.6,1.53),
	      std::make_pair(0.65,1.26),
	      std::make_pair(0.7,1.11),
	      std::make_pair(0.75,0.92),
	      std::make_pair(0.8,0.8),
	      std::make_pair(0.85,0.65),
	      std::make_pair(0.9,0.5)


	    };

	    std::vector<double> cos_hright, tan_hright;
	    for(const auto& val : curve_hright) {
	      cos_hright.push_back(val.first);
	      tan_hright.push_back(val.second);
	    }

	    TGraph fl_hright(cos_hright.size(),cos_hright.data(),tan_hright.data());
	    grs.push_back(fl_hright);
	  }
	  else{
	    throw std::invalid_argument("Error at LHCXSGScenario::type2::getAtlasResults axis " + var + " has not been defined");
	  }
	}
      else if (PAS == "HIG-18-005"){
	if(lowerCase_var == "z"){
	  //LOW LEFT
	  std::vector<std::pair<double,double>> lower_left= {
	    std::make_pair(200,0.1),
	    std::make_pair(200,1.2),
	    std::make_pair(205,2),
	    std::make_pair(210,2.1),
	    std::make_pair(215,1.9),
	    std::make_pair(220,2),
	    std::make_pair(230,2.1),
	    std::make_pair(240,2.2),
	    std::make_pair(260,2.5),
	    std::make_pair(280,2.5),
	    std::make_pair(300,3.2),
	    std::make_pair(310,4),
	    std::make_pair(315,4),
	    std::make_pair(320,3.8),
	    std::make_pair(330,3.8),
	    std::make_pair(335,4.5),
	    std::make_pair(340,1.6),
	    std::make_pair(340,0.48),
	    std::make_pair(338,0.15),
	    std::make_pair(330,0.1)
	  };
	  std::vector<double> cos_lower_left, tan_lower_left;
	  for(const auto& val : lower_left) {
	    cos_lower_left.push_back(val.first);
	    tan_lower_left.push_back(val.second);
	  }
	  TGraph fl_lower_left(cos_lower_left.size(),cos_lower_left.data(),tan_lower_left.data());
	  grs.push_back(fl_lower_left);
	 
	  //LOW CENTER LEFT
	  std::vector<std::pair<double,double>> lower_center_left= {
	    std::make_pair(440,1.8),
	    std::make_pair(470,1),
	    std::make_pair(475,0.42),
	    std::make_pair(470,0.2),
	    std::make_pair(430,0.38),
	    std::make_pair(430,1),
	    std::make_pair(440,0.15)
	  };
	  std::vector<double> cos_lower_center_left, tan_lower_center_left;
	  for(const auto& val : lower_center_left) {
	    cos_lower_center_left.push_back(val.first);
	    tan_lower_center_left.push_back(val.second);
	  }
	  TGraph fl_lower_center_left(cos_lower_center_left.size(),cos_lower_center_left.data(),tan_lower_center_left.data());
	  grs.push_back(fl_lower_center_left);

	  //LOW CENTER RIGHT
	  std::vector<std::pair<double,double>> lower_center_right= {
	    std::make_pair(580,2.45),
	    std::make_pair(540,1.7),
	    std::make_pair(540,0.15),
	    std::make_pair(600,0.25),
	    std::make_pair(605,0.4),
	    std::make_pair(605,0.7),
	    std::make_pair(600,1),
	    std::make_pair(580,2.45)
	  };
	  std::vector<double> cos_lower_center_right, tan_lower_center_right;
	  for(const auto& val : lower_center_right) {
	    cos_lower_center_right.push_back(val.first);
	    tan_lower_center_right.push_back(val.second);
	  }
	  TGraph fl_lower_center_right(cos_lower_center_right.size(),cos_lower_center_right.data(),tan_lower_center_right.data());
	  grs.push_back(fl_lower_center_right);

	  //LOW RIGHT
	  std::vector<std::pair<double,double>> lower_right= {
	    std::make_pair(810,0.19),
	    std::make_pair(810,1.02),
	    std::make_pair(840,0.9),
	    std::make_pair(840,0.24)
	  };
	  std::vector<double> cos_lower_right, tan_lower_right;
	  for(const auto& val : lower_right) {
	    cos_lower_right.push_back(val.first);
	    tan_lower_right.push_back(val.second);
	  }
	  TGraph fl_lower_right(cos_lower_right.size(),cos_lower_right.data(),tan_lower_right.data());
	  grs.push_back(fl_lower_right);

	  //TOP
	  std::vector<std::pair<double,double>> top= {
	    std::make_pair(410,100),
	    std::make_pair(410,40),
	    std::make_pair(415,30),
	    std::make_pair(430,21),
	    std::make_pair(460,21),
	    std::make_pair(500,23),
	    std::make_pair(520,32),
	    std::make_pair(540,19),
	    std::make_pair(580,18),
	    std::make_pair(600,19),
	    std::make_pair(610,19),
	    std::make_pair(680,20),
	    std::make_pair(700,100)
	  };
	  std::vector<double> cos_top, tan_top;
	  for(const auto& val : top) {
	    cos_top.push_back(val.first);
	    tan_top.push_back(val.second);
	  }
	  TGraph fl_top(cos_top.size(),cos_top.data(),tan_top.data());
	  grs.push_back(fl_top);
	}
	else if(lowerCase_var == "x"){
	  //LEFT
	  std::vector<std::pair<double,double>> left= {
	    std::make_pair(-0.164,100),
	    std::make_pair(-0.164,90),
	    std::make_pair(-0.164,80),
	    std::make_pair(-0.164,70),
	    std::make_pair(-0.164,60),
	    std::make_pair(-0.167,50),
	    std::make_pair(-0.169,40),
	    std::make_pair(-0.170,30),
	    std::make_pair(-0.176,20),
	    std::make_pair(-0.181,12),
	    std::make_pair(-0.188,10),
	    std::make_pair(-0.190,9),
	    std::make_pair(-0.188,8),
	    std::make_pair(-0.181,7),
	    std::make_pair(-0.167,6),
	    std::make_pair(-0.134,5),
	    std::make_pair(-0.093,4),
	    std::make_pair(-0.6,4),
	    std::make_pair(-0.6,100)
	  };
	  std::vector<double> cos_left, tan_left;
	  for(const auto& val : left) {
	    cos_left.push_back(val.first);
	    tan_left.push_back(val.second);
	  }
	  TGraph fl_left(cos_left.size(),cos_left.data(),tan_left.data());
	  grs.push_back(fl_left);

	  //RIGHT
	  std::vector<std::pair<double,double>> right= {
	    std::make_pair(0.158,100),
	    std::make_pair(0.158,90),
	    std::make_pair(0.158,80),
	    std::make_pair(0.158,70),
	    std::make_pair(0.159,60),
	    std::make_pair(0.161,50),
	    std::make_pair(0.165,40),
	    std::make_pair(0.167,30),
	    std::make_pair(0.170,20),
	    std::make_pair(0.194,12),
	    std::make_pair(0.218,10),
	    std::make_pair(0.230,9),
	    std::make_pair(0.239,8),
	    std::make_pair(0.255,7),
	    std::make_pair(0.274,6),
	    std::make_pair(0.290,5),
	    std::make_pair(0.337,4),
	    std::make_pair(0.5,4),
	    std::make_pair(0.5,100)
	  };
	  std::vector<double> cos_right, tan_right;
	  for(const auto& val : right) {
	    cos_right.push_back(val.first);
	    tan_right.push_back(val.second);
	  }
	  TGraph fl_right(cos_right.size(),cos_right.data(),tan_right.data());
	  grs.push_back(fl_right);
	}
	else throw std::invalid_argument("Error at LHCXSGScenario::type2::getAtlasResults axis " + var + " has not been defined (x or z for tanb vs cosb-a and tanb vs mA respectively)");
      }
      /*
       * 13 TeV results
       * ATLAS-CONF-2017-055
       */
      else if (PAS == "ATLAS-CONF-2017-055"){
	if(lowerCase_var == "z"){
	  std::vector<std::pair<double,double>> lower_left= {
	    std::make_pair(220,0.5),
	    std::make_pair(220,0.6),
	    std::make_pair(220,0.7),
	    std::make_pair(220,0.8),
	    std::make_pair(220,0.9),

	    std::make_pair(220,1.0),
	    std::make_pair(221,1.1),
	    std::make_pair(221.88,1.2),
	    std::make_pair(223.125,1.3),
	    std::make_pair(223.75,1.4),
	    std::make_pair(225.625,1.5),
	    std::make_pair(228.125,1.6),
	    std::make_pair(232.5,1.7),
	    std::make_pair(236.875,1.8),
	    std::make_pair(240,1.9),

	    std::make_pair(245,2.),
	    std::make_pair(249.375,2.1),
	    std::make_pair(255,2.2),
	    std::make_pair(263.75,2.3),
	    std::make_pair(266.25,2.4),
	    std::make_pair(271.25,2.5),
	    std::make_pair(273.75,2.6),
	    std::make_pair(275,2.7),
	    std::make_pair(276.25,2.8),
	    std::make_pair(280,2.9),

	    std::make_pair(282.5,3.0),
	    std::make_pair(300.0,3.6),
	    std::make_pair(320.0,3.3),

	    std::make_pair(330,4),
	    std::make_pair(334.375,4.5),
	    std::make_pair(340,4.7),

	    std::make_pair(349.375,4.5),
	    std::make_pair(351.875,4),

	    std::make_pair(357.5,3.6),
	    std::make_pair(358.125,3.3),
	    std::make_pair(360,3.),

	    std::make_pair(360,2.9),
	    std::make_pair(360.625,2.8),
	    std::make_pair(361.25,2.7),
	    std::make_pair(361.25,2.6),
	    std::make_pair(361.5,2.5),
	    std::make_pair(362,2.4),
	    std::make_pair(362.25,2.3),
	    std::make_pair(362.375,2.2),
	    std::make_pair(362.5,2.1),
	    std::make_pair(363.125,2.),

	    std::make_pair(363.5,1.9),
	    std::make_pair(363.75,1.8),
	    std::make_pair(363.75,1.7),
	    std::make_pair(364.375,1.6),
	    std::make_pair(363.75,1.5),
	    std::make_pair(363.75,1.4),
	    std::make_pair(363.75,1.3),
	    std::make_pair(363.125,1.2),
	    std::make_pair(362.5,1.1),
	    std::make_pair(361.25,1.0),

	    std::make_pair(360.625,0.9),
	    std::make_pair(360,0.8),
	    std::make_pair(358.125,0.7),
	    std::make_pair(355.625,0.6),
	    std::make_pair(351.875,0.5)
	  };

	  std::vector<double> cos_lower_left, tan_lower_left;
	  for(const auto& val : lower_left) {
	    cos_lower_left.push_back(val.first);
	    tan_lower_left.push_back(val.second);
	  }

	  std::vector<std::pair<double,double>> lower_right= {
	    {584.2,0.7},
	    {581.5,0.8},
	    {579.8,0.9},
	    {578.7,1},
	    {578.7,1.1},
	    {577.7,1.2},
	    {576,1.3},
	    {576,1.4},
	    {578.7,1.5},
	    {581.5,1.6},
	    {581.5,1.7},
	    {581.5,1.8},
	    {581.5,1.9},
	    {581.5,2.0},
	    {583.1,2.1},
	    {583.6,2.2},
	    {583.6,2.3},
	    {584.2,2.4},
	    {584.7,2.5},
	    {585.3,2.6},
	    {585.8,2.7},
	    {586.4,2.8},
	    {588.5,2.9},
	    {589.6,3.0},
	    {603.2,4.0},
	    {646.7,3.0},
	    {649.4,2.9},
	    {671.1,2.8},
	    {706.5,2.7},
	    {709.2,2.6},
	    {717.4,2.5},
	    {722.8,2.4},
	    {728.2,2.3},
	    {733.7,2.2},
	    {739.1,2.1},
	    {744.5,2.0},
	    {750.0,1.9},
	    //					{755.4,2.0},
	    //					{771.7,2.1},
	    //					{771.1,2.2},
	    //					{782.6,2.3},
	    //					{788,2.4},
	    {798.9,2.5},
	    {804.3,2.4},
	    {807,2.3},
	    {809.8,2.2},
	    {812.5,2.1},
	    {815.2,2.0},
	    {815.2,1.9},
	    {817.9,1.8},
	    {819,1.7},
	    {819,1.6},
	    {819,1.5},
	    {820.6,1.4},
	    {819.5,1.3},
	    {817.9,1.2},
	    {817.9,1.1},
	    {817.8,1.0},
	    {815.2,0.9},
	    {806.,0.8},
	    {800.0,0.72},
	    {747.2,0.7}
	  };

	  std::vector<double> cos_lower_right, tan_lower_right;
	  for(const auto& val : lower_right) {
	    cos_lower_right.push_back(val.first);
	    tan_lower_right.push_back(val.second);
	  }

	  std::vector<std::pair<double,double>> upper_right= {
	    {546.1,50.},
	    {546.1,40.},
	    {557.0,30.},
	    {578.7,23.5},
	    {589.6,20},
	    {600.5,19},
	    {616.8,20},
	    {644,23.6},
	    {709.2,23.5},
	    {739.1,24},
	    {752.7,23.6},
	    {771.7,24},
	    //					{796.2,20},
	    {796.2,19},
	    {817.9,20},
	    {850.5,23.5},
	    {850.5,24},
	    {883.1,30},
	    {894,40},
	    {904.9,50}
	  };

	  std::vector<double> cos_upper_right, tan_upper_right;
	  for(const auto& val : upper_right) {
	    cos_upper_right.push_back(val.first);
	    tan_upper_right.push_back(val.second);
	  }

	  TGraph fl_lower_left(cos_lower_left.size(),cos_lower_left.data(),tan_lower_left.data());
	  TGraph fl_lower_right(cos_lower_right.size(),cos_lower_right.data(),tan_lower_right.data());
	  TGraph fl_upper_right(cos_upper_right.size(),cos_upper_right.data(),tan_upper_right.data());
	  grs.push_back(fl_lower_left);
	  grs.push_back(fl_lower_right);
	  grs.push_back(fl_upper_right);
	}
	else if (lowerCase_var == "x"){
	  //LEFT
	  std::vector<std::pair<double,double>> curve_left= {
	    std::make_pair(-0.9,50), // ADDITIONAL POINT TO DRAW

	    std::make_pair(-0.195652174,50),
	    std::make_pair(-0.195652174,40),
	    std::make_pair(-0.188405797,30),
	    std::make_pair(-0.188405797,21),
	    std::make_pair(-0.188405797,20),
	    std::make_pair(-0.188405797,12),
	    std::make_pair(-0.195652174,11),
	    std::make_pair(-0.188405797,10),
	    std::make_pair(-0.188405797,9),
	    std::make_pair(-0.188405797,8),
	    std::make_pair(-0.188405797,7),
	    std::make_pair(-0.173913043,6),
	    std::make_pair(-0.15942029,5),
	    std::make_pair(-0.101449275,4),

	    std::make_pair(-0.9,4),	//ADDITIONAL POINT TO DRAW
	  };

	  std::vector<double> cos_left, tan_left;
	  for(const auto& val : curve_left) {
	    cos_left.push_back(val.first);
	    tan_left.push_back(val.second);
	  }

	  TGraph fl_left(cos_left.size(),cos_left.data(),tan_left.data());
	  grs.push_back(fl_left);

	  // RIGHT
	  std::vector<std::pair<double,double>> curve_hright= {
	    std::make_pair(0.9,50),		//ADDITIONAL POINT TO DRAW

	    std::make_pair(0.173913043,50),
	    std::make_pair(0.173913043,40),
	    std::make_pair(0.15942029,30),
	    std::make_pair(0.15942029,21),
	    std::make_pair(0.15942029,20),
	    std::make_pair(0.173913043,12),
	    std::make_pair(0.188405797,11),
	    std::make_pair(0.202898551,10),
	    std::make_pair(0.210144928,9),
	    std::make_pair(0.202898551,8),
	    std::make_pair(0.231884058,7),
	    std::make_pair(0.246376812,6),
	    std::make_pair(0.260869565,5),
	    std::make_pair(0.304347826,4),


	    std::make_pair(0.9,0.5)		//ADDITIONAL POINT TO DRAW


	  };

	  std::vector<double> cos_hright, tan_hright;
	  for(const auto& val : curve_hright) {
	    cos_hright.push_back(val.first);
	    tan_hright.push_back(val.second);
	  }

	  TGraph fl_hright(cos_hright.size(),cos_hright.data(),tan_hright.data());
	  grs.push_back(fl_hright);
	}

      }
      return grs;
    }

    std::vector<TGraph> flipped::getPreviousResults(const std::string& var,const std::string& PAS) const{
      /*
       * ATLAS results according to the scenario and VAR (dedicated to the specific axis)
       */
      std::vector<TGraph> grs;
      std::string lowerCase_var = var;
      std::transform(lowerCase_var.begin(),lowerCase_var.end(),lowerCase_var.begin(),::tolower);
      /*
       * If 8 TeV Analysis
       */
      if(PAS == "ATLAS-CONF-2017-055")
	{
	  if(lowerCase_var == "z"){
	    std::vector<std::pair<double,double>> lower_left= {
	      std::make_pair(220,0.5),
	      std::make_pair(220,0.6),
	      std::make_pair(220,0.7),
	      std::make_pair(220,0.8),
	      std::make_pair(220,0.9),

	      std::make_pair(220,1.0),
	      std::make_pair(220.625,1.1),
	      std::make_pair(221.875,1.2),
	      std::make_pair(222.5,1.3),
	      std::make_pair(223.75,1.4),
	      std::make_pair(225.625,1.5),
	      std::make_pair(226.875,1.6),
	      std::make_pair(231.25,1.7),
	      std::make_pair(233.125,1.8),
	      std::make_pair(238.125,1.9),

	      std::make_pair(244.375,2.),
	      std::make_pair(249.375,2.1),
	      std::make_pair(251.875,2.2),
	      std::make_pair(255.625,2.3),
	      std::make_pair(259.375,2.4),
	      std::make_pair(263.75,2.5),
	      std::make_pair(267.5,2.6),
	      std::make_pair(271.25,2.7),
	      std::make_pair(273.75,2.8),
	      std::make_pair(276.875,2.9),

	      std::make_pair(281.875,3.),
	      std::make_pair(283.125,3.1),
	      std::make_pair(284.375,3.3),
	      std::make_pair(295.625,3.4),
	      std::make_pair(299.375,3.5),
	      std::make_pair(320,3.41),

	      std::make_pair(330,4),
	      std::make_pair(336.25,4.5),
	      std::make_pair(340,4.7),

	      std::make_pair(346.25,4.5),
	      std::make_pair(350.625,4),

	      std::make_pair(355.625,3.5),
	      std::make_pair(356.875,3.4),
	      std::make_pair(357.25,3.3),
	      std::make_pair(358.125,3.1),
	      std::make_pair(359.375,3.),

	      std::make_pair(359.75,2.9),
	      std::make_pair(360.25,2.8),
	      std::make_pair(360.625,2.7),
	      std::make_pair(361.,2.6),
	      std::make_pair(361.25,2.5),
	      std::make_pair(361.25,2.4),
	      std::make_pair(361.5,2.3),
	      std::make_pair(361.875,2.2),
	      std::make_pair(362.5,2.1),
	      std::make_pair(362.5,2.),

	      std::make_pair(362.75,1.9),
	      std::make_pair(363.5,1.8),
	      std::make_pair(363.75,1.7),
	      std::make_pair(363.75,1.6),
	      std::make_pair(363.125,1.5),
	      std::make_pair(363.125,1.4),
	      std::make_pair(362.5,1.3),
	      std::make_pair(361.875,1.2),
	      std::make_pair(361.875,1.1),
	      std::make_pair(361.25,1.0),

	      std::make_pair(360.,0.9),
	      std::make_pair(359.375,0.8),
	      std::make_pair(358.75,0.7),
	      std::make_pair(357.5,0.6),
	      std::make_pair(355.,0.5)

	      //ADDITIONAL POINT TO DRAW!!!
	      //				std::make_pair(-0.95,0.5)
	    };

	    std::vector<double> cos_lower_left, tan_lower_left;
	    for(const auto& val : lower_left) {
	      cos_lower_left.push_back(val.first);
	      tan_lower_left.push_back(val.second);
	    }

	    std::vector<std::pair<double,double>> lower_right= {
	      {586.7,0.7},
	      {581.5,0.8},
	      {579.8,0.9},
	      {578.7,1},
	      {578.7,1.1},
	      {577.7,1.2},
	      {578.7,1.3},
	      {578.7,1.4},
	      {578.7,1.5},
	      {579.8,1.6},
	      {580.4,1.7},
	      {582.,1.8},
	      {582.6,1.9},
	      {584.2,2.0},
	      {584.7,2.1},
	      {585.3,2.2},
	      {585.8,2.3},
	      {586.4,2.4},
	      {586.9,2.5},
	      {586.9,2.6},
	      {589.6,2.7},
	      {600.5,4.0},
	      //						{638.5,3.0},
	      //						{639.1,2.9},
	      //						{640.2,2.8},
	      {641.3,2.7},
	      {698.3,2.6},
	      {730.9,2},
	      //						{736.4,1.9},
	      //						{747.2,1.8},
	      //						{747.2,1.7},
	      {752.7,1.6},
	      //						{755.4,1.7},
	      //						{758.1,1.8},
	      //						{774.4,1.9},
	      {777.1,2.0},
	      {801.6,2.5},
	      {807.6,2.0},
	      {808.1,1.9},
	      {809.8,1.8},
	      {812.5,1.7},
	      {812.5,1.6},
	      {815.2,1.5},
	      {817.9,1.4},
	      {817.9,1.3},
	      {817.9,1.2},
	      {815.2,1.1},
	      {812.5,1.0},
	      {809.8,0.9},
	      {801.6,0.8},
	      {801.4,0.79},
	      {750,0.79},
	      {730.9,0.71}
	    };

	    std::vector<double> cos_lower_right, tan_lower_right;
	    for(const auto& val : lower_right) {
	      cos_lower_right.push_back(val.first);
	      tan_lower_right.push_back(val.second);
	    }

	    std::vector<std::pair<double,double>> upper_right= {
	      {539.1,50.},
	      {539.5,40.},
	      {546.1,30.},
	      {548.9,24},
	      {600.5,18},
	      {649.4,21.5},
	      {698.3,20},
	      {722.8,21.5},
	      {801.6,18},
	      {877.7,24},
	      {915.7,30},
	      {948.4,40},
	      {997.3,35},
	      {1000,50}
	    };

	    std::vector<double> cos_upper_right, tan_upper_right;
	    for(const auto& val : upper_right) {
	      cos_upper_right.push_back(val.first);
	      tan_upper_right.push_back(val.second);
	    }

	    TGraph fl_lower_left(cos_lower_left.size(),cos_lower_left.data(),tan_lower_left.data());
	    TGraph fl_lower_right(cos_lower_right.size(),cos_lower_right.data(),tan_lower_right.data());
	    TGraph fl_upper_right(cos_upper_right.size(),cos_upper_right.data(),tan_upper_right.data());
	    grs.push_back(fl_lower_left);
	    grs.push_back(fl_lower_right);
	    grs.push_back(fl_upper_right);

	  }
	  else if (lowerCase_var == "x"){
	    //LEFT
	    std::vector<std::pair<double,double>> curve_left= {
	      std::make_pair(-0.9,50), // ADDITIONAL POINT TO DRAW

	      std::make_pair(-0.202898551,50),
	      std::make_pair(-0.188405797,40),
	      std::make_pair(-0.173913043,30),
	      std::make_pair(-0.173913043,21),
	      std::make_pair(-0.173913043,20),
	      std::make_pair(-0.184057971,12),
	      std::make_pair(-0.188405797,11),
	      std::make_pair(-0.188405797,10),
	      std::make_pair(-0.188405797,9),
	      std::make_pair(-0.18115942,8),
	      std::make_pair(-0.173913043,7),
	      std::make_pair(-0.152173913,6),
	      std::make_pair(-0.130434783,5),
	      std::make_pair(-0.094202899,4),

	      std::make_pair(-0.9,4),	//ADDITIONAL POINT TO DRAW
	    };

	    std::vector<double> cos_left, tan_left;
	    for(const auto& val : curve_left) {
	      cos_left.push_back(val.first);
	      tan_left.push_back(val.second);
	    }

	    TGraph fl_left(cos_left.size(),cos_left.data(),tan_left.data());
	    grs.push_back(fl_left);

	    // RIGHT
	    std::vector<std::pair<double,double>> curve_hright= {
	      std::make_pair(0.9,50),		//ADDITIONAL POINT TO DRAW

	      std::make_pair(0.173913043,50),
	      std::make_pair(0.152173913,40),
	      std::make_pair(0.144927536,30),
	      std::make_pair(0.152173913,21),
	      std::make_pair(0.152173913,20),
	      std::make_pair(0.15942029,12),
	      std::make_pair(0.173913043,11),
	      std::make_pair(0.195652174,10),
	      std::make_pair(0.202898551,9),
	      std::make_pair(0.217391304,8),
	      std::make_pair(0.231884058,7),
	      std::make_pair(0.253623188,6),
	      std::make_pair(0.268115942,5),
	      std::make_pair(0.304347826,4),


	      std::make_pair(0.9,4)		//ADDITIONAL POINT TO DRAW


	    };

	    std::vector<double> cos_hright, tan_hright;
	    for(const auto& val : curve_hright) {
	      cos_hright.push_back(val.first);
	      tan_hright.push_back(val.second);
	    }

	    TGraph fl_hright(cos_hright.size(),cos_hright.data(),tan_hright.data());
	    grs.push_back(fl_hright);
	  }
	}
      
      else if (PAS == "HIG-18-005"){
	if (lowerCase_var == "z"){
	  //LOW LEFT
	  std::vector<std::pair<double,double>> lower_left= {
	    std::make_pair(200,0.1),
	    std::make_pair(200,1.2),
	    std::make_pair(205,2.01),
	    std::make_pair(210,2.1),
	    std::make_pair(215,2),
	    std::make_pair(220,2),
	    std::make_pair(225,2.1),
	    std::make_pair(230,2.1),
	    std::make_pair(260,2.5),
	    std::make_pair(270,2.5),
	    std::make_pair(300,3.1),
	    std::make_pair(305,4),
	    std::make_pair(310,3.5),
	    std::make_pair(315,3.5),
	    std::make_pair(320,6),
	    std::make_pair(330,1.3),
	    std::make_pair(330,0.51),
	    std::make_pair(320,0.1)
	  };
	  std::vector<double> cos_lower_left, tan_lower_left;
	  for(const auto& val : lower_left) {
	    cos_lower_left.push_back(val.first);
	    tan_lower_left.push_back(val.second);
	  }
	  TGraph fl_lower_left(cos_lower_left.size(),cos_lower_left.data(),tan_lower_left.data());
	  grs.push_back(fl_lower_left);
	 
	  //LOW CENTER LEFT
	  std::vector<std::pair<double,double>> lower_center_left= {
	    std::make_pair(440,1.2),
	    std::make_pair(430,0.9),
	    std::make_pair(430,0.5),
	    std::make_pair(440,0.23),
	    std::make_pair(460,0.4),
	    std::make_pair(465,0.7),
	    std::make_pair(460,1)
	  };
	  std::vector<double> cos_lower_center_left, tan_lower_center_left;
	  for(const auto& val : lower_center_left) {
	    cos_lower_center_left.push_back(val.first);
	    tan_lower_center_left.push_back(val.second);
	  }
	  TGraph fl_lower_center_left(cos_lower_center_left.size(),cos_lower_center_left.data(),tan_lower_center_left.data());
	  grs.push_back(fl_lower_center_left);

	  //LOW CENTER RIGHT
	  std::vector<std::pair<double,double>> lower_center_right= {
	    std::make_pair(580,2),
	    std::make_pair(530,1.2),
	    std::make_pair(530,0.23),
	    std::make_pair(580,0.12),
	    std::make_pair(590,0.23),
	    std::make_pair(600,0.39),
	    std::make_pair(600,0.7),
	    std::make_pair(590,1.05)
	  };
	  std::vector<double> cos_lower_center_right, tan_lower_center_right;
	  for(const auto& val : lower_center_right) {
	    cos_lower_center_right.push_back(val.first);
	    tan_lower_center_right.push_back(val.second);
	  }
	  TGraph fl_lower_center_right(cos_lower_center_right.size(),cos_lower_center_right.data(),tan_lower_center_right.data());
	  grs.push_back(fl_lower_center_right);

	  //LOW RIGHT
	  std::vector<std::pair<double,double>> lower_right= {
	    std::make_pair(810,0.95),
	    std::make_pair(810,0.3),
	    std::make_pair(830,0.42),
	    std::make_pair(830,0.6)
	  };
	  std::vector<double> cos_lower_right, tan_lower_right;
	  for(const auto& val : lower_right) {
	    cos_lower_right.push_back(val.first);
	    tan_lower_right.push_back(val.second);
	  }
	  TGraph fl_lower_right(cos_lower_right.size(),cos_lower_right.data(),tan_lower_right.data());
	  grs.push_back(fl_lower_right);

	  //TOP
	  std::vector<std::pair<double,double>> top= {
	    std::make_pair(405,100),
	    std::make_pair(410,22),
	    std::make_pair(420,20),
	    std::make_pair(460,20),
	    std::make_pair(510,22),
	    std::make_pair(530,18),
	    std::make_pair(560,17),
	    std::make_pair(600,18),
	    std::make_pair(610,18),
	    std::make_pair(670,20),
	    std::make_pair(695,21),
	    std::make_pair(695,100)
	  };
	  std::vector<double> cos_top, tan_top;
	  for(const auto& val : top) {
	    cos_top.push_back(val.first);
	    tan_top.push_back(val.second);
	  }
	  TGraph fl_top(cos_top.size(),cos_top.data(),tan_top.data());
	  grs.push_back(fl_top);
	}

	else if(lowerCase_var == "x"){
	  //LEFT
	  std::vector<std::pair<double,double>> left= {
	    std::make_pair(-0.141,100),
	    std::make_pair(-0.141,90),
	    std::make_pair(-0.144,80),
	    std::make_pair(-0.147,70),
	    std::make_pair(-0.148,60),
	    std::make_pair(-0.148,50),
	    std::make_pair(-0.149,40),
	    std::make_pair(-0.152,30),
	    std::make_pair(-0.155,20),
	    std::make_pair(-0.165,12),
	    std::make_pair(-0.167,10),
	    std::make_pair(-0.171,9),
	    std::make_pair(-0.174,8),
	    std::make_pair(-0.165,7),
	    std::make_pair(-0.147,6),
	    std::make_pair(-0.124,5),
	    std::make_pair(-0.088,4),
	    std::make_pair(-0.5,4),
	    std::make_pair(-0.5,100)
	  };
	  std::vector<double> cos_left, tan_left;
	  for(const auto& val : left) {
	    cos_left.push_back(val.first);
	    tan_left.push_back(val.second);
	  }
	  TGraph fl_left(cos_left.size(),cos_left.data(),tan_left.data());
	  grs.push_back(fl_left);
		  
	  //RIGHT
	  std::vector<std::pair<double,double>> right= {
	    std::make_pair(0.141,100),
	    std::make_pair(0.141,90),
	    std::make_pair(0.141,80),
	    std::make_pair(0.141,70),
	    std::make_pair(0.141,60),
	    std::make_pair(0.146,50),
	    std::make_pair(0.149,40),
	    std::make_pair(0.153,30),
	    std::make_pair(0.159,20),
	    std::make_pair(0.182,12),
	    std::make_pair(0.206,10),
	    std::make_pair(0.221,9),
	    std::make_pair(0.235,8),
	    std::make_pair(0.247,7),
	    std::make_pair(0.276,6),
	    std::make_pair(0.300,5),
	    std::make_pair(0.341,4),
	    std::make_pair(0.5,4),
	    std::make_pair(0.5,100)
	  };
	  std::vector<double> cos_right, tan_right;
	  for(const auto& val : right) {
	    cos_right.push_back(val.first);
	    tan_right.push_back(val.second);
	  }
	  TGraph fl_right(cos_right.size(),cos_right.data(),tan_right.data());
	  grs.push_back(fl_right);
	}
      }
      else if (PAS == "1502.04478"){
	if (lowerCase_var == "z"){
	  std::vector<std::pair<double,double>> lower_left= {
	    std::make_pair(220,0.5),
	    std::make_pair(225,1),
	    std::make_pair(230,1.8),
	    std::make_pair(235,2.),
	    std::make_pair(240,2.3),
	    std::make_pair(245,2.5),
	    std::make_pair(250,2.7),
	    std::make_pair(255,2.8),
	    std::make_pair(260,2.9),
	    std::make_pair(265,2.95),
	    std::make_pair(270,3.),
	    std::make_pair(275,3.05),
	    std::make_pair(280,3.1),
	    std::make_pair(285,3.15),
	    std::make_pair(290,3.2),
	    std::make_pair(295,3.3),
	    std::make_pair(300,3.4),
	    std::make_pair(305,3.5),
	    std::make_pair(310,3.6),
	    std::make_pair(315,3.7),
	    std::make_pair(320,3.8),
	    std::make_pair(325,3.9),
	    std::make_pair(330,3.95),
	    std::make_pair(335,4),
	    std::make_pair(340,4),
	    std::make_pair(345,3.7),
	    std::make_pair(347.5,3),
	    std::make_pair(350,0.5),

	    //ADDITIONAL POINT TO DRAW!!!
	    //				std::make_pair(-0.95,0.5)
	  };

	  std::vector<double> cos_lower_left, tan_lower_left;
	  for(const auto& val : lower_left) {
	    cos_lower_left.push_back(val.first);
	    tan_lower_left.push_back(val.second);
	  }
	  TGraph fl_lower_left(cos_lower_left.size(),cos_lower_left.data(),tan_lower_left.data());

	  grs.push_back(fl_lower_left);
	}
	else if(lowerCase_var == "x"){
	  //LOWER LEFT
	  std::vector<std::pair<double,double>> lower_left= {
	    std::make_pair(-0.02,0.5),
	    std::make_pair(-0.021,0.6),
	    std::make_pair(-0.022,0.7),
	    std::make_pair(-0.023,0.8),
	    std::make_pair(-0.024,0.9),
	    std::make_pair(-0.025,1.),
	    std::make_pair(-0.026,1.3),
	    std::make_pair(-0.05,2.),
	    std::make_pair(-0.054,2.5),
	    std::make_pair(-0.1,3.),
	    std::make_pair(-0.15,3.5),
	    std::make_pair(-0.21,4),
	    std::make_pair(-0.3,4.5),
	    std::make_pair(-0.45,5),
	    std::make_pair(-0.64,5.5),
	    std::make_pair(-0.95,6),

	    //ADDITIONAL POINT TO DRAW!!!
	    std::make_pair(-0.95,0.5)
	  };

	  std::vector<double> cos_lower_left, tan_lower_left;
	  for(const auto& val : lower_left) {
	    cos_lower_left.push_back(val.first);
	    tan_lower_left.push_back(val.second);
	  }

	  TGraph fl_lower_left(cos_lower_left.size(),cos_lower_left.data(),tan_lower_left.data());
	  grs.push_back(fl_lower_left);

	  //UPPER LEFT
	  std::vector<std::pair<double,double> > upper_left = {
	    std::make_pair(-0.95,8.5),
	    std::make_pair(-0.75,9.),
	    std::make_pair(-0.7,9.5),
	    std::make_pair(-0.675,10.),
	    std::make_pair(-0.65,11),
	    std::make_pair(-0.6,12),
	    std::make_pair(-0.55,12.5),
	    std::make_pair(-0.5,13.5),
	    std::make_pair(-0.45,14),
	    std::make_pair(-0.4,18),
	    std::make_pair(-0.375,25.),
	    std::make_pair(-0.36,40.),
	    std::make_pair(-0.35,50.),

	    //ADDITIONAL POINT TO DRAW!!!
	    std::make_pair(-0.95,50)
	  };

	  std::vector<double> cos_upper_left, tan_upper_left;
	  for(const auto& val : upper_left) {
	    cos_upper_left.push_back(val.first);
	    tan_upper_left.push_back(val.second);
	  }

	  TGraph fl_upper_left(cos_upper_left.size(),cos_upper_left.data(),tan_upper_left.data());
	  grs.push_back(fl_upper_left);

	  //LOWER RIGHT
	  std::vector<std::pair<double,double> > lower_right= {
	    std::make_pair(0.95,6),
	    std::make_pair(0.725,5.5),
	    std::make_pair(0.575,5),
	    std::make_pair(0.5,4.5),
	    std::make_pair(0.425,4),
	    std::make_pair(0.4,3.7),
	    std::make_pair(0.35,3.5),
	    std::make_pair(0.3,3.5),
	    std::make_pair(0.275,3.5),
	    std::make_pair(0.25,3.4),
	    std::make_pair(0.15,3.2),
	    std::make_pair(0.12,3.),
	    std::make_pair(0.1,2.7),
	    std::make_pair(0.07,2.5),
	    std::make_pair(0.05,2.),
	    std::make_pair(0.026,1.3),
	    std::make_pair(0.025,1.),
	    std::make_pair(0.024,0.9),
	    std::make_pair(0.023,0.8),
	    std::make_pair(0.022,0.7),
	    std::make_pair(0.021,0.6),
	    std::make_pair(0.02,0.5),

	    //ADDITIONAL POINT TO DRAW!!!
	    std::make_pair(0.95,0.5)
	  };

	  std::vector<double> cos_lower_right, tan_lower_right;
	  for(const auto& val : lower_right) {
	    cos_lower_right.push_back(val.first);
	    tan_lower_right.push_back(val.second);
	  }

	  TGraph fl_lower_right(cos_lower_right.size(),cos_lower_right.data(),tan_lower_right.data());
	  grs.push_back(fl_lower_right);

	  //UPPER RIGHT
	  std::vector<std::pair<double,double> > upper_right = {
	    std::make_pair(0.35,50),
	    std::make_pair(0.36,40.),
	    std::make_pair(0.375,25.),
	    std::make_pair(0.4,20),
	    std::make_pair(0.45,14),
	    std::make_pair(0.5,13.5),
	    std::make_pair(0.55,12.5),
	    std::make_pair(0.6,12),
	    std::make_pair(0.65,11),
	    std::make_pair(0.675,10.),
	    std::make_pair(0.7,9.7),
	    std::make_pair(0.75,9.),
	    std::make_pair(0.95,8.5),

	    //ADDITIONAL POINT TO DRAW!!!
	    std::make_pair(0.95,50)
	  };

	  std::vector<double> cos_upper_right, tan_upper_right;
	  for(const auto& val : upper_right) {
	    cos_upper_right.push_back(val.first);
	    tan_upper_right.push_back(val.second);
	  }

	  TGraph fl_upper_right(cos_upper_right.size(),cos_upper_right.data(),tan_upper_right.data());
	  grs.push_back(fl_upper_right);

	}
      }

      return grs;
    }

  } /* namespace mssmhbb */
} /* namespace analysis */
