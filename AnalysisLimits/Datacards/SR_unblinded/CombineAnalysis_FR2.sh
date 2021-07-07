for subrange in SR2
do
    limit_file=Hbb_limits_${subrange}_newoffTF
    [[ -f "$limit_file" ]] && rm "$limit_file"
    if [ "$subrange" = "SR2" ]
#    then masses=(400 450 500 600 700)
    then masses=(400)
	#root -l -q AnalysisWorkspaceSR2.C
    fi
    echo "$subrange, masses ${masses[@]}"
    for mass in ${masses[@]}
    do
	if [ ${mass} = 400 ]
        then bias=0.465; offbtag=1.112; onlbtag=1.042; kintrig=1.013; purew=1.002
	elif [ ${mass} = 450 ]
        then bias=0.339; offbtag=1.109; onlbtag=1.044; kintrig=1.012; purew=1.001
        elif [ ${mass} = 500 ]
        then bias=0.276; offbtag=1.104; onlbtag=1.046; kintrig=1.012; purew=1.001
	elif [ ${mass} = 600 ]
        then bias=0.183; offbtag=1.101; onlbtag=1.051; kintrig=1.011; purew=1.002
	elif [ ${mass} = 700 ]
        then bias=0.102; offbtag=1.097; onlbtag=1.056; kintrig=1.011; purew=1.002
        fi
#	mkdir "${subrange}_${mass}_newoffTF"
#	cp datacard_Analysis_toySR_template.txt datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/MASS/${mass}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/SUBRANGE/${subrange}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/BIASRESULT/${bias}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/OFFBT/${offbtag}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
 #       sed -i "s/ONLBT/${onlbtag}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#        sed -i "s/KINTO/${kintrig}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#        sed -i "s/PUVAL/${purew}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	echo "Datacard $mass GeV, $subrange created"
#	text2workspace.py datacard_Analysis_toy_${subrange}_${mass}GeV.txt --X-allow-no-signal --channel-masks
#	mv datacard_Analysis_toy_${subrange}_${mass}GeV.* ${subrange}_${mass}_newoffTF
	cd ${subrange}_${mass}_newoffTF
	echo "Starting combine ${subrange} ${mass}"
	combine -M FitDiagnostics -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367,offsetTF=170,alphaTF=0 --freezeParameters offsetTF,par0,slopelinTF --setParameterRanges r=-5,5:tail=-10,-0.5:par0=-0.001,0.001 -n Hbb_${subrange} -m $mass --plots --saveShapes --saveWithUncertainties --rebinFactor 15 --cminDefaultMinimizerStrategy 0 -v 5 > FDplain.log #setParameters r=0 if not expectSignal
#	combine -M AsymptoticLimits -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367,offsetTF=170,alphaTF=0 --freezeParameters offsetTF,par0,slopelinTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} --setParameterRanges r=-5,5:tail=-10,-0.5:par0=-0.001,0.001 -v 5 &> Plain.log
#	combine -M Significance -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --pvalue --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367,offsetTF=170,alphaTF=0 --freezeParameters offsetTF,par0,slopelinTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} --setParameterRanges r=-5,5:tail=-10,-0.5:par0=-0.001,0.001 > Significance.log
#	combine -M GoodnessOfFit -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --algo=saturated --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367,offsetTF=170,alphaTF=0 --freezeParameters offsetTF,par0,slopelinTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} --setParameterRanges r=-5,5:tail=-10,-0.5:par0=-0.001,0.001 > GoF.log
#	combine -M GoodnessOfFit -d datacard_Analysis_toy_${subrange}_${mass}GeV.root -t 1000 -s 123456 --algo=saturated --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367,offsetTF=170,alphaTF=0 --freezeParameters offsetTF,par0,slopelinTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} --setParameterRanges r=-5,5:tail=-10,-0.5:par0=-0.001,0.001 | tee GoFToys.log
#	combineTool.py -M CollectGoodnessOfFit --input *GoodnessOfFit*root -o Hbb_${subrange}_${mass}.json
#	plotGof.py -o TestStat_${subrange}_${mass} --mass ${mass}.0 Hbb_${subrange}_${mass}.json
	cd ..
