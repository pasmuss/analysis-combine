#!/usr/bin/python

'''Macro to run combine tool with Asymptotic method.
    
'''

import os,sys, shutil
from subprocess import Popen

__author__ = "Rostyslav Shevchenko"
__maintainer__ = "Rostyslav Shevchenko"
__email__ = "rostyslav.shevchenko@desy.de"

def writeHbbLimits(mass):
    '''Function to write Hbb.limits file.
    
    '''
    f = open("Hbb.limits","a")
    #file name:
    result = os.path.abspath("./") + '/higgsCombineHbb.Asymptotic.mH' + mass + '.root\n'
    #write to the file
    f.write(result)
    
def checkInput(datacard_folder):
    '''Method to check whether folder exists or not.
    
    '''
    if not os.path.exists(datacard_folder):
        raise AttributeError('Wrong input folder path: ' + datacard_folder)

def runCombineTool(combine,data_card,m,log_file = 'log_asymptotic'):
    '''Function to run combine for paticular mass.
    
    '''
    print combine + ' -m ' + m + ' ' + data_card + ' > ' + log_file + '_' + m + '.o' 
    proc = Popen(combine + ' -m ' + m + ' ' + data_card + ' > ' + log_file + '_' + m + '.o',shell=True)

if __name__ == '__main__':

    # working directory with datacards and stored output:
    # mhmodp_200 2hdm tau_phobic light_stop light_stau hMSSM independent
    model = 'independent'
    datacard_folder = '/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201709/20/unblinded/' + model + '/bias/'
    #datacard_folder = '/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201708/23/unblinded/tests/asymptotic_600_newMinimizers/'
    checkInput(datacard_folder)
    os.chdir(datacard_folder)
    #if combination of 7+8+13 is performed:
    combination = False
    #Clean Hbb.Limits file if exists
    if os.path.exists("Hbb.limits"): os.remove("Hbb.limits")
    #list of mass points
    mass = ['300','350','400','500','600','700','900','1100','1300']
    blinded = False
    #mass = ['600']
    #combine preferences to be added
    combine_add = '--setPhysicsModelParameterRanges CMS_PDF_13TeV=-10,10'#'--freezeNuisances CMS_scale_j_13TeV,CMS_PDF_13TeV'
    #values of rMin and rMax to be used
    rMin = '-20'
    rMax = '20'
    for m in mass:
        data_card = 'hbb_mbb' + m + '_mssm-13TeV.txt'
        if combination:
            data_card = 'Comb_7TeV_8TeV_HIG16018_13TeV_Packed_M' + m + '_card.txt'
            rMin = '-30'
            rMax = '40'

        #adjust combine command
        if m == '300' or m == '350':
            if m == '350' and combination:
                rMin = '-20'
                rMax = '30'
            if m == '300' and 'Bernstein' in datacard_folder:
                rMin = '-30'
                rMax = '30'
            if m == '300':
            	if not blinded:
                    rMin = '-20'
                    rMax = '20'
                    if model == 'mhmodp_200' or model == 'light_stau':
                        rMin = '-15'
                        rMax = '15'
                else: 
                	rMin = '-10'
                	rMax = '10'
            if m == '350':
            	if not blinded:
                	rMin = '-10'
                	rMax = '10'
                else:
                	rMax = '20'
	                rMin = '-10'
            combine = 'combine -M Asymptotic -n Hbb --rMin=' + rMin + ' --rMax=' + rMax + ' --minimizerStrategy 0 --cminFallbackAlgo "Minuit2,Minimize,0:0.1" --cminOldRobustMinimize 0 -v5'# -t -1 --expectSignal=0' 
        else:
            combine = 'combine -M Asymptotic -n Hbb --rMin=' + rMin + ' --rMax=' + rMax + ' -v 5'# -t -1 --expectSignal=0'
        combine += ' ' + combine_add
        #run combination tool
        runCombineTool(combine,data_card,m)
        #write file with results to Hbb.Limits
        writeHbbLimits(m)
