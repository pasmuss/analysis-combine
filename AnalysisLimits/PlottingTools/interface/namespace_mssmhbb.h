#include <algorithm>
#include <string>
#include <map>
#include <vector>
//#include "Analysis/Tools/interface/CMS_lumi.h"
#include "Analysis/MssmHbb/interface/PublicationStatus.h"

/*
 * Namespace that contains all basic variables
 * used in the analysis flow
 */

namespace mssmhbb{
	// current CMSSW release base dir
	static const std::string cmsswBase = getenv("CMSSW_BASE");
	//publication status
	const PublicationStatus publication_status = WORKINPROGRESS;
	//pictures output folder
	const std::string pictures_output = cmsswBase +  "/src/Analysis/MssmHbb/pictures/" + PublicationStatusToString(publication_status) + "/";
//	const std::string pictures_output = cmsswBase +  "/src/Analysis/MssmHbb/pictures/";
	//blinded analysis or not
	const bool blinded = true;
	// current signal selection
	//static const std::string signal_selection = "NLO_correction_";
	// vector of masses:
	static const std::vector<int> masses = {300,350,400,450,500,600,700,800,900,1000,1200,1400,1600};
	// list of shape signal systematics
	//static const std::vector<std::string> signal_shape_unc = {"CMS_scale_j_13TeV","CMS_res_j_13TeV","CMS_eff_pTonl_13TeV","CMS_eff_b_13TeV"};//,"CMS_PDF_13TeV"};
	// signal MC points and pathes
	/*static const std::map<int,std::string> signal_templates {
		{300,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + signal_selection + "lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-300_TuneCUETP8M1_13TeV-pythia8.root"},
		{350,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + signal_selection + "lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-350_TuneCUETP8M1_13TeV-pythia8.root"},
		{400,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + signal_selection + "lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-400_TuneCUETP8M1_13TeV-pythia8.root"},
		{500,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + signal_selection + "lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-500_TuneCUETP8M1_13TeV-pythia8.root"},
		{600,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + signal_selection + "lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-600_TuneCUETP8M1_13TeV-pythia8.root"},
		{700,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + signal_selection + "lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-700_TuneCUETP8M1_13TeV-pythia8.root"},
		{900,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + signal_selection + "lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-900_TuneCUETP8M1_13TeV-pythia8.root"},
		{1100,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + signal_selection + "lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-1100_TuneCUETP8M1_13TeV-pythia8.root"},
		{1300,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + signal_selection + "lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-1300_TuneCUETP8M1_13TeV-pythia8.root"},
	};
	// signal MC folders
	static const std::map<int,std::string> signal_folders {
		{300,cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_signal_M-300"},
		{350,cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_signal_M-350"},
		{400,cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_signal_M-400"},
		{500,cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_signal_M-500"},
		{600,cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_signal_M-600"},
		{700,cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_signal_M-700"},
		{900,cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_signal_M-900"},
		{1100,cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_signal_M-1100"},
		{1300,cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_signal_M-1300"},
	};
	// signal MC PDF workspaces
	static const std::map<int,std::string> signal_workspaces {
		{300,signal_folders.at(300) + "/workspace/signal_workspace.root"},
		{350,signal_folders.at(350) + "/workspace/signal_workspace.root"},
		{400,signal_folders.at(400) + "/workspace/signal_workspace.root"},
		{500,signal_folders.at(500) + "/workspace/signal_workspace.root"},
		{600,signal_folders.at(600) + "/workspace/signal_workspace.root"},
		{700,signal_folders.at(700) + "/workspace/signal_workspace.root"},
		{900,signal_folders.at(900) + "/workspace/signal_workspace.root"},
		{1100,signal_folders.at(1100) + "/workspace/signal_workspace.root"},
		{1300,signal_folders.at(1300) + "/workspace/signal_workspace.root"},
		};
	// signal MC PDF fits
	static const std::map<int,std::string> signal_fits {
		{300,signal_folders.at(300) + "/workspace/FitContainer_workspace.root"},
		{350,signal_folders.at(350) + "/workspace/FitContainer_workspace.root"},
		{400,signal_folders.at(400) + "/workspace/FitContainer_workspace.root"},
		{500,signal_folders.at(500) + "/workspace/FitContainer_workspace.root"},
		{600,signal_folders.at(600) + "/workspace/FitContainer_workspace.root"},
		{700,signal_folders.at(700) + "/workspace/FitContainer_workspace.root"},
		{900,signal_folders.at(900) + "/workspace/FitContainer_workspace.root"},
		{1100,signal_folders.at(1100) + "/workspace/FitContainer_workspace.root"},
		{1300,signal_folders.at(1300) + "/workspace/FitContainer_workspace.root"},
		};
	// path to mssm_xs_tool root file
	static const std::string path_mssm_xsections = cmsswBase + "/src/Analysis/MssmHbb/macros/signal/mhmodp_mu200_13TeV.root";

	//path to bg fits
//	static const std::string path_bg_sr1 = cmsswBase + "/src/Analysis/BackgroundModel/test/extnovoeffprod_200to650_10GeV_G4/workspace/FitContainer_workspace.root";
//	static const std::string path_bg_sr2 = cmsswBase + "/src/Analysis/BackgroundModel/test/novosibirsk_350to1190_20GeV_G4/workspace/FitContainer_workspace.root";
//	static const std::string path_bg_sr3 = cmsswBase + "/src/Analysis/BackgroundModel/test/novosibirsk_500to1700_25GeV_G4/workspace/FitContainer_workspace.root";
	static const std::string path_bg_sr1 = (blinded) ?
			cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_bg_fit/sr1/FitContainer_workspace_SR1.root" :
			cmsswBase + "/src/Analysis/MssmHbb/output/unblinded/ReReco_bg_fit/sr1/FitContainer_workspace_SR1.root";
	static const std::string path_bg_sr2 = (blinded) ?
			cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_bg_fit/sr2/FitContainer_workspace_SR2.root" :
			cmsswBase + "/src/Analysis/MssmHbb/output/unblinded/ReReco_bg_fit/sr2/FitContainer_workspace_SR2.root";
	static const std::string path_bg_sr3 = (blinded) ?
			cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_bg_fit/sr3/FitContainer_workspace_SR3.root" :
			cmsswBase + "/src/Analysis/MssmHbb/output/unblinded/ReReco_bg_fit/sr3/FitContainer_workspace_SR3.root";
	static const std::map<int,std::string> path_bg_fits {
		{1,path_bg_sr1}, //sub-range 1
		{2,path_bg_sr2}, //sub-range 2
		{3,path_bg_sr3}, //sub-range 2
		};
	//Observed data
	static const std::string path_data_sr1 = (blinded) ?
			cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_bg_fit/sr1/QCD_Templates_SR1.root" :
			cmsswBase + "/src/Analysis/MssmHbb/output/unblinded/ReReco_bg_fit/sr1/Data_obs_SR1.root";
	static const std::string path_data_sr2 = (blinded) ?
			cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_bg_fit/sr2/QCD_Templates_SR2.root" :
			cmsswBase + "/src/Analysis/MssmHbb/output/unblinded/ReReco_bg_fit/sr2/Data_obs_SR2.root";
	static const std::string path_data_sr3 = (blinded) ?
			cmsswBase + "/src/Analysis/MssmHbb/output/ReReco_bg_fit/sr3/QCD_Templates_SR3.root" :
			cmsswBase + "/src/Analysis/MssmHbb/output/unblinded/ReReco_bg_fit/sr3/Data_obs_SR3.root";
	static const std::map<int,std::string> path_data_obs {
		{1,path_data_sr1}, //sub-range 1
		{2,path_data_sr2}, //sub-range 2
		{3,path_data_sr3}, //sub-range 2
	};
	*/
	//sub-ranges
	static const std::vector<int> sr1 = {300,350,400};
	static const std::vector<int> sr2 = {400,450,500,600,700};
	static const std::vector<int> sr3 = {700,800,900,1000};
	static const std::vector<int> sr4 = {1000,1200,1400,1600};
	//Signal functions and mass points
	static const std::map<std::string,std::vector<int> > signal_models {
		{"doublegausexp", {300,350,400,500}},
		{"bukin", {700,900,1100,1300}},
		{"quadgausexp", {600}},
	};

	const std::string getModelForTheMass(const int& mass);
		
};
