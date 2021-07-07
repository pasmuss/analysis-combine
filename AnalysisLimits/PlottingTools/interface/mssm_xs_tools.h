#ifndef ANALYSIS_MSSMHBB_INTERFACE_mssm_xs_tools_h
#define ANALYSIS_MSSMHBB_INTERFACE_mssm_xs_tools_h
 
#include <map>
#include <cmath>
#include <string>
#include <iostream>
 
#include "TH2F.h"
#include "TFile.h"
 
/* ________________________________________________________________________________________________
 * Class: mssm_xs_tools
 *
 * This is a class to provide a facilitated interface to mass relations, decay widths, branching
 * fractions and production cross sections for all Higgs bosons of various benchmark
 * models of the MSSM. This information is usually provided in form of 2d (TH2F) histograms in root
 * input files linked to the LHCXSWG-3 TWiki page (*). It is usually parametrized as a function of
 * the pseudoscalar neutral Higgs boson mass, mA, and the ratio between the vacuum expectation
 * values, tanb=v2/v1, of the two Higgs doublets in the MSSM, in the case of mass relations,
 * neutral Higgs boson cross sections, decay widths or branching fractions and as a function of the
 * charged Higgs boson mass mH+ and tanb in the case of charged Higgs boson cross sections, decay
 * widths and branching fractions.  
 *
 * The access functions provided in this class are supposed to facilitate the process of finding
 * the proper histogram (following LHCXSWG internal naming conventions) and the proper bin in the
 * 2d histogram corresponding to mphi=(mA,mH+) and tanb, of which the latter is a root technicali-
 * ty (cf. mssm_xs_tools::read for more details).
 * ------------------------------------------------------------------------------------------------
 * The quantity mphi below denotes either mA or mHp dependent on the scenario under consideration.
 * By default it yields mphi=mA. Only for the heavy Higgs scenario and the CPV scenario mphi=mH+.
 * ------------------------------------------------------------------------------------------------
 *
 * The names of the 2d histograms are build from building blocks separated by "_", to identify the
 * contained information. The following building blocks exist:
 *
 * [MASS]   : indicating mass ("m");
 * [WIDTH]  : indicating width ("width");
 * [BOSON]  : indicating the boson type, used for cross section and decay info ("A", "H", "h");
 * [PROD]   : indicating the production type, separated by gluon-gluon fusion and production in
 *            association with b-quarks ("ggH", "bbH");
 * [UNCERT] : indicating the kind of variation corresponding to a given uncertainty ("up", "down").
 *            "up" and "down" give the absolute difference to the central scale "Delta \Sigma^\pm".
 *            Some uncertainties are split into scale and pdf+alpha_s uncertainties, then
 *            the prefix scale or pdfas to "up" and "down" allows to obtain the individual
 *            uncertainties. Generally such individual uncertainties should be added in quadrature.
 *            For this purpose some routines named _unc and _uncboundaries can be found below.
 * [DECAY]  : indicating the decay type (e.g. "tautau", "mumu", "bb", ...).
 *
 * The exact conventions of these building blocks are not always defined in a unique way and might
 * change with time. The user should not worry about such changes. For this reasons the access
 * functions provided by this class make use of a set of internal translation rules from a more
 * intuitive naming scheme to the actual histogram names. For the sake of simplicity this naming
 * scheme partially makes use of the naming conventions as used for the individual building blocks
 * of the histogram names. For more information check the documentation for each individual trans-
 * lation rule in the class implementation. In the naming scheme used throughtout this class the
 * following rules apply:
 *
 * Production: separate building block for [PROD] from building block for [BOSON] by "->" (e.g.
 *             "gg->A"). This will provide you with the corresponding central value for the cross
 *             section. There is a way to obtain the cross sections after uncertainty shifts by
 *             appending a tag for the [UNCERT] type to the production string, separated by "::" (
 *             "gg->A::scaleup").
 * Decay     : separate building block for [BOSON] from building block for [DECAY] by "->" (e.g.
 *             "A->tautau"). This will provide you with the branching fraction for the correspon-
 *             ding decay.
 * Mass/Width: just give the building block for the [BOSON] type as function argument (e.g. "A",
 *             "H", "h", "Hp").
 *
 * In case that called histograms so not exist in the root input file a WARNING is issued and a
 * NULL pointer for TH2F is returned. Make sure you catch these WARNINGS in further use of this
 * class. To hide away also this simple logic from the user a full set of fully documented pre-de-
 * fined access functions for all available information is also provided. These function can also
 * be used as reference for further examples of the access logic described above (e.g. concerning
 * further conventions used for numerous building blocks of type [DECAY]).
 *
 * PHYSICS CONTENT:
 *
 * (*)
 * https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHXSWG3
 */
class mssm_xs_tools{
 
 public:
  /// constructor
  mssm_xs_tools(const char* filename="", bool kINTERPLOTATION=false, unsigned verbosity=0);
  /// destructor
  ~mssm_xs_tools(){ input_->Close(); };
 
  /// a save way to access a histogram from the stack; if the histogram with name does not exist
  /// already on the stack an attempt is made to load it from the input file; on success a bunch
  /// of relevant global information is cashed: number of bins on x- and y-axis and the minimum
  /// and maximum on x- and y-axis on first call; returns NULL if histogram does not exist on stack
  /// nor in input file
  TH2F* hist(std::string name);
  /// get mass of a given Higgs boson for given values of mA and tanb (in GeV)
  double mass(const char* boson, double mphi, double tanb){ return read(boson, mphi, tanb, &mssm_xs_tools::mass_rule); }
  /// get total decay width of a given Higgs boson for given values of mA and tanb (in GeV)
  double width(const char* boson, double mphi, double tanb){ return read(boson, mphi, tanb, &mssm_xs_tools::width_rule); }
  /// get the relative couplings for a given Higgs boson for given values of mA and tanb (in GeV)
  double coupling(const char* boson, double mphi, double tanb){ return read(boson, mphi, tanb, &mssm_xs_tools::coupling_rule); }
  /// get branching fraction for a given decay of a given Higgs boson for given values of mA and tanb
  double br(const char* decay, double mphi, double tanb){ return read(decay, mphi, tanb, &mssm_xs_tools::br_rule); }
  /// get production cross section for a given production model of a given Higgs boson for given values of mA and tanb (in pb)
  double xsec(const char* mode, double mphi, double tanb){ return read(mode, mphi, tanb, &mssm_xs_tools::xsec_rule); }
  /// Get mA for a given value of mass for a fixed value of tanb. The parameter mass can stand for hH or Hp, the meaning is determined from the parameter boson
  double mass2mA(const char* boson, double m, double tanb);
 
