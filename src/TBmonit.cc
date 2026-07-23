#include "TBmonit.h"
#include "TBmid.h"
#include "TBevt.h"
#include "TBread.h"
#include "TButility.h"

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>

#include "TFile.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"

template <typename T>
TBmonit<T>::TBmonit(const std::string &fConfig_, int fRunNum_)
    : fConfig(TBconfig(fConfig_, fRunNum_)), fRunNum(fRunNum_), fMaxEvent(-1), fMaxFile(-1)
{
}

template <typename T>
void TBmonit<T>::Loop()
{
  TBplotengine enginePlot = TBplotengine(fConfig.GetConfig()["Plots"], fRunNum, TButility(), false);
  TBread<TBwaveform> readerWave = TBread<TBwaveform>(fRunNum, fMaxEvent, fMaxFile, "/u/user/swkim/SE_UserHome/2023_DRC_TB_Data", enginePlot.getMIDtoLoad());

  std::chrono::time_point time_begin = std::chrono::system_clock::now();

  int entries = fMaxEvent;
  if (fMaxEvent == -1)
    entries = readerWave.GetMaxEvent();

  std::cout << " " << std::endl;
  for (int i = 0; i < entries; i++)
  {
    if (i > 0 && i % 10 == 0)
    {
      std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; // delete
      float percent_done = 1. * i / (float)entries;
      std::chrono::duration time_left = time_taken * (1 / percent_done - 1);
      std::chrono::minutes minutes_left = std::chrono::duration_cast<std::chrono::minutes>(time_left);
      std::chrono::seconds seconds_left = std::chrono::duration_cast<std::chrono::seconds>(time_left - minutes_left);
      std::cout << "\r\033[F"
                << " " << i << " events  " << minutes_left.count() << ":";
      printf("%02d left   %.1f%%            \n", int(seconds_left.count()), percent_done * 100);
    }

    TBevt<TBwaveform> aEvent;
    aEvent = readerWave.GetAnEvent();
    enginePlot.Fill(aEvent);
  }

  enginePlot.SaveAs("DQM_Run" + std::to_string(fRunNum));

  std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; // delete
  std::chrono::minutes minutes_taken = std::chrono::duration_cast<std::chrono::minutes>(time_taken);
  std::chrono::seconds seconds_taken = std::chrono::duration_cast<std::chrono::seconds>(time_taken - minutes_taken);

  std::cout << "Time Taken : " << minutes_taken.count() << ":" << seconds_taken.count() << std::endl;
}

template <typename T>
void TBmonit<T>::LoopFast()
{
  TBfastengine enginePlot = TBfastengine(fConfig.GetConfig()["Plots"], fRunNum, TButility());
  TBread<TBfastmode> readerFast = TBread<TBfastmode>(fRunNum, fMaxEvent, fMaxFile, "/u/user/swkim/SE_UserHome/2023_DRC_TB_Data", enginePlot.getMIDtoLoad());

  std::chrono::time_point time_begin = std::chrono::system_clock::now();

  int entries = fMaxEvent;
  if (fMaxEvent == -1)
    entries = readerFast.GetMaxEvent();

  std::cout << " " << std::endl;
  for (int i = 0; i < entries; i++)
  {
    if (i > 0 && i % 10 == 0)
    {
      std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; // delete
      float percent_done = 1. * i / (float)entries;
      std::chrono::duration time_left = time_taken * (1 / percent_done - 1);
      std::chrono::minutes minutes_left = std::chrono::duration_cast<std::chrono::minutes>(time_left);
      std::chrono::seconds seconds_left = std::chrono::duration_cast<std::chrono::seconds>(time_left - minutes_left);
      std::cout << "\r\033[F"
                << " " << i << " events  " << minutes_left.count() << ":";
      printf("%02d left   %.1f%%            \n", int(seconds_left.count()), percent_done * 100);
    }

    TBevt<TBfastmode> aEvent;
    aEvent = readerFast.GetAnEvent();
    enginePlot.Fill(aEvent);
  }

  enginePlot.SaveAs("DQM_fast_Run" + std::to_string(fRunNum) + ".root");

  std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; // delete
  std::chrono::minutes minutes_taken = std::chrono::duration_cast<std::chrono::minutes>(time_taken);
  std::chrono::seconds seconds_taken = std::chrono::duration_cast<std::chrono::seconds>(time_taken - minutes_taken);

  std::cout << "Time Taken : " << minutes_taken.count() << ":" << seconds_taken.count() << std::endl;
}

template class TBmonit<TBwaveform>;
template class TBmonit<TBfastmode>;
