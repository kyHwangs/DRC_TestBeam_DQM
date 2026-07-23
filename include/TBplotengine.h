#ifndef TBplotengine_h
#define TBplotengine_h 1

#include <map>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <cmath>
#include <numeric>
#include <functional>

#include "TBconfig.h"
#include "TButility.h"
#include "TBdetector.h"
#include "TBmid.h"
#include "TBevt.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

class Plotter
{
	TH1F *aHist;

public:
	Plotter() = default;
	~Plotter() = default;

	virtual void Fill(std::vector<short> aWave) {}
	virtual void Set(std::vector<float> input) {}
	virtual void SetHisto(TH1F *input) {}
	virtual TH1F *GetHisto()
	{
		return aHist;
	}
};

class cIntADC : public Plotter
{
	float ped;
	float pStart;
	float pFinish;

	TH1F *aHist;

public:
	cIntADC() { aHist = new TH1F(); }
	~cIntADC() {}

	virtual void Fill(std::vector<short> aWave)
	{
		float aTmpPed = ped;
		if (ped == -1)
			aTmpPed = std::accumulate(aWave.begin() + 1, aWave.begin() + 101, 0.) / 100.;

		std::vector<float> aWavePedCorr;
		for (int i = 0; i < aWave.size(); i++)
			aWavePedCorr.push_back(aTmpPed - aWave.at(i));

		aHist->Fill(std::accumulate(aWavePedCorr.begin() + pStart, aWavePedCorr.begin() + pFinish, 0.));
	}

	virtual void Set(std::vector<float> input)
	{
		ped = input.at(0);
		pStart = input.at(1);
		pFinish = input.at(2);
	}

	virtual TH1F *GetHisto() { return aHist; }
	virtual void SetHisto(TH1F *input) { aHist = input; }
};

class cPeakADC : public Plotter
{
	float ped;
	float pStart;
	float pFinish;

	TH1F *aHist;

public:
	cPeakADC() { aHist = new TH1F(); }
	~cPeakADC() {}

	virtual void Fill(std::vector<short> aWave)
	{
		float aTmpPed = ped;
		if (ped == -1)
			aTmpPed = std::accumulate(aWave.begin() + 1, aWave.begin() + 101, 0.) / 100.;

		std::vector<float> aWavePedCorr;
		for (int i = 0; i < aWave.size(); i++)
			aWavePedCorr.push_back(aTmpPed - aWave.at(i));

		aHist->Fill(*std::max_element(aWavePedCorr.begin() + pStart, aWavePedCorr.begin() + pFinish));
	}

	virtual void Set(std::vector<float> input)
	{
		ped = input.at(0);
		pStart = input.at(1);
		pFinish = input.at(2);
	}

	virtual TH1F *GetHisto() { return aHist; }
	virtual void SetHisto(TH1F *input) { aHist = input; }
};

class cAvgTimeStruc : public Plotter
{
	TH1F *aHist;
	float nInput;
	double xScale;
	bool isScaled;
	bool isFilled;

public:
	cAvgTimeStruc() { aHist = new TH1F(); }
	~cAvgTimeStruc() {}

	virtual void Fill(std::vector<short> aWave)
	{
		if (!isFilled)
			isFilled = true;

		for (int i = 1; i <= 1000; i++)
			aHist->Fill(i, (float)aWave.at(i));

		nInput++;
	}

	virtual void Set(std::vector<float> input)
	{
		nInput = 0;
		isScaled = false;
		isFilled = false;
		xScale = input.at(0);
	}

	void ScaleHisto()
	{
		aHist->Scale(1. / (double)nInput);
		for (int i = 1; i <= aHist->GetNbinsX(); i++)
			aHist->SetBinError(i, 0.);

		isScaled = true;
	}

	virtual TH1F *GetHisto()
	{
		if (isFilled && !isScaled)
			ScaleHisto();

		return aHist;
	}

	void SetHisto(TH1F *input) { aHist = input; }
};

class TBplotengine
{
public:
	TBplotengine(const YAML::Node fNodePlot_, int fRunNum_, TButility fUtility_, bool fUseExPed_);
	~TBplotengine() {}

	enum PlotInfo
	{
		kIntADC = 0,
		kPeakADC,
		kAvgTimeStruc
	};

	void init();

	TBcid getCid(std::string input);
	PlotInfo getPlotInfo(std::string input);
	Plotter *getPlot(std::string plotName, TBcid cid, PlotInfo which, const YAML::Node node);
	std::vector<int> getMIDtoLoad() { return fMIDtoLoad; }
	void PrintInfo();

	TH1F *setDefault1D(const YAML::Node node);
	TH2F *setDefault2D(const YAML::Node node);
	TH1F *setIntADCplot(const YAML::Node node);
	TH1F *setPeakADCplot(const YAML::Node node);
	TH1F *setAvgStructureplot(const YAML::Node node);
	std::vector<TH1F *> setDWCplot(const YAML::Node node);

	void Fill(TBevt<TBwaveform> anEvent);

	void SaveAs(TString output);

private:
	const YAML::Node fNodePlot;
	int fRunNum;
	TButility fUtility;
	bool fUseExPed;

	std::string fCaseName;

	std::vector<int> fMIDtoLoad;
	std::vector<TBcid> fCIDtoPlot;

	std::map<TBcid, std::vector<std::pair<PlotInfo, Plotter *>>> fPlotSet;
};

#endif