  /*
   * pre-defined access function for the masses for A/H/h/H+/H1/H2/H3
   */
  /// get mH for given value of mphi and tanb (in GeV)
  double mH(double mphi, double tanb){ return mass("H", mphi, tanb); }
  /// get mh for given value of mphi and tanb (in GeV)
  double mh(double mphi, double tanb){ return mass("h", mphi, tanb); }
  /// get mH+ for given value of mphi and tanb (in GeV)
  double mHp(double mphi, double tanb){ return mass("Hp", mphi, tanb); }
  /// get mA for given value of mu in low-mH scenario (need a more complex name
  /// of the member function here not to shadow it by the function arg in GeV)
  double mu2mA(double mu, double tanb){ return mass("A", mu, tanb); }
  //CPV
  /// get mH1 for given value of mphi and tanb (in GeV)
  double mH1(double mphi, double tanb){ return mass("H1", mphi, tanb); }
  /// get mH2 for given value of mphi and tanb (in GeV)
  double mH2(double mphi, double tanb){ return mass("H2", mphi, tanb); }
  /// get mH3 for given value of mphi and tanb (in GeV)
  double mH3(double mphi, double tanb){ return mass("H3", mphi, tanb); }
 
  /*
   * pre-defined access functions for total decay widths for A/H/h/H+/H1/H2/H3
   */
  // get total decay width for A  (in GeV)
  double width_A(double mphi, double tanb){ return width("A", mphi, tanb); }
  // get total decay width for H  (in GeV)
  double width_H(double mphi, double tanb){ return width("H", mphi, tanb); }
  // get total decay width for h  (in GeV)
  double width_h(double mphi, double tanb){ return width("h", mphi, tanb); }
  //non-CPV and CPV
  // get total decay width for H+  (in GeV)
  double width_Hp(double mphi, double tanb){ return width("Hp", mphi, tanb); }
  // get total decay width for t  (in GeV)
  double width_t(double mphi, double tanb){ return width("t", mphi, tanb); }
  //CPV
  // get total decay width for H1  (in GeV)
  double width_H1(double mphi, double tanb){ return width("H1", mphi, tanb); }
  // get total decay width for H2  (in GeV)
  double width_H2(double mphi, double tanb){ return width("H2", mphi, tanb); }
  // get total decay width for H3  (in GeV)
  double width_H3(double mphi, double tanb){ return width("H3", mphi, tanb); }
 
  /*
   * pre-defined access functions for relative Yukawa couplings for A/H/h
   */
  // get relative Yukawa of A to top
  double gtA(double mphi, double tanb){ return coupling("gt_A", mphi, tanb); }
  // get relative Yukawa of A to bottom
  double gbA(double mphi, double tanb){ return coupling("gb_A", mphi, tanb); }
  // get relative Yukawa of H to top
  double gtH(double mphi, double tanb){ return coupling("gt_H", mphi, tanb); }
  // get relative Yukawa of H to bottom
  double gbH(double mphi, double tanb){ return coupling("gb_H", mphi, tanb); }
  // get relative Yukawa of h to top
  double gth(double mphi, double tanb){ return coupling("gt_h", mphi, tanb); }
  // get relative Yukawa of h to bottom
  double gbh(double mphi, double tanb){ return coupling("gb_h", mphi, tanb); }
  //CPV
  //We do not provide relative couplings for Higgs sectors with CP violation
  //as the couplings are complex and have a vector-axial structure.

