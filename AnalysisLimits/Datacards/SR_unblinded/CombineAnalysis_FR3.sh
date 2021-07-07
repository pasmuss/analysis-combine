peak=240
for subrange in SR3
do
    limit_file=Hbb_limits_${subrange}_freezealpha0
    [[ -f "$limit_file" ]] && rm "$limit_file"
    if [ "$subrange" = "SR3" ]
    then masses=(700 800 900 1000)
#    then masses=(800 900)
	tail=-0.711103
	width=57.801
    fi
    echo "$subrange, masses ${masses[@]}"
    for mass in ${masses[@]}
    do
	if [ "$subrange" = "SR3" ]
	then
	    if [ ${mass} = 700 ]
            then bias=0.038; offbtag=1.115; onlbtag=1.057; kintrig=1.011; purew=1.002
            elif [ ${mass} = 800 ]
            then bias=0.026; offbtag=1.115; onlbtag=1.062; kintrig=1.011; purew=1.002
            elif [ ${mass} = 900 ]
            then bias=0.021; offbtag=1.113; onlbtag=1.067; kintrig=1.011; purew=1.001
            elif [ ${mass} = 1000 ]
            then bias=0.017; offbtag=1.112; onlbtag=1.072; kintrig=1.011; purew=1.001
            fi
	fi
#	mkdir "${subrange}_${mass}_freezealpha0"
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
#	mv datacard_Analysis_toy_${subrange}_${mass}GeV.* "${subrange}_${mass}_freezealpha0"
	cd ${subrange}_${mass}_freezealpha0
#	echo "Starting combine ${subrange} ${mass}"
#	combine -M FitDiagnostics -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak,offsetTF=170,alphaTF=0 --setParameterRanges r=-3,3:tail=-1,-0.68:width=40,65 --freezeParameters offsetTF,tail,alphaTF -n Hbb_${subrange} -m $mass --plots --saveShapes --saveWithUncertainties --rebinFactor 4 --cminDefaultMinimizerStrategy 0 -v5 > FDplain.log
#	combine -M AsymptoticLimits -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --setParameterRanges r=-3,3:tail=-1,-0.68:width=40,65 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak,offsetTF=170,alphaTF=0 --freezeParameters offsetTF,tail,alphaTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} -v 5 > Plain.log
	combine -M Significance -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --pvalue --setParameterRanges r=-3,3:tail=-1,-0.68:width=40,65 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak,offsetTF=170,alphaTF=0 --freezeParameters offsetTF,tail,alphaTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} > Significance.log
	combine -M GoodnessOfFit -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --algo=saturated --setParameterRanges r=-3,3:tail=-1,-0.68:width=40,65 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak,offsetTF=170,alphaTF=0 --freezeParameters offsetTF,tail,alphaTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} > GoF.log
	combine -M GoodnessOfFit -d datacard_Analysis_toy_${subrange}_${mass}GeV.root -t 1000 -s 123456 --algo=saturated --setParameterRanges r=-3,3:tail=-1,-0.68:width=40,65 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak,offsetTF=170,alphaTF=0 --freezeParameters offsetTF,tail,alphaTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} | tee GoFToys.log
	combineTool.py -M CollectGoodnessOfFit --input *GoodnessOfFit*root -o Hbb_${subrange}_${mass}.json
	plotGof.py -o TestStat_${subrange}_${mass} --mass ${mass}.0 Hbb_${subrange}_${mass}.json
	cd ..
#	root_name=`readlink -f "${subrange}_${mass}_freezealpha0/higgsCombineHbb_${subrange}.AsymptoticLimits.mH${mass}.root"`
#	echo "$root_name" >> "$limit_file"
	echo "$mass GeV $subrange finished"
    done
#    echo "Merge Combination output:"
#    hadd -f combineMerge_${subrange}_freezealpha0.root ${subrange}*_freezealpha0/higgsCombineHbb_${subrange}.AsymptoticLimits.mH*.root
#    echo "Merge Done: combineMerge_${subrange}_freezealpha0.root was created"
#    cd /nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src; eval `scramv1 runtime -sh`; cd -
#    PlotLimit -i $limit_file
#    eval `scramv1 runtime -sh`
done

#SR 3: tail = -0.711103; width = 57.801, peak = 240
#SR 4: tail = -0.764043; width = 51.0459, peak = 240
