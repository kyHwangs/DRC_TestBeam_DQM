#ifndef TBmcppmt_h
#define TBmcppmt_h 1

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
#include "TBread.h"
#include "TButility.h"
#include "TBdetector.h"
#include "TBmid.h"
#include "TBevt.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TApplication.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TRootCanvas.h"
#include "TCanvas.h"

template <class T>
class TBmcppmt
{
public:
  enum calculator
  {
    AvgTimeStruc = 0,
    PeakADC,
    IntADC
  };

  TBmcppmt(int fRunNum_, calculator fCalc_, bool fUseExPed_);
  ~TBmcppmt()
  {
    delete fReaderWave;
    delete fReaderFast;
  }

  void SetReader(TBwaveform mode)
  {
    fReaderWave = new TBread<TBwaveform>(fRunNum, fMaxEvent, -1, "/Users/yhep/scratch/YUdaq", std::vector<int>{1, 2, 3, 4});
    fReaderFast = nullptr;
  }
  void SetReader(TBfastmode mode)
  {
    fReaderFast = new TBread<TBfastmode>(fRunNum, fMaxEvent, -1, "/Users/yhep/scratch/YUdaq", std::vector<int>{1, 2, 3, 4});
    fReaderWave = nullptr;
  }

  void Loop();
  void PreparePlots();

  void SetMapping(std::string path) { fUtility.loading(path); }
  void SetPedestal(std::string path);
  void SetMaxEvent(int max) { fMaxEvent = max; }
  void SetHeatmap() { fDoHeatMap = true; }
  void SetEventHeatmap()
  {
    fDoEvent = true;
    if (fCalc == calculator::AvgTimeStruc)
      fDoEvent = false;

    if (fDoEvent)
    {
      fCanvas = new TCanvas("c", "c", 1600, 800);
      fCanvas->Divide(2, 1);
      fHist2DCeren = new TH2F((TString)("Heatmap_Event_" + std::to_string(static_cast<int>(fCalc)) + "_Ceren"), "", 5, 0, 5, 10, 0, 10);
      fHist2DScint = new TH2F((TString)("Heatmap_Event_" + std::to_string(static_cast<int>(fCalc)) + "_Scint"), "", 5, 0, 5, 10, 0, 10);
      fHist2DCeren->SetStats(0);
      fHist2DScint->SetStats(0);
    }
  }

  void GetAnEvent(TBwaveform mode) { fAnWaveEvent = fReaderWave->GetAnEvent(); }
  void GetAnEvent(TBfastmode mode) { fAnFastEvent = fReaderFast->GetAnEvent(); }

  void GetData(TBwaveform mode);
  void GetData(TBfastmode mode);

  void Fill(TBfastmode mode);

  void Fill(TBwaveform mode);
  void FillAverageTimeStruc();
  void FillPeakADC();
  void FillIntADC();

  void EndOfLoop();

  TString GetOutputName(TBwaveform mode) { return (TString)("./MCPPMT/DQM_MCPPMT_Wave_" + std::to_string(fRunNum) + "_" + std::to_string(static_cast<int>(fCalc)) + ".root"); }
  TString GetOutputName(TBfastmode mode) { return (TString)("./MCPPMT/DQM_MCPPMT_Fast_" + std::to_string(fRunNum) + "_" + std::to_string(static_cast<int>(fCalc)) + ".root"); }

  TString GetEvtOutputName(TBwaveform mode) { return (TString)("./MCPPMT/DQM_MCPPMT_Evt_Wave_" + std::to_string(fRunNum) + "_" + std::to_string(static_cast<int>(fCalc)) + ".root"); }
  TString GetEvtOutputName(TBfastmode mode) { return (TString)("./MCPPMT/DQM_MCPPMT_Evt_Fast_" + std::to_string(fRunNum) + "_" + std::to_string(static_cast<int>(fCalc)) + ".root"); }

  void SetPlotRangeX(int nbin, float min, float max);
  void SetCalcRangeX(float min, float max);

  int GetRow(int input) { return (int)(input / 5) + 1; }
  int GetCol(int input) { return (int)(input % 5) + 1; }

  TH2F *GetScintHeatmap();
  TH2F *GetCerenHeatmap();

  void DrawEventHeatMap(int iEvt);

  void PrepareEvtLoop();
  void EndEvtLoop();
  T getMode() { return fMode; }
  TCanvas* GetEventHeatMap(int iEvt);
  void SaveEventHeatMap(int iEvt);

private:
  T fMode;

  int fRunNum;
  calculator fCalc;
  bool fUseExPed;
  int fMaxEvent;
  bool fDoHeatMap;
  bool fDoEvent;

  TFile* fOutfile;
  TCanvas *fCanvas;

  TButility fUtility;

  TBread<TBwaveform> *fReaderWave;
  TBread<TBfastmode> *fReaderFast;

  std::vector<TH1F *> fPlotCeren;
  std::vector<TH1F *> fPlotScint;

  std::vector<TBcid> fCIDCeren;
  std::vector<TBcid> fCIDScint;

  std::vector<TBwaveform> fChCerenWave;
  std::vector<TBwaveform> fChScintWave;

  std::vector<TBfastmode> fChCerenFast;
  std::vector<TBfastmode> fChScintFast;

  TBevt<TBwaveform> fAnWaveEvent;
  TBevt<TBfastmode> fAnFastEvent;

  int fNBin;
  float fPlotXmin;
  float fPlotXmax;

  float fCalcRangeMin;
  float fCalcRangeMax;

  TH2F *fHist2DCeren;
  TH2F *fHist2DScint;
};

#endif