  /*
   * pre-defined access functions for A/H/h/H+/H1/H2/H3 decays into quarks
   */
  /// get BR(A->tt) for given value of mphi and tanb
  double br_Att(double mphi, double tanb){ return br("A->tt", mphi, tanb); }
  /// get BR(H->tt) for given value of mphi and tanb
  double br_Htt(double mphi, double tanb){ return br("H->tt", mphi, tanb); }
  /// get BR(h->tt) for given value of mphi and tanb
  double br_htt(double mphi, double tanb){ return br("h->tt", mphi, tanb); }
  /// get BR(A->bb) for given value of mphi and tanb
  double br_Abb(double mphi, double tanb){ return br("A->bb", mphi, tanb); }
  /// get BR(H->bb) for given value of mphi and tanb
  double br_Hbb(double mphi, double tanb){ return br("H->bb", mphi, tanb); }
  /// get BR(h->bb) for given value of mphi and tanb
  double br_hbb(double mphi, double tanb){ return br("h->bb", mphi, tanb); }
  /// get BR(A->cc) for given value of mphi and tanb
  double br_Acc(double mphi, double tanb){ return br("A->cc", mphi, tanb); }
  /// get BR(H->cc) for given value of mphi and tanb
  double br_Hcc(double mphi, double tanb){ return br("H->cc", mphi, tanb); }
  /// get BR(h->cc) for given value of mphi and tanb
  double br_hcc(double mphi, double tanb){ return br("h->cc", mphi, tanb); }
  /// get BR(A->ss) for given value of mphi and tanb
  double br_Ass(double mphi, double tanb){ return br("A->ss", mphi, tanb); }
  /// get BR(H->ss) for given value of mphi and tanb
  double br_Hss(double mphi, double tanb){ return br("H->ss", mphi, tanb); }
  /// get BR(h->ss) for given value of mphi and tanb
  double br_hss(double mphi, double tanb){ return br("h->ss", mphi, tanb); }
  /// get BR(A->uu) for given value of mphi and tanb
  double br_Auu(double mphi, double tanb){ return br("A->uu", mphi, tanb); }
  /// get BR(H->uu) for given value of mphi and tanb
  double br_Huu(double mphi, double tanb){ return br("H->uu", mphi, tanb); }
  /// get BR(h->uu) for given value of mphi and tanb
  double br_huu(double mphi, double tanb){ return br("h->uu", mphi, tanb); }
  /// get BR(A->dd) for given value of mphi and tanb
  double br_Add(double mphi, double tanb){ return br("A->dd", mphi, tanb); }
  /// get BR(H->dd) for given value of mphi and tanb
  double br_Hdd(double mphi, double tanb){ return br("H->dd", mphi, tanb); }
  /// get BR(h->dd) for given value of mphi and tanb
  double br_hdd(double mphi, double tanb){ return br("h->dd", mphi, tanb); }
  //non-CPV and CPV
  /// get BR(t->H+b) for given value of mphi=(mA,mHp) and tanb
  double br_tHpb(double mphi, double tanb){ return br("t->Hpb", mphi, tanb); }
  /// get BR(H+->tbb) for given value of mphi and tanb
  double br_Hptbb(double mphi, double tanb){ return br("Hp->tbb", mphi, tanb); }
  /// get BR(H+->tsb) for given value of mphi and tanb
  double br_Hptsb(double mphi, double tanb){ return br("Hp->tsb", mphi, tanb); }
  /// get BR(H+->tdb) for given value of mphi and tanb
  double br_Hptdb(double mphi, double tanb){ return br("Hp->tdb", mphi, tanb); }
  /// get BR(H+->cbb) for given value of mphi and tanb
  double br_Hpcbb(double mphi, double tanb){ return br("Hp->cbb", mphi, tanb); }
  /// get BR(H+->csb) for given value of mphi and tanb
  double br_Hpcsb(double mphi, double tanb){ return br("Hp->csb", mphi, tanb); }
  /// get BR(H+->cdb) for given value of mphi and tanb
  double br_Hpcdb(double mphi, double tanb){ return br("Hp->cdb", mphi, tanb); }
  /// get BR(H+->ubb) for given value of mphi and tanb
  double br_Hpubb(double mphi, double tanb){ return br("Hp->ubb", mphi, tanb); }
  /// get BR(H+->usb) for given value of mphi and tanb
  double br_Hpusb(double mphi, double tanb){ return br("Hp->usb", mphi, tanb); }
  /// get BR(H+->udb) for given value of mphi and tanb
  double br_Hpudb(double mphi, double tanb){ return br("Hp->udb", mphi, tanb); }
  //CPV
  /// get BR(H3->tt) for given value of mHp and tanb
  double br_H3tt(double mphi, double tanb){ return br("H3->tt", mphi, tanb); }
  /// get BR(H2->tt) for given value of mHp and tanb
  double br_H2tt(double mphi, double tanb){ return br("H2->tt", mphi, tanb); }
  /// get BR(H1->tt) for given value of mHp and tanb
  double br_H1tt(double mphi, double tanb){ return br("H1->tt", mphi, tanb); }
  /// get BR(H3->bb) for given value of mHp and tanb
  double br_H3bb(double mphi, double tanb){ return br("H3->bb", mphi, tanb); }
  /// get BR(H2->bb) for given value of mHp and tanb
  double br_H2bb(double mphi, double tanb){ return br("H2->bb", mphi, tanb); }
  /// get BR(H1->bb) for given value of mHp and tanb
  double br_H1bb(double mphi, double tanb){ return br("H1->bb", mphi, tanb); }
  /// get BR(H3->cc) for given value of mHp and tanb
  double br_H3cc(double mphi, double tanb){ return br("H3->cc", mphi, tanb); }
  /// get BR(H2->cc) for given value of mHp and tanb
  double br_H2cc(double mphi, double tanb){ return br("H2->cc", mphi, tanb); }
  /// get BR(H1->cc) for given value of mHp and tanb
  double br_H1cc(double mphi, double tanb){ return br("H1->cc", mphi, tanb); }
  /// get BR(H3->ss) for given value of mHp and tanb
  double br_H3ss(double mphi, double tanb){ return br("H3->ss", mphi, tanb); }
  /// get BR(H2->ss) for given value of mHp and tanb
  double br_H2ss(double mphi, double tanb){ return br("H2->ss", mphi, tanb); }
  /// get BR(H1->ss) for given value of mHp and tanb
  double br_H1ss(double mphi, double tanb){ return br("H1->ss", mphi, tanb); }
  /// get BR(H3->uu) for given value of mHp and tanb
  double br_H3uu(double mphi, double tanb){ return br("H3->uu", mphi, tanb); }
  /// get BR(H2->uu) for given value of mHp and tanb
  double br_H2uu(double mphi, double tanb){ return br("H2->uu", mphi, tanb); }
  /// get BR(H1->uu) for given value of mHp and tanb
  double br_H1uu(double mphi, double tanb){ return br("H1->uu", mphi, tanb); }
  /// get BR(H3->dd) for given value of mHp and tanb
  double br_H3dd(double mphi, double tanb){ return br("H3->dd", mphi, tanb); }
  /// get BR(H2->dd) for given value of mHp and tanb
  double br_H2dd(double mphi, double tanb){ return br("H2->dd", mphi, tanb); }
  /// get BR(H1->dd) for given value of mHp and tanb
  double br_H1dd(double mphi, double tanb){ return br("H1->dd", mphi, tanb); }

