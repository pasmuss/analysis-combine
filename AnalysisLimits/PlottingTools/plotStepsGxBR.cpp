/*
 * plotStepsGxBR.cpp
 *
 *  Created on: 6 Sep 2017
 *  Author: shevchen
 *
 *  Macro to calculate and plot bias step GxBR limits
 *  from the output of the combine tool.
 */

#include <iostream>
#include <string>

#include "Analysis/MssmHbb/interface/HbbLimits.h"
#include "Analysis/MssmHbb/src/namespace_mssmhbb.cpp"
#include "Analysis/MssmHbb/interface/HbbStyleClass.h"

using namespace std;
using namespace analysis::mssmhbb;
using namespace mssmhbb;

int main(){

	HbbStyle style;
	style.setTDRstyle(PRELIMINARY);

	//Prefix to the output
	string output_prefix = "adjoint_13TeV_limits_Feb16-21";
	//paths with results
	string path2017_sr1 	 = cmsswBase + "/src/Combine/AnalysisLimits/ActualLimits/Hbb_limits_SR1_alluncert_exptoy_freealpha";
	string path2017_sr2 	 = cmsswBase + "/src/Combine/AnalysisLimits/ActualLimits/Hbb_limits_SR2_alluncert_exptoy_theo";
	string path2017_sr3 	 = cmsswBase + "/src/Combine/AnalysisLimits/ActualLimits/Hbb_limits_SR3_alluncert_exptoy_theo_fralpha"; 
	string path2017_sr4      = cmsswBase + "/src/Combine/AnalysisLimits/ActualLimits/Hbb_limits_SR4_alluncert_exptoy_untouchedOffset";
	//ouptut folder
	string output = cmsswBase + "/src/Combine/AnalysisLimits/ActualLimits/";
	if(!mssmhbb::blinded) output += "unblinded/";
	string output_name = output + output_prefix;
	CheckOutputDir(output);

	HbbLimits limits_sr1(mssmhbb::blinded);
	limits_sr1.ReadCombineLimits(path2017_sr1);
	limits_sr1.Write(output_name + "_sr1");
	
	HbbLimits limits_sr2(mssmhbb::blinded);
	limits_sr2.ReadCombineLimits(path2017_sr2);
	limits_sr2.Write(output_name + "_sr2");
	
	HbbLimits limits_sr3(mssmhbb::blinded);
	limits_sr3.ReadCombineLimits(path2017_sr3);
	limits_sr3.Write(output_name + "_sr3");

	HbbLimits limits_sr4(mssmhbb::blinded);
        limits_sr4.ReadCombineLimits(path2017_sr4);
        limits_sr4.Write(output_name + "_sr4");

	//if comparison needed - insert vector of CompareLimits and TLegend in front of the LimitPlotter method
//	limits_sr1.PlotSubRangeSteps(limits_sr1.getLimits(),limits_sr2.getLimits(),limits_sr3.getLimits(),output_name,"35.7 (2016) fb^{-1}","M_{A/H} [GeV]","95%CL limit on #sigma x BR [pb]",true);
	limits_sr1.PlotSubRangeSteps(limits_sr1.getLimits(),limits_sr2.getLimits(),limits_sr3.getLimits(),limits_sr4.getLimits(),output_name,"36.0 (2017) fb^{-1}",string(HbbStyle::axisTitleMAH().Data()),"#sigma(b#bar{b}A/H) #bf{#it{#Beta}}(A/H#rightarrow b#bar{b}) [pb]",true,true);
}
