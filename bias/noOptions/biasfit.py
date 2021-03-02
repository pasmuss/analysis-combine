import os
from subprocess import call
import glob
import ROOT

phrase = "newSignal"
#otherphrase = "eml200-2000"
mulist = ["0", "1", "2", "3"]
#mulist = ["0"]
#subrangelist = ["SR1","SR2","SR3","SR4"]
subrangelist = ["SR1"]
grepphrase = '"Best fit r: "'
failures = []
counter = 0
largebias = 0
divzero = 0
toyindex = str(5)
fitindex = str(4)

for subrange in subrangelist:
    if subrange=="SR1":
        masslist = ["300","350","400"]
        #masslist = ["300"]
    elif subrange=="SR2":
        masslist = ["400","450","500","600","700"]
    elif subrange=="SR3":
        masslist = ["700","800","900","1000"]
    elif subrange=="SR4":
        masslist = ["1000","1200","1400","1600"]
    totaloutfile = 'AllBiasesPlusSigmas_' + phrase + '_' + subrange + '_toy' + toyindex + '-fit' + fitindex + '.txt'
    open(totaloutfile,'w').close()
    for mu in mulist:
        for mass in masslist:
            folder = phrase + '_BIAS_mu' + mu + '_M-' + mass + '_pdftoy_' + toyindex + '_pdffit_' + fitindex + '_' + subrange
            print(folder)
            txtfile = 'FDraw_mu' + mu + '_mass' + mass + '_' +  subrange + '.txt'
            rootfile = ROOT.TFile.Open('FDraw_mu' + mu + '_mass' + mass + '_' +  subrange + '.root','RECREATE')
            grepstring = ('grep ' + grepphrase + ' ' + folder + '/HTC*/exe.sh.out > ' + txtfile)
            #print(grepstring)
            call(grepstring, shell=True)
            with open(txtfile,'r') as rawdata:
                rvalues = []
                sigmas = []
                content = rawdata.readlines()
                content = [x.strip() for x in content]
                histogram = ROOT.TH1F("histogram","Bias",20,-5,5)
                histo_mu = ROOT.TH1F("histo_mu","sig strength fit results",400,-10,10)
                histo_sigma = ROOT.TH1F("histo_sigma","sig strength fit uncertainties",400,-2,2)
                for oneresult in content:
                    print(oneresult)
                    partresult = oneresult.split(':')[-1].split(' ')
                    individualr = partresult[1]
                    histo_mu.Fill(float(individualr))
                    individualerrors = partresult[3]
                    try:
                        downerrorind = individualerrors.split('/')[0].split('-')[1]
                    except IndexError:
                        downerrorind = individualerrors.split('/')[0]
                    print(individualerrors.split('/')[0].split('-'))
                    try:
                        uperrorind = individualerrors.split('/')[1].split('+')[1]
                    except IndexError:
                        uperrorind = individualerrors.split('/')[1]
                    if downerrorind.find('e') != -1:
                        print(individualerrors.split('/')[0].split('-')[1])
                        print(individualerrors.split('/')[0].split('-')[2])
                        downerrorind = individualerrors.split('/')[0].split('-')[1] + '-' + individualerrors.split('/')[0].split('-')[2]
                        failures.append(downerrorind)
                    if uperrorind.find('-')!=-1:
                        counter += 1
                        continue
                    print(individualr,downerrorind,uperrorind)
                    if float(downerrorind)+float(uperrorind)==0:
                        divzero += 1
                        continue
                    bias = (float(individualr) - float(mu)) / (0.5*(float(downerrorind)+float(uperrorind)))
                    rvalues.append(float(individualr))
                    print("length sigmas before: " + str(len(sigmas)) + ", sum: " + str(sum(sigmas)))
                    sigmahere = 0.5*(float(downerrorind)+float(uperrorind))
                    if (len(sigmas) == 0):
                        sigmaaverage = 1
                    else:
                        sigmaaverage = sum(sigmas)/len(sigmas)
                    print("sigma here: " + str(sigmahere) + ", average so far: " + str(sigmaaverage))
                    if (sigmahere <= 10*sigmaaverage) and (downerrorind.find('e') == -1):
                        sigmas.append(0.5*(float(downerrorind)+float(uperrorind)))
                    print("adding sigma of " + str(0.5*(float(downerrorind)+float(uperrorind))) + ", length after: " + str(len(sigmas)) + ", sum: " + str(sum(sigmas)))
                    histo_sigma.Fill(0.5*(float(downerrorind)+float(uperrorind)))
                    print(individualr,0.5*(float(downerrorind)+float(uperrorind)))
                    print(bias)
                    if (float(downerrorind) > 1) or (float(uperrorind) > 1):
                        largebias += 1
                    histogram.Fill(bias)
                    histogram.Write()
                    histo_mu.Write()
                    histo_sigma.Write()
                fitfunc = ROOT.TF1("fitfunc","gaus",-5,5)
                fitres = histogram.Fit("fitfunc",'S')
                finalbias = fitfunc.GetParameter(1)
                finalsigma = fitfunc.GetParameter(2)
                raverage = sum(rvalues)/len(rvalues)
                sigav = sum(sigmas)/len(sigmas)
                with open(totaloutfile,'a') as finalfile:
                    finalfile.write("Bias " + mass + " GeV (" + subrange + ", mu=" + mu +"): " + str(float(finalbias)*100) + "%, sigma: " + str(finalsigma) + ", <mu> = " + str(raverage) + ", <sigma> = " + str(sigav) + ', toys: ' + str(len(rvalues)) +'\n')
#print(largebias)
#print(failures)
#print(counter)