  /*
   * pre-defined access functions for A/H/h/H+ decays into leptons
   */
  /// get BR(A->tautau) for given value of mphi and tanb
  double br_Atautau(double mphi, double tanb){ return br("A->tautau", mphi, tanb); }
  /// get BR(H->tautau) for given value of mphi and tanb
  double br_Htautau(double mphi, double tanb){ return br("H->tautau", mphi, tanb); }
  /// get BR(h->tautau) for given value of mphi and tanb
  double br_htautau(double mphi, double tanb){ return br("h->tautau", mphi, tanb); }
  /// get BR(A->mumu) for given value of mphi and tanb
  double br_Amumu(double mphi, double tanb){ return br("A->mumu", mphi, tanb); }
  /// get BR(H->tautau) for given value of mphi and tanb
  double br_Hmumu(double mphi, double tanb){ return br("H->mumu", mphi, tanb); }
  /// get BR(h->mumu) for given value of mphi and tanb
  double br_hmumu(double mphi, double tanb){ return br("h->mumu", mphi, tanb); }
  /// get BR(A->ee) for given value of mphi and tanb
  double br_Aee(double mphi, double tanb){ return br("A->ee", mphi, tanb); }
  /// get BR(H->ee) for given value of mphi and tanb
  double br_Hee(double mphi, double tanb){ return br("H->ee", mphi, tanb); }
  /// get BR(h->ee) for given value of mphi and tanb
  double br_hee(double mphi, double tanb){ return br("h->ee", mphi, tanb); }
  //non-CPV and CPV
  /// get BR(H+->taunu) for given value of mphi and tanb
  double br_Hptaunu(double mphi, double tanb){ return br("Hp->taunu", mphi, tanb); }
  /// get BR(H+->munu) for given value of mphi and tanb
  double br_Hpmunu(double mphi, double tanb){ return br("Hp->munu", mphi, tanb); }
  /// get BR(H+->enu) for given value of mphi and tanb
  double br_Hpenu(double mphi, double tanb){ return br("Hp->enu", mphi, tanb); }
  //CPV
  /// get BR(H3->tautau) for given value of mphi and tanb
  double br_H3tautau(double mphi, double tanb){ return br("H3->tautau", mphi, tanb); }
  /// get BR(H2->tautau) for given value of mphi and tanb
  double br_H2tautau(double mphi, double tanb){ return br("H2->tautau", mphi, tanb); }
  /// get BR(H1->tautau) for given value of mphi and tanb
  double br_H1tautau(double mphi, double tanb){ return br("H1->tautau", mphi, tanb); }
  /// get BR(H3->mumu) for given value of mphi and tanb
  double br_H3mumu(double mphi, double tanb){ return br("H3->mumu", mphi, tanb); }
  /// get BR(H2->tautau) for given value of mphi and tanb
  double br_H2mumu(double mphi, double tanb){ return br("H2->mumu", mphi, tanb); }
  /// get BR(H1->mumu) for given value of mphi and tanb
  double br_H1mumu(double mphi, double tanb){ return br("H1->mumu", mphi, tanb); }
  /// get BR(H3->ee) for given value of mphi and tanb
  double br_H3ee(double mphi, double tanb){ return br("H3->ee", mphi, tanb); }
  /// get BR(H2->ee) for given value of mphi and tanb
  double br_H2ee(double mphi, double tanb){ return br("H2->ee", mphi, tanb); }
  /// get BR(H1->ee) for given value of mphi and tanb
  double br_H1ee(double mphi, double tanb){ return br("H1->ee", mphi, tanb); } 

  /*
   * pre-defined access functions for A/H/h decays into gauge bosons
   */
  /// get BR(A->WW) for given value of mphi and tanb
  double br_AWW(double mphi, double tanb){ return br("A->WW", mphi, tanb); }
  /// get BR(H->WW) for given value of mphi and tanb
  double br_HWW(double mphi, double tanb){ return br("H->WW", mphi, tanb); }
  /// get BR(h->WW) for given value of mphi and tanb
  double br_hWW(double mphi, double tanb){ return br("h->WW", mphi, tanb); }
  /// get BR(A->ZZ) for given value of mphi and tanb
  double br_AZZ(double mphi, double tanb){ return br("A->ZZ", mphi, tanb); }
  /// get BR(H->ZZ) for given value of mphi and tanb
  double br_HZZ(double mphi, double tanb){ return br("H->ZZ", mphi, tanb); }
  /// get BR(h->ZZ) for given value of mphi and tanb
  double br_hZZ(double mphi, double tanb){ return br("h->ZZ", mphi, tanb); }
  /// get BR(A->gg) for given value of mphi and tanb
  double br_Agg(double mphi, double tanb){ return br("A->gluglu", mphi, tanb); }
  /// get BR(H->gg) for given value of mphi and tanb
  double br_Hgg(double mphi, double tanb){ return br("H->gluglu", mphi, tanb); }
  /// get BR(h->gg) for given value of mphi and tanb
  double br_hgg(double mphi, double tanb){ return br("h->gluglu", mphi, tanb); }
  /// get BR(A->Zgamma) for given value of mphi and tanb
  double br_AZgamma(double mphi, double tanb){ return br("A->Zgam", mphi, tanb); }
  /// get BR(H->gammagamma) for given value of mphi and tanb
  double br_HZgamma(double mphi, double tanb){ return br("H->Zgam", mphi, tanb); }
  /// get BR(h->Zgamma) for given value of mphi and tanb
  double br_hZgamma(double mphi, double tanb){ return br("h->Zgamm", mphi, tanb); }
  /// get BR(A->gammagamma) for given value of mphi and tanb
  double br_Agammagamma(double mphi, double tanb){ return br("A->gamgam", mphi, tanb); }
  /// get BR(H->gammagamma) for given value of mphi and tanb
  double br_Hgammagamma(double mphi, double tanb){ return br("H->gamgam", mphi, tanb); }
  /// get BR(h->gammagamma) for given value of mphi and tanb
  double br_hgammagamma(double mphi, double tanb){ return br("h->gamgam", mphi, tanb); }
  //CPV
  /// get BR(H3->WW) for given value of mphi and tanb
  double br_H3WW(double mphi, double tanb){ return br("H3->WW", mphi, tanb); }
  /// get BR(H2->WW) for given value of mphi and tanb
  double br_H2WW(double mphi, double tanb){ return br("H2->WW", mphi, tanb); }
  /// get BR(H1->WW) for given value of mphi and tanb
  double br_H1WW(double mphi, double tanb){ return br("H1->WW", mphi, tanb); }
  /// get BR(H3->ZZ) for given value of mphi and tanb
  double br_H3ZZ(double mphi, double tanb){ return br("H3->ZZ", mphi, tanb); }
  /// get BR(H2->ZZ) for given value of mphi and tanb
  double br_H2ZZ(double mphi, double tanb){ return br("H2->ZZ", mphi, tanb); }
  /// get BR(H1->ZZ) for given value of mphi and tanb
  double br_H1ZZ(double mphi, double tanb){ return br("H1->ZZ", mphi, tanb); }
  /// get BR(A->gg) for given value of mphi and tanb
  double br_H3gg(double mphi, double tanb){ return br("H3->gluglu", mphi, tanb); }
  /// get BR(H->gg) for given value of mphi and tanb
  double br_H2gg(double mphi, double tanb){ return br("H2->gluglu", mphi, tanb); }
  /// get BR(h->gg) for given value of mphi and tanb
  double br_H1gg(double mphi, double tanb){ return br("H1->gluglu", mphi, tanb); }
  /// get BR(H3->Zgamma) for given value of mphi and tanb
  double br_H3Zgamma(double mphi, double tanb){ return br("H3->Zgam", mphi, tanb); }
  /// get BR(H2->gammagamma) for given value of mphi and tanb
  double br_H2Zgamma(double mphi, double tanb){ return br("H2->Zgam", mphi, tanb); }
  /// get BR(H1->Zgamma) for given value of mphi and tanb
  double br_H1Zgamma(double mphi, double tanb){ return br("H1->Zgamm", mphi, tanb); }
  /// get BR(A->gammagamma) for given value of mphi and tanb
  double br_H3gammagamma(double mphi, double tanb){ return br("H3->gamgam", mphi, tanb); }
  /// get BR(H->gammagamma) for given value of mphi and tanb
  double br_H2gammagamma(double mphi, double tanb){ return br("H2->gamgam", mphi, tanb); }
  /// get BR(h->gammagamma) for given value of mphi and tanb
  double br_H1gammagamma(double mphi, double tanb){ return br("H1->gamgam", mphi, tanb); }
 
