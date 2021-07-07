peak=240
for subrange in SR4
do
    limit_file=Hbb_limits_${subrange}_oldTFset
    [[ -f "$limit_file" ]] && rm "$limit_file"
    if [ "$subrange" = "SR4" ]
    then masses=(1000 1200 1400 1600)
#    then masses=(1000)
	tail=-0.764043
	width=51.0459
    fi
    echo "$subrange, masses ${masses[@]}"
    for mass in ${masses[@]}
    do
	if [ "$subrange" = "SR4" ]
	then
	    if [ ${mass} = 1000 ]
            then bias=0.017; offbtag=1.129; onlbtag=1.073; kintrig=1.011; purew=1.001
            elif [ ${mass} = 1200 ]
            then bias=0.011; offbtag=1.124; onlbtag=1.083; kintrig=1.011; purew=1.000
            elif [ ${mass} = 1400 ]
            then bias=0.011; offbtag=1.119; onlbtag=1.094; kintrig=1.011; purew=1.001
            elif [ ${mass} = 1600 ]
            then bias=0.010; offbtag=1.116; onlbtag=1.105; kintrig=1.011; purew=1.000
            fi
	fi
#	mkdir "${subrange}_${mass}_oldTFset"
#	cp datacard_Analysis_toySR_template.txt datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/MASS/${mass}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/SUBRANGE/${subrange}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/BIASRESULT/${bias}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/OFFBT/${offbtag}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/ONLBT/${onlbtag}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/KINTO/${kintrig}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	sed -i "s/PUVAL/${purew}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
#	echo "Datacard $mass GeV, $subrange created"
#	text2workspace.py datacard_Analysis_toy_${subrange}_${mass}GeV.txt --X-allow-no-signal --channel-masks
#	mv datacard_Analysis_toy_${subrange}_${mass}GeV.* "${subrange}_${mass}_oldTFset"
	cd ${subrange}_${mass}_oldTFset
	echo "Starting combine ${subrange} ${mass}"
#	combine -M FitDiagnostics -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak --setParameterRanges r=-1,1:tail=-1,-0.68:width=40,65:steepnessTF=-0.003,0.03:slopelinTF=0.00001,0.001 --freezeParameters offsetTF -n Hbb_${subrange} -m $mass --plots --saveShapes --rebinFactor 2 --saveWithUncertainties --cminDefaultMinimizerStrategy 0 -v5 > FDplain.log #steepnessTF=-0.003,0.03, nothing for linear slope
#	combine -M AsymptoticLimits -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --setParameterRanges r=-1,1:tail=-1,-0.68:width=40,65:steepnessTF=-0.003,0.03:slopelinTF=0.00001,0.001 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak --freezeParameters offsetTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} -v 5 > Plain.log #steepnessTF=-0.003,0.03, nothing for linear slope
	combine -M Significance -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --pvalue --setParameterRanges r=-1,1:tail=-1,-0.68:width=40,65:steepnessTF=-0.003,0.03:slopelinTF=0.00001,0.001 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak --freezeParameters offsetTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} > Significance.log
	combine -M GoodnessOfFit -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --algo=saturated --setParameterRanges r=-1,1:tail=-1,-0.68:width=40,65:steepnessTF=-0.003,0.03:slopelinTF=0.00001,0.001 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak --freezeParameters offsetTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} > GoF.log
	combine -M GoodnessOfFit -d datacard_Analysis_toy_${subrange}_${mass}GeV.root -t 1000 -s 123456 --algo=saturated --setParameterRanges r=-1,1:tail=-1,-0.68:width=40,65:steepnessTF=-0.003,0.03:slopelinTF=0.00001,0.001 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak --freezeParameters offsetTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} | tee GoFToys.log
	combineTool.py -M CollectGoodnessOfFit --input *GoodnessOfFit*root -o Hbb_${subrange}_${mass}.json
        plotGof.py -o TestStat_${subrange}_${mass} --mass ${mass}.0 Hbb_${subrange}_${mass}.json
	cd ..
#	root_name=`readlink -f "${subrange}_${mass}_oldTFset/higgsCombineHbb_${subrange}.AsymptoticLimits.mH${mass}.root"`
#	echo "$root_name" >> "$limit_file"
	echo "$mass GeV $subrange finished"
    done
#    echo "Merge Combination output:"
#    hadd -f combineMerge_${subrange}_oldTFset.root ${subrange}*_oldTFset/higgsCombineHbb_${subrange}.AsymptoticLimits.mH*.root
#    echo "Merge Done: combineMerge_${subrange}_oldTFset.root was created"
#    cd /nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src; eval `scramv1 runtime -sh`; cd -
#    PlotLimit -i $limit_file
#    eval `scramv1 runtime -sh`
done

#SR 3: tail = -0.711103; width = 57.801, peak = 240
#SR 4: tail = -0.764043; width = 51.0459, peak = 240
