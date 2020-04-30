for run in {800..999}
do
    for mag in 0p0 0p05 0p1 0p25 0p5 1p0 5p0
    do
	cp WorkspaceForPDFTemplate_Template.C WorkspaceForPDFTemplate_tmp.C
	sed -i "s/WorkspaceForPDFTemplate()/WorkspaceForPDFTemplate_tmp()/g" WorkspaceForPDFTemplate_tmp.C
	sed -i "s/CRHISTDUMMY/CR_Hist_${run}/g" WorkspaceForPDFTemplate_tmp.C
	sed -i "s/SRHISTDUMMY/SR_Hist_${run}_${mag}/g" WorkspaceForPDFTemplate_tmp.C
	sed -i "s/OUTDUMMY/${run}_${mag}/g" WorkspaceForPDFTemplate_tmp.C
	root -l -q WorkspaceForPDFTemplate_tmp.C
	cp Pulls/datacards/datacard_template.txt Pulls/datacards/datacard_PDFtemplate_allSigSR_${run}_${mag}.txt
	sed -i "s/SAMPLE/${run}/g" Pulls/datacards/datacard_PDFtemplate_allSigSR_${run}_${mag}.txt
	sed -i "s/STRENGTH/${mag}/g" Pulls/datacards/datacard_PDFtemplate_allSigSR_${run}_${mag}.txt
	text2workspace.py Pulls/datacards/datacard_PDFtemplate_allSigSR_${run}_${mag}.txt --X-allow-no-signal
	combine -M FitDiagnostics --rMin -20 --rMax 20 -n .Pulls_tmp Pulls/datacards/datacard_PDFtemplate_allSigSR_${run}_${mag}.root --cminDefaultMinimizerStrategy 0 | grep r: >> Pulls/r/r_${mag}.txt
	#--plots --saveWithUncertainties --saveNormalization --saveShapes
	echo "${run}_${mag} done"
	sleep 1
    done
done
sleep 1
rm WorkspaceForPDFTemplate_tmp.C
rm higgsCombine.Pulls*
rm fitDiagnostics*
rm *png
rm combine_logger.out