  /*
   * pre-defined access functions for Higgs to Higgs/SUSY decays
   */
  /// get BR(H->hh) for given value of mphi and tanb
  double br_Hhh(double mphi, double tanb){ return br("H->hh", mphi, tanb); }
  /// get BR(A->Zh) for given value of mphi and tanb
  double br_AZh(double mphi, double tanb){ return br("A->Zh", mphi, tanb); }
  /// get BR(H+->Wh) for given value of mphi and tanb
  double br_HpWh(double mphi, double tanb){ return br("Hp->hW", mphi, tanb); }
  /// get BR(H+->WA) for given value of mphi and tanb
  double br_HpWA(double mphi, double tanb){ return br("Hp->AW", mphi, tanb); }
  /// get BR(H+->HHA) for given value of mphi and tanb
  double br_HpHHW(double mphi, double tanb){ return br("Hp->HHW", mphi, tanb); }
  /// get BR(A->SUSY) for given value of mphi and tanb
  double br_ASUSY(double mphi, double tanb){ return br("A->SUSY", mphi, tanb); }
  /// get BR(H->SUSY) for given value of mphi and tanb
  double br_HSUSY(double mphi, double tanb){ return br("H->SUSY", mphi, tanb); }
  /// get BR(h->SUSY) for given value of mphi and tanb
  double br_hSUSY(double mphi, double tanb){ return br("h->SUSY", mphi, tanb); }
  //non-CPV and CPV
  /// get BR(H+->SUSY) for given value of mphi and tanb
  double br_HpSUSY(double mphi, double tanb){ return br("Hp->SUSY", mphi, tanb); }
  //CPV
  /// get BR(H3->H1H1) for given value of mphi and tanb
  double br_H3H1H1(double mphi, double tanb){ return br("H3->H1H1", mphi, tanb); }
  /// get BR(H2->H1H1) for given value of mphi and tanb
  double br_H2H1H1(double mphi, double tanb){ return br("H2->H1H1", mphi, tanb); }
  /// get BR(H3->ZH1) for given value of mphi and tanb
  double br_H3ZH1(double mphi, double tanb){ return br("H3->ZH1", mphi, tanb); }
  /// get BR(H2->ZH1) for given value of mphi and tanb
  double br_H2ZH1(double mphi, double tanb){ return br("H2->ZH1", mphi, tanb); }
  /// get BR(H+->WH1) for given value of mphi and tanb
  double br_HpWH1(double mphi, double tanb){ return br("Hp->H1W", mphi, tanb); }
  /// get BR(H+->WH3) for given value of mphi and tanb
  double br_HpWH3(double mphi, double tanb){ return br("Hp->H3W", mphi, tanb); }
  /// get BR(H+->WH2) for given value of mphi and tanb
  double br_HpWH2(double mphi, double tanb){ return br("Hp->H2W", mphi, tanb); }
  /// get BR(H3->SUSY) for given value of mphi and tanb
  double br_H3SUSY(double mphi, double tanb){ return br("H3->SUSY", mphi, tanb); }
  /// get BR(H2->SUSY) for given value of mphi and tanb
  double br_H2SUSY(double mphi, double tanb){ return br("H2->SUSY", mphi, tanb); }
  /// get BR(H1->SUSY) for given value of mphi and tanb
  double br_H1SUSY(double mphi, double tanb){ return br("H1->SUSY", mphi, tanb); }
 
