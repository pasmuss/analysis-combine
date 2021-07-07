/*
 * HbbLimits.h
 *
 *  Created on: Dec 13, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_INTERFACE_HBBLIMITS_H_
#define MSSMHBB_INTERFACE_HBBLIMITS_H_

#include <string>
#include <iostream>
#include <map>

#include "TH3.h"
#include <TH2.h>
#include <TH1.h>
#include "TLegend.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TGraphAsymmErrors.h"

// Root includes
#include <TTree.h>
#include <TMath.h>
#include <TH1.h>
#include <TH3F.h>

// cpp includes
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

//my includes
#include "Analysis/MssmHbb/interface/Luminosity.h"
#include "Analysis/MssmHbb/interface/HbbStyleClass.h"
//#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/interface/Limit.h"
#include "Analysis/MssmHbb/interface/utilLib.h"

namespace analysis {
namespace mssmhbb {

class HbbLimits {
public:
	HbbLimits();
	HbbLimits(const bool& blindData, const double& xMin = 250, const double& xMax = 1650, const double& yMin = 0.05, const double& yMax = 30, const bool& test = false);
	virtual ~HbbLimits();

	struct LimitsToCompare{
		std::string legend = "";
		std::vector<Limit> limits;
	};

	struct DecomposedLimits{
		std::vector<double> X;
		std::vector<double> obs;
		std::vector<double> median;
		std::vector<double> minus1;
		std::vector<double> minus2;
		std::vector<double> plus1;
		std::vector<double> plus2;
		std::vector<double> zero;
	};
	//Method to decompose vector of limits to the vectors of individual quantities
	DecomposedLimits DecomposeLimits(const std::vector<Limit>& v);

//	Method to read Limits from the combine output
	virtual void ReadCombineLimits(const std::string& file_name);
//	Method to read one limit
	const Limit ReadCombineLimit(const std::string& tfile_name, const bool& blindData);

	//	Make output .txt with limits written in a human readable way
	void Write(const std::string& name);

	//getters
	std::vector<Limit> getLimits() const;
	double getXMin() const;
	double getXMax() const;
	double getYMin() const;
	double getYMax() const;

	//setters
	void setLimits(std::vector<Limit> limits);
	void setXMin(const double& xmin);
	void setXMax(const double& xmax);
	void setYMin(const double& ymin);
	void setYMax(const double& ymax);

	//Plot sub-ranges "step plot"
	void PlotSubRangeSteps(
			const std::vector<Limit>& limits_sr1,
			const std::vector<Limit>& limits_sr2,
			const std::vector<Limit>& limits_sr3,
			const std::vector<Limit>& limits_sr4,
			const std::string& output = "",
			const std::string& Lumi = "36.0 fb^{-1}",
			const std::string& xtitle = "m_{A/H} [GeV]",
			const std::string& ytitle = "95%C.L. limit on #sigma(b#bar{b}A/H) x BR(A/H#rightarrow b#bar{b}) [pb]",
			const bool& logY = false,
			const bool& comp = false
			);

	//Plotting tools
	virtual void LimitPlotter(const LimitsToCompare& comp_limits,
			TLegend leg,
			const std::string& output = "",
			const std::string& Lumi = "36.0 fb^{-1}",
			const std::string& xtitle = "m_{A/H} [GeV]",
			const std::string& ytitle = "95%C.L. limit on #sigma(b#bar{b}A/H) x BR(A/H#rightarrow b#bar{b}) [pb]",
			const bool& logY = false);

	virtual void LimitPlotter(
			TLegend leg,
			const std::string& comp_limits = "",
			const std::string& output = "",
			const std::string& Lumi = "36.0 fb^{-1}",
			const std::string& xtitle = "m_{A/H} [GeV]",
			const std::string& ytitle = "95%C.L. limit on #sigma(b#bar{b}A/H) x BR(A/H#rightarrow b#bar{b}) [pb]",
			const bool& logY = false);

	virtual void LimitPlotter(
			TLegend leg,
			const std::string& output = "",
			const std::string& Lumi = "36.0 fb^{-1}",
			const std::string& xtitle = "m_{A/H} [GeV]",
			const std::string& ytitle = "95%C.L. limit on #sigma(b#bar{b}A/H) x BR(A/H#rightarrow b#bar{b}) [pb]",
			const bool& logY = false);

	void LimitPlotter(
			const std::string& output = "",
			const std::string& Lumi = "36.0 fb^{-1}",
			const std::string& xtitle = "m_{A/H} [GeV]",
			const std::string& ytitle = "95%C.L. limit on #sigma(b#bar{b}A/H) x BR(A/H#rightarrow b#bar{b}) [pb]",
			const bool& logY = false);

	virtual void AddPlottingObjects(TH2F &frame, TLegend &leg, TGraph& obs, TGraph& exp, TGraphAsymmErrors& inner_band, TGraphAsymmErrors& outer_band, TCanvas &can);

protected:

	std::vector<Limit> limits_;
	bool blindData_;
	bool TEST_;
	double xMin_;
	double xMax_;
	double yMin_;
	double yMax_;

};

inline std::vector<Limit> HbbLimits::getLimits() const {
	return limits_;
}

inline void HbbLimits::setLimits(
		std::vector<Limit> limits) {
	this->limits_ = limits;
}

inline void HbbLimits::setXMin(const double& xmin) {
	this->xMin_ = xmin;
}
inline void HbbLimits::setXMax(const double& xmax) {
	this->xMax_ = xmax;
}
inline void HbbLimits::setYMin(const double& ymin) {
	this->yMin_ = ymin;
}
inline void HbbLimits::setYMax(const double& ymax) {
	this->yMax_ = ymax;
}

inline double HbbLimits::getXMin() const {
	return xMin_;
}
inline double HbbLimits::getXMax() const{
	return xMax_;
}
inline double HbbLimits::getYMin() const{
	return yMin_;
}
inline double HbbLimits::getYMax() const{
	return yMax_;
}

} /* namespace MssmHbb */
} /* namespace Analysis */

#endif /* MSSMHBB_INTERFACE_HBBLIMITS_H_ */
