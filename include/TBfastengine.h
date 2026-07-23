#ifndef TBfastengine_h
#define TBfastengine_h 1

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

class fastPlotter
{
public:
  fastPlotter(){};
  ~fastPlotter(){};

  virtual void Fill(int input) {}
  virtual void Set(int input) {}
  virtual void SetHisto(TH1F *fHist_) {}
  virtual TH1F *GetHisto() { return fHist; }

private:
  TH1F *fHist;
};

class fIntADC : public fastPlotter
{
public:
  fIntADC() { fHist = new TH1F(); }
  ~fIntADC() {}

  virtual void Fill(int adc) { fHist->Fill(adc); }
  virtual void SetHisto(TH1F *fHist_) { fHist = fHist_; }
  virtual TH1F *GetHisto() { return fHist; }

private:
  TH1F *fHist;
};

class fTiming : public fastPlotter
{
public:
  fTiming()
  {
    fHist = new TH1F();
    fWaveformBinSize = -1;
  }
  ~fTiming() {}

  virtual void Fill(int clock_timing)
  {
    if (fWaveformBinSize == -1)
      throw std::runtime_error("Waveform bin size not defined, please set it before filling histogram!");
    float real_timing = (fWaveformBinSize * (1. / 16.) * (float)clock_timing);
    fHist->Fill(real_timing);
  }
  virtual void Set(int fWaveformBinSize_) { fWaveformBinSize = (float)fWaveformBinSize_; }
  virtual void SetHisto(TH1F *fHist_) { fHist = fHist_; }
  virtual TH1F *GetHisto() { return fHist; }

private:
  TH1F *fHist;
  float fWaveformBinSize;
};

class TBfastengine
{
public:
  TBfastengine(const YAML::Node fNodePlot_, int fRunNum_, TButility fUtility_);
  ~TBfastengine() {}

  enum PlotInfo
  {
    kIntADC = 0,
    kTiming
  };

  void init();

  TBcid getCid(std::string input);
  PlotInfo getPlotInfo(std::string input);
  fastPlotter *getPlot(std::string plotName, TBcid cid, PlotInfo which, const YAML::Node node);
  std::vector<int> getMIDtoLoad() { return fMIDtoLoad; }
  void PlotDWC(const YAML::Node node);
  void PrintInfo();

  TH1F *setDefault1D(const YAML::Node node);
  TH2F *setDefault2D(const YAML::Node node);
  TH1F *setIntADCplot(const YAML::Node node);
  TH1F *setTimingplot(const YAML::Node node);
  std::vector<TH1F *> setDWCplot(const YAML::Node node);

  void Fill(TBevt<TBfastmode> anEvent);

  void SaveAs(TString output);

private:
  const YAML::Node fNodePlot;
  int fRunNum;
  TButility fUtility;

  std::string fCaseName;

  std::vector<int> fMIDtoLoad;
  std::vector<TBcid> fCIDtoPlot;

  std::map<TBcid, std::vector<std::pair<PlotInfo, fastPlotter *>>> fPlotSet;
};

#endif