  /*
   * pre-defined access functions for A/H/h/H+/H1/H2/H3 production
   */
  /// get cross section for production of A in gluon-gluon fusion (in pb)
  double ggH_A(double mphi, double tanb){ return xsec("gg->A", mphi, tanb); }
  /// get cross section for production of H in gluon-gluon fusion (in pb)
  double ggH_H(double mphi, double tanb){ return xsec("gg->H", mphi, tanb); }
  /// get cross section for production of h in gluon-gluon fusion (in pb)
  double ggH_h(double mphi, double tanb){ return xsec("gg->h", mphi, tanb); }
  /// get cross section for production of A in association with b quarks (in pb)
  double bbH_A(double mphi, double tanb){ return xsec("bb->A", mphi, tanb); }
  /// get cross section for production of H in association with b quarks (in pb)
  double bbH_H(double mphi, double tanb){ return xsec("bb->H", mphi, tanb); }
  /// get cross section for production of h in association with b quarks (in pb)
  double bbH_h(double mphi, double tanb){ return xsec("bb->h", mphi, tanb); }
  //non-CPV and CPV
  /// get cross section for production of H+ directly from proton-proton collision (in pb)
  double pp_Hp(double mphi, double tanb){ return xsec("pp->Hp", mphi, tanb); }
  //CPV
  /// get cross section for production of H3 in gluon-gluon fusion (in pb)
  double ggH_H3(double mphi, double tanb){ return xsec("gg->H3", mphi, tanb); }
  /// get cross section for production of H2 in gluon-gluon fusion (in pb)
  double ggH_H2(double mphi, double tanb){ return xsec("gg->H2", mphi, tanb); }
  /// get cross section for production of H1 in gluon-gluon fusion (in pb)
  double ggH_H1(double mphi, double tanb){ return xsec("gg->H1", mphi, tanb); }
  /// get cross section for production of H3 in association with b quarks (in pb)
  double bbH_H3(double mphi, double tanb){ return xsec("bb->H3", mphi, tanb); }
  /// get cross section for production of H2 in association with b quarks (in pb)
  double bbH_H2(double mphi, double tanb){ return xsec("bb->H2", mphi, tanb); }
  /// get cross section for production of H1 in association with b quarks (in pb)
  double bbH_H1(double mphi, double tanb){ return xsec("bb->H1", mphi, tanb); }
 
  /*
   * pre-defined access functions for interference factors in CPV scenarios
   */
  double interference_bb_tautau_H3(double mphi, double tanb){ return xsec("int_bb_tautau_H3", mphi, tanb); }
  double interference_bb_tautau_H2(double mphi, double tanb){ return xsec("int_bb_tautau_H2", mphi, tanb); }
  double interference_bb_tautau_H1(double mphi, double tanb){ return xsec("int_bb_tautau_H1", mphi, tanb); }
  double interference_gg_tautau_H3(double mphi, double tanb){ return xsec("int_gg_tautau_H3", mphi, tanb); }
  double interference_gg_tautau_H2(double mphi, double tanb){ return xsec("int_gg_tautau_H2", mphi, tanb); }
  double interference_gg_tautau_H1(double mphi, double tanb){ return xsec("int_gg_tautau_H1", mphi, tanb); }

