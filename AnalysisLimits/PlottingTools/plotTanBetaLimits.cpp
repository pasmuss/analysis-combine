/*
 * plotTanBetaLimits.cpp
 *
 *  Created on: 9 Mar 2017
 *  Author: shevchen
 *
 *  Macro to calculate and plot MSSM limits
 *  from the output of the combine tool.
 *  Comparison of 13 TeV and 7+8 TeV combination
 *  can be performed as well.
 */

#include <iostream>
#include <string>
#include <vector>

//ROOT includes
#include <TFile.h>

#include "Analysis/MssmHbb/interface/LHCXSGLimits.h"
#include "Analysis/MssmHbb/interface/LHCXSGScenarious.h"
#include "Analysis/MssmHbb/interface/Limit.h"
#include "Analysis/MssmHbb/interface/namespace_mssmhbb.h"

HbbStyle style;

using namespace std;
using namespace analysis::mssmhbb;
using namespace mssmhbb;

std::string getBanchmarkPath(AvailableScenarios scenario);
void setFolderNamesAccordingToBlinding(string& input, string & output);

int main(){
  PublicationStatus status = mssmhbb::publication_status;
  //status = SUPPLEMENTARY;
  style.setTDRstyle(status);
  //Prefix to the output
  string output_prefix = "13TeV_2017_Mar14-21";
  //paths with results
  string path2016_solo 	 = "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Combine/AnalysisLimits/ActualLimits";
  string path2016_combined = "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Combine/AnalysisLimits/ActualLimits/Hbb_limits_full_alluncert_theo";
  //ouptut folder
  string output = "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Combine/AnalysisLimits/ActualLimits/Plots";
  setFolderNamesAccordingToBlinding(path2016_solo,output);
  string boson = "both";
  std::vector<AvailableScenarios> scenarious = {MH125,NEGMU1,NEGMU2,NEGMU3};//{MH125,NEGMU1,NEGMU2,NEGMU3};//
  for(const auto& scenario : scenarious){
    std::cout<<"Scenario: "<<AvailableScenariosToString(scenario)<<std::endl;
    string output_mod 			= output + AvailableScenariosToString(scenario) + "/";
    string path2016_solo_mod 	= path2016_solo + AvailableScenariosToString(scenario) + "/Hbb_limits_full_alluncert_theo";
    CheckOutputDir(output_mod);
    //benchmark scenario path
    LHCXSGLimits limits(mssmhbb::blinded,boson,getBanchmarkPath(scenario));
    //limits.compareWithPrevious("HIG-16-018");
    limits.setScenario(scenario);
    limits.setXMin(300);
    limits.setXMax(1600);
    if (scenario == HMSSM) limits.setXMax(1400);
    if (scenario == MH125) limits.setXMax(1200);
    if(scenario != HMSSM)limits.drawExcludedRegion(3);	// Region NOT compatible with H(125)
    else limits.drawExcludedRegion(0);

    string benchmark_name = AvailableScenariosToString(scenario);
    string output_name = output_mod + boson + "_" + benchmark_name + "_" + output_prefix;
          
    // combined results UNCOMMENT if combination is performed
    //vector<Limit> GxBR_combined = limits.ReadCombineLimits(path2016_combined);
    //string benchmark_ref = "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/data/mhmodp_mu200_13TeV.root";
    //vector<Limit> mssm_limits_combined = limits.GetMSSMLimits(GxBR_combined,benchmark,"",false,benchmark_ref,30);
    //limits.Write(mssm_limits_combined, "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Combine/AnalysisLimits/ActualLimits/Plots/combined_limits.txt");

    //solo 2016 13 TeV
    limits.ReadCombineLimits(path2016_combined);
    limits.Write(output_name);

    limits.LimitPlotter(output_name,"36.0(2017)",string(HbbStyle::axisTitleMA().Data()),"tan#beta",false);
  }
	

}

std::string getBanchmarkPath(AvailableScenarios scenario){
  string output = "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/MssmHbb/macros/signal/";
  switch (scenario){
  case MHMODP_200:
    output += "mhmodp_mu200_13TeV.root";
    break;
  case LIGHT_STOP:
    output += "lightstopmod_13TeV.root";
    break;
  case LIGHT_STAU:
    output += "lightstau1_13TeV.root";
    break;
  case HMSSM:
    output += "hMSSM_13TeV.root";
    break;
  case TAU_PHOBIC:
    output += "tauphobic_13TeV.root";
    break;
  case MH125:
    output += "mh125_13.root";
    break;
  case NEGMU1:
    output += "mh125_muneg_1_13.root";
    break;
  case NEGMU2:
    output += "mh125_muneg_2_13.root";
    break;
  case NEGMU3:
    output += "mh125_muneg_3_13.root";
    break;
  default: 
    break;
  }
  return output;
}

void setFolderNamesAccordingToBlinding(string& input, string & output){
  if(!mssmhbb::blinded) {
    input += "unblinded/";
    output += "unblinded/";
  }
  else {
    input += "blinded/";
    output += "blinded/";
  }
}
