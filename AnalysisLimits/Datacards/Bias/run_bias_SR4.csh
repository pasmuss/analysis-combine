#!/bin/csh -f

set ntoys=10
set ndirs=500
set index_fit=0
set index_toy=2
echo "++++++ Produce toys with pdfindex = " ${index_toy} "++++++++"
foreach mp(1000 1200 1400 1600)
    foreach mu(0 1 2 3)
	mkdir newSignal_BIAS_mu${mu}_M-${mp}_pdftoy_${index_toy}_pdffit_${index_fit}_SR4
	cd newSignal_BIAS_mu${mu}_M-${mp}_pdftoy_${index_toy}_pdffit_${index_fit}_SR4
	echo Processing Mass point $mp GeV with injected signal strength $mu and fit index ${index_fit}
	../htc_all_SR4.csh $ntoys $ndirs $mu $mp $index_toy $index_fit
	cd ..
	echo "============================================="
    end
end