#	root_name=`readlink -f "${subrange}_${mass}_newoffTF/higgsCombineHbb_${subrange}.AsymptoticLimits.mH${mass}.root"`
#	echo "$root_name" >> "$limit_file"
	echo "$mass GeV $subrange finished"
    done
#    echo "Merge Combination output:"
#    hadd -f combineMerge_${subrange}_newoffTF.root ${subrange}_${mass}_newoffTF/higgsCombineHbb_${subrange}.AsymptoticLimits.mH*.root
#    echo "Merge Done: combineMerge_${subrange}_newoffTF.root was created"
#    cd /nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src; eval `scramv1 runtime -sh`; cd -
#    PlotLimit -i $limit_file
#    eval `scramv1 runtime -sh`
done

#SR 2: par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs r --floatOtherPOIs 1 -n ${subrange}_fastscan_r_50 -m ${mass} --algo grid --points 50 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs turnon_novoeff --floatOtherPOIs 1 -n ${subrange}_fastscan_turnon_novoeff_50 -m ${mass} --algo grid --points 50 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs slope_novoeff --floatOtherPOIs 1 -n ${subrange}_fastscan_slope_novoeff_50 -m ${mass} --algo grid --points 50 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs par0 --floatOtherPOIs 1 -n ${subrange}_fastscan_par0_50 -m ${mass} --algo grid --points 50 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs peak --floatOtherPOIs 1 -n ${subrange}_fastscan_peak_50 -m ${mass} --algo grid --points 50 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs tail --floatOtherPOIs 1 -n ${subrange}_fastscan_tail_50 -m ${mass} --algo grid --points 50 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs width --floatOtherPOIs 1 -n ${subrange}_fastscan_width_50 -m ${mass} --algo grid --points 50 --fastScan

#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs r --floatOtherPOIs 1 -n ${subrange}_fastscan_r_100 -m ${mass} --algo grid --points 100 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs turnon_novoeff --floatOtherPOIs 1 -n ${subrange}_fastscan_turnon_novoeff_100 -m ${mass} --algo grid --points 100 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs slope_novoeff --floatOtherPOIs 1 -n ${subrange}_fastscan_slope_novoeff_100 -m ${mass} --algo grid --points 100 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs par0 --floatOtherPOIs 1 -n ${subrange}_fastscan_par0_100 -m ${mass} --algo grid --points 100 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs peak --floatOtherPOIs 1 -n ${subrange}_fastscan_peak_100 -m ${mass} --algo grid --points 100 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs tail --floatOtherPOIs 1 -n ${subrange}_fastscan_tail_100 -m ${mass} --algo grid --points 100 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs width --floatOtherPOIs 1 -n ${subrange}_fastscan_width_100 -m ${mass} --algo grid --points 100 --fastScan

#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs r --floatOtherPOIs 1 -n ${subrange}_fastscan_r_500 -m ${mass} --algo grid --points 500 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs turnon_novoeff --floatOtherPOIs 1 -n ${subrange}_fastscan_turnon_novoeff_500 -m ${mass} --algo grid --points 500 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs slope_novoeff --floatOtherPOIs 1 -n ${subrange}_fastscan_slope_novoeff_500 -m ${mass} --algo grid --points 500 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs par0 --floatOtherPOIs 1 -n ${subrange}_fastscan_par0_500 -m ${mass} --algo grid --points 500 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs peak --floatOtherPOIs 1 -n ${subrange}_fastscan_peak_500 -m ${mass} --algo grid --points 500 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs tail --floatOtherPOIs 1 -n ${subrange}_fastscan_tail_500 -m ${mass} --algo grid --points 500 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,par0=0.000232321,peak=270.117,slope_novoeff=0.0122963,tail=-0.878842,turnon_novoeff=281.955,width=51.8367 --cminDefaultMinimizerStrategy 0 --redefineSignalPOIs width --floatOtherPOIs 1 -n ${subrange}_fastscan_width_500 -m ${mass} --algo grid --points 500 --fastScan