  /*
   * pre-defined access functions for A/H/h/H+/H1/H2/H3 production uncertainties in several flavours
   */
  /// get uncertainties for gluon-gluon fusion production of A from mu scale variations (in pb)  
  double ggH_A_scale(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "gg->A::scaleup" : "gg->A::scaledown", mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of H from mu scale variations (in pb)  
  double ggH_H_scale(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "gg->H::scaleup" : "gg->H::scaledown", mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of h from mu scale variations (in pb)  
  double ggH_h_scale(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "gg->h::scaleup" : "gg->h::scaledown", mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of A from variations of pdfs and alphas (in pb)  
  double ggH_A_pdfas(double mphi, double tanb, const bool kUP){ return xsec((kUP ? "gg->A::pdfasup" : "gg->A::pdfasdown"), mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of H from variations of pdfs and alphas (in pb)  
  double ggH_H_pdfas(double mphi, double tanb, const bool kUP){ return xsec((kUP ? "gg->H::pdfasup" : "gg->H::pdfasdown"), mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of h from variations of pdfs and alphas (in pb)  
  double ggH_h_pdfas(double mphi, double tanb, const bool kUP){ return xsec((kUP ? "gg->h::pdfasup" : "gg->h::pdfasdown"), mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of A (in pb)  
  double ggH_A_unc(double mphi, double tanb, const bool kUP);
  /// get uncertainties for gluon-gluon fusion production of H (in pb)  
  double ggH_H_unc(double mphi, double tanb, const bool kUP);
  /// get uncertainties for gluon-gluon fusion production of h (in pb)  
  double ggH_h_unc(double mphi, double tanb, const bool kUP);
  /// get uncertainties for A production in association with b quarks (in pb)  
  double bbH_A_unc(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "bb->A::up" : "bb->A::down", mphi, tanb); }
  /// get uncertainties for H production in association with b quarks (in pb)  
  double bbH_H_unc(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "bb->H::up" : "bb->H::down", mphi, tanb); }
  /// get uncertainties for h production in association with b quarks (in pb)  
  double bbH_h_unc(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "bb->h::up" : "bb->h::down", mphi, tanb); }
  //non-CPV and CPV
  /// get uncertainties for H+ production directly from proton-proton collision (in pb)
  double pp_Hp_unc(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "pp->Hp::up" : "pp->Hp::down", mphi, tanb); }
  //CPV
  /// get uncertainties for gluon-gluon fusion production of H3 from mu scale variations (in pb)  
  double ggH_H3_scale(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "gg->H3::scaleup" : "gg->H3::scaledown", mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of H2 from mu scale variations (in pb)  
  double ggH_H2_scale(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "gg->H2::scaleup" : "gg->H2::scaledown", mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of H1 from mu scale variations (in pb)  
  double ggH_H1_scale(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "gg->H1::scaleup" : "gg->H1::scaledown", mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of H3 from variations of pdfs and alphas (in pb)  
  double ggH_H3_pdfas(double mphi, double tanb, const bool kUP){ return xsec((kUP ? "gg->H3::pdfasup" : "gg->H3::pdfasdown"), mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of H2 from variations of pdfs and alphas (in pb)  
  double ggH_H2_pdfas(double mphi, double tanb, const bool kUP){ return xsec((kUP ? "gg->H2::pdfasup" : "gg->H2::pdfasdown"), mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of H1 from variations of pdfs and alphas (in pb)  
  double ggH_H1_pdfas(double mphi, double tanb, const bool kUP){ return xsec((kUP ? "gg->H1::pdfasup" : "gg->H1::pdfasdown"), mphi, tanb); }
  /// get uncertainties for gluon-gluon fusion production of H3 (in pb)  
  double ggH_H3_unc(double mphi, double tanb, const bool kUP);
  /// get uncertainties for gluon-gluon fusion production of H2 (in pb)  
  double ggH_H2_unc(double mphi, double tanb, const bool kUP);
  /// get uncertainties for gluon-gluon fusion production of H1 (in pb)  
  double ggH_H1_unc(double mphi, double tanb, const bool kUP);
  /// get uncertainties for H3 production in association with b quarks (in pb)  
  double bbH_H3_unc(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "bb->H3::up" : "bb->H3::down", mphi, tanb); }
  /// get uncertainties for H2 production in association with b quarks (in pb)  
  double bbH_H2_unc(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "bb->H2::up" : "bb->H2::down", mphi, tanb); }
  /// get uncertainties for H1 production in association with b quarks (in pb)  
  double bbH_H1_unc(double mphi, double tanb, const bool kUP){ return xsec(kUP ? "bb->H1::up" : "bb->H1::down", mphi, tanb); }  

  /// routines for upper and lower values of uncertainties:
  double ggH_A_uncboundaries(double mphi, double tanb, const bool kUP);
  double ggH_H_uncboundaries(double mphi, double tanb, const bool kUP);
  double ggH_h_uncboundaries(double mphi, double tanb, const bool kUP);
  double bbH_A_uncboundaries(double mphi, double tanb, const bool kUP);
  double bbH_H_uncboundaries(double mphi, double tanb, const bool kUP);
  double bbH_h_uncboundaries(double mphi, double tanb, const bool kUP);
  //non-CPV and CPV
  double pp_Hp_uncboundaries(double mphi, double tanb, const bool kUP);
  //CPV
  double ggH_H3_uncboundaries(double mphi, double tanb, const bool kUP);
  double ggH_H2_uncboundaries(double mphi, double tanb, const bool kUP);
  double ggH_H1_uncboundaries(double mphi, double tanb, const bool kUP);
  double bbH_H3_uncboundaries(double mphi, double tanb, const bool kUP);
  double bbH_H2_uncboundaries(double mphi, double tanb, const bool kUP);
  double bbH_H1_uncboundaries(double mphi, double tanb, const bool kUP);
 
 private:
  /// open input file to access histograms (only needed if input file was not specified in the constructor)
  void setup(const char* filename);
  /// rule to determine histogram names related to branching fractions in the input file
  std::string br_rule(const char* br);
  /// rule to determine histogram names related to full decay widths in the input file
  std::string width_rule(const char* b);
  /// rule to determine histogram names related to relative Yukawas in the input file
  std::string coupling_rule(const char* b);
  /// rule to determine histogram names related to cross sections in the input file
  std::string xsec_rule(const char* xs);
  /// rule to determine histogram names related to masses in the input file
  std::string mass_rule(const char* b);
  /// a common function to return histograms from the input file. Apart from mA and tanb it takes an
  /// identification string for the object in the input file [key] and a conversion rule to transform
  /// this string into the histogram name [rule]. The conversion rule is a pointer-to-member-function.
  /// The conversion rules are provided as private member functions of the class. The function "read"
  /// will determine the histogram name from the rule, determine the bin containing mA and tanb and
  /// return the bin content. Alternatiely it returns a bilinear interpolation corresponding to the
  /// values mA and tanb, if kINTERPOL_ has been set to true during the instantiation of the class.
  double read(const char* key, double mphi, double tanb, std::string (mssm_xs_tools::*rule)(const char* key)){
     std::string name = (this->*rule)(key);
     TH2F* h = hist(name);
     if(h){
       if(kINTERPOL_){
                 // restrict interpolation to boundaries of histogram, at the boundaries fall back to plain bin
                 // content
                 if(h->GetXaxis()->FindBin(mphi  )<nbinsX_-1 && h->GetYaxis()->FindBin(tanb)<nbinsY_-1){
                   return h->Interpolate(mphi,tanb);
                 }
                 else{
                   return h->GetBinContent(h->FindBin(mphi,tanb));    
                 }    
       }
       else{
               return h->GetBinContent(h->FindBin(mphi,tanb));    
       }
     }
     else{ return -1.; }
  }
  /// verbosity level
  unsigned verbosity_;
  /// apply bilinear interpolation (built-in to TH2F) or get bin content, plain as it is
  bool kINTERPOL_;
  /// root input file (opened in constructor)
  TFile* input_;
  /// histogram container (filled in constructor)
  std::map<std::string, TH2F*> hists_;
  /// flag that indicates whether meta information on the input histograms (given below
  /// has already been cashed in a function call of hist or not
  bool cashed_;
  /// maximal number of bins on x-axis (mA)   of TH2F (filled in function hist; used in
  /// function read in interpolation mode)
  int nbinsX_;
  /// minimal and maximal element on x-axis (mA) of TH2F (filled in function hist; used
  /// in function mH2mA)
  double minX_, maxX_;
  /// maximal number of bins on y-axis (tanb) of TH2F (filled in function hist; used in
  /// function read in interpolation mode)
  int nbinsY_;
  /// minimal and maximal element on y-axis (tanb) of TH2F (filled in function hist;
  /// used in function mH2mA)
  double minY_, maxY_;
};
 
inline double
mssm_xs_tools::ggH_A_unc(double mphi, double tanb, const bool kUP){
  double scaleunc=this->ggH_A_scale(mphi,tanb,kUP);
  double pdfasunc=this->ggH_A_pdfas(mphi,tanb,kUP);
  double sign = -1;
  if (kUP) sign = +1;
  return sign*sqrt(pow(scaleunc,2)+pow(pdfasunc,2));
}

inline double
mssm_xs_tools::ggH_H_unc(double mphi, double tanb, const bool kUP){
  double scaleunc=this->ggH_H_scale(mphi,tanb,kUP);
  double pdfasunc=this->ggH_H_pdfas(mphi,tanb,kUP);
  double sign = -1;
  if (kUP) sign = +1;
  return sign*sqrt(pow(scaleunc,2)+pow(pdfasunc,2));
}

inline double
mssm_xs_tools::ggH_h_unc(double mphi, double tanb, const bool kUP){
  double scaleunc=this->ggH_h_scale(mphi,tanb,kUP);
  double pdfasunc=this->ggH_h_pdfas(mphi,tanb,kUP);
  double sign = -1;
  if (kUP) sign = +1;
  return sign*sqrt(pow(scaleunc,2)+pow(pdfasunc,2));
}

inline double
mssm_xs_tools::ggH_h_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->ggH_h(mphi,tanb);
  double absunc =this->ggH_h_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::ggH_H_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->ggH_H(mphi,tanb);
  double absunc =this->ggH_H_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::ggH_A_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->ggH_A(mphi,tanb);
  double absunc =this->ggH_A_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::bbH_h_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->bbH_h(mphi,tanb);
  double absunc =this->bbH_h_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::bbH_H_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->bbH_H(mphi,tanb);
  double absunc =this->bbH_H_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::bbH_A_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->bbH_A(mphi,tanb);
  double absunc =this->bbH_A_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::pp_Hp_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->pp_Hp(mphi,tanb);
  double absunc =this->pp_Hp_unc(mphi,tanb,kUP);
  return central+absunc;
}
//CPV
inline double
mssm_xs_tools::ggH_H3_unc(double mphi, double tanb, const bool kUP){
  double scaleunc=this->ggH_H3_scale(mphi,tanb,kUP);
  double pdfasunc=this->ggH_H3_pdfas(mphi,tanb,kUP);
  double sign = -1;
  if (kUP) sign = +1;
  return sign*sqrt(pow(scaleunc,2)+pow(pdfasunc,2));
}

inline double
mssm_xs_tools::ggH_H2_unc(double mphi, double tanb, const bool kUP){
  double scaleunc=this->ggH_H2_scale(mphi,tanb,kUP);
  double pdfasunc=this->ggH_H2_pdfas(mphi,tanb,kUP);
  double sign = -1;
  if (kUP) sign = +1;
  return sign*sqrt(pow(scaleunc,2)+pow(pdfasunc,2));
}

inline double
mssm_xs_tools::ggH_H1_unc(double mphi, double tanb, const bool kUP){
  double scaleunc=this->ggH_H1_scale(mphi,tanb,kUP);
  double pdfasunc=this->ggH_H1_pdfas(mphi,tanb,kUP);
  double sign = -1;
  if (kUP) sign = +1;
  return sign*sqrt(pow(scaleunc,2)+pow(pdfasunc,2));
}

inline double
mssm_xs_tools::ggH_H1_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->ggH_H1(mphi,tanb);
  double absunc =this->ggH_H1_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::ggH_H2_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->ggH_H2(mphi,tanb);
  double absunc =this->ggH_H2_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::ggH_H3_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->ggH_H3(mphi,tanb);
  double absunc =this->ggH_H3_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::bbH_H1_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->bbH_H1(mphi,tanb);
  double absunc =this->bbH_H1_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::bbH_H2_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->bbH_H2(mphi,tanb);
  double absunc =this->bbH_H2_unc(mphi,tanb,kUP);
  return central+absunc;
}

inline double
mssm_xs_tools::bbH_H3_uncboundaries(double mphi, double tanb, const bool kUP){
  double central=this->bbH_H3(mphi,tanb);
  double absunc =this->bbH_H3_unc(mphi,tanb,kUP);
  return central+absunc;
}


/* ________________________________________________________________________________________________
 * Class: mssm_xs_tools
 *
 * External C wrapper to make core functionality available in python, too.
 */
/* 
extern "C" {
  /// constructor
  mssm_xs_tools* mssm_xs_tools_new(const char* filename="", bool kINTERPLOTATION=false, unsigned verbosity=0){
    return new mssm_xs_tools(filename, kINTERPLOTATION, verbosity);
  };
  /// destructor
  void mssm_xs_tools_delete(mssm_xs_tools* obj){
    delete obj;
  };
  /// get mass of a given Higgs boson for given values of mphi and tanb (in GeV)
  double mssm_xs_tools_mass(mssm_xs_tools* obj, const char* boson, double mphi, double tanb){
    //std::cout << obj->read(boson, mA, tanb, &mssm_xs_tools::mass_rule) << std::endl;
    return obj->mass(boson, mphi, tanb);
  }
  /// get total decay width of a given Higgs boson for given values of mA and tanb (in GeV)
  double mssm_xs_tools_width(mssm_xs_tools* obj, const char* boson, double mphi, double tanb){
    return obj->width(boson, mphi, tanb);
  }
  /// get relative Yukawa of a given Higgs boson for given values of mA and tanb (in GeV)
  double mssm_xs_tools_coupling(mssm_xs_tools* obj, const char* boson, double mphi, double tanb){
    return obj->coupling(boson, mphi, tanb);
  }
  /// get branching fraction for a given decay of a given Higgs boson for given values of mA and tanb
  double mssm_xs_tools_br(mssm_xs_tools* obj, const char* decay, double mphi, double tanb){
    return obj->br(decay, mphi, tanb);
  }
  /// get production cross section for a given production model of a given Higgs boson for given values of mA and tanb (in pb)
  double mssm_xs_tools_xsec(mssm_xs_tools* obj, const char* mode, double mphi, double tanb){
    return obj->xsec(mode, mphi, tanb);
  }
}
*/
#endif // ANALYSIS_MSSMHBB_INTERFACE_MSSM_XS_TOOLS_H
