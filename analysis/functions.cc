#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

float getPed(std::vector<short> waveform) {
  return std::accumulate( waveform.begin() + 1, waveform.begin() + 101, 0.) / 100.;
}

float getPedfromBack(std::vector<short> waveform) {
  return std::accumulate( waveform.end() - 24, waveform.end()  - 124, 0.) / 100.;
}

float getMin(std::vector<short> waveform) {
  return *(std::min_element(waveform.begin() + 1, waveform.end() - 23));
}

float getMinFrom(std::vector<short> waveform, int from, int end) {
  int minus = waveform.size() - end;
  return *(std::min_element(waveform.begin() + from, waveform.end() - minus));
}

int getMinIdx(std::vector<short> waveform) {
  return std::distance( waveform.begin(), std::min_element(waveform.begin() + 1, waveform.end() - 23) );
}

int getMinIdxFrom(std::vector<short> waveform, int from, int end) {
  int minus = waveform.size() - end;
  return std::distance( waveform.begin(), std::min_element(waveform.begin() + from, waveform.end() - minus) );
}

float interpolate(std::vector<short> waveform, int thrs_bin) {
  float x0 = (float) (thrs_bin - 1) * 0.2;
  float x1 = (float) thrs_bin * 0.2;
  float y0 = (float) getPed(waveform) - waveform.at(thrs_bin-1);
  float y1 = (float) getPed(waveform) - waveform.at(thrs_bin);

  float thrs = (getPed(waveform) - getMin(waveform)) * 0.3;

  return (float)( x0 + (thrs - y0) * (x1 - x0) / (y1 - y0) );
}

std::vector<float> GetAvg(std::vector<short> waveform, int maxEntry)
{
  std::vector<float> scaled_waveform;
  for(int i = 0; i < waveform.size(); i++)
    scaled_waveform.push_back( ( (float) waveform.at(i) / (float) maxEntry) );
  return scaled_waveform;
}

float GetInt(std::vector<short> waveform, int startBin, int endBin)
{
  float ped = std::accumulate(waveform.begin() + 1, waveform.begin() + 101, 0.) / 100.;
  
  std::vector<float> pedCorrectedWave;
  for (int i = 0; i < waveform.size(); i++)
    pedCorrectedWave.push_back(ped - waveform.at(i));

  return (std::accumulate(pedCorrectedWave.begin() + startBin, pedCorrectedWave.begin() + endBin, 0.));
}

float GetPeak(std::vector<short> waveform, int startBin, int endBin)
{
  float ped = std::accumulate(waveform.begin() + 1, waveform.begin() + 101, 0.) / 100.;
  
  std::vector<float> pedCorrectedWave;
  for (int i = 0; i < waveform.size(); i++)
    pedCorrectedWave.push_back(ped - waveform.at(i));

  return (*std::max_element(pedCorrectedWave.begin() + startBin, pedCorrectedWave.begin() + endBin));
}

std::map<std::string, std::vector<int>> getModuleConfigMap() {
    std::map<std::string, std::vector<int>> map_btw_MIDCH_and_Name;

    map_btw_MIDCH_and_Name.insert(std::make_pair("HW-Ceren", std::vector<int>{9, 2}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H1-Ceren", std::vector<int>{9, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H2-Ceren", std::vector<int>{9, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H3-Ceren", std::vector<int>{9, 20}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("HW-Scint", std::vector<int>{8, 2}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H1-Scint", std::vector<int>{8, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H2-Scint", std::vector<int>{8, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H3-Scint", std::vector<int>{8, 20}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("L1-Ceren", std::vector<int>{9, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L2-Ceren", std::vector<int>{9, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L3-Ceren", std::vector<int>{9, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L4-Ceren", std::vector<int>{9, 18}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("L1-Scint", std::vector<int>{8, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L2-Scint", std::vector<int>{8, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L3-Scint", std::vector<int>{8, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L4-Scint", std::vector<int>{8, 18}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("mid-Ceren", std::vector<int>{9, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W1-Ceren", std::vector<int>{9, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W2-Ceren", std::vector<int>{9, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W3-Ceren", std::vector<int>{9, 4}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("mid-Scint", std::vector<int>{8, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W1-Scint", std::vector<int>{8, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W2-Scint", std::vector<int>{8, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W3-Scint", std::vector<int>{8, 4}));
    
    map_btw_MIDCH_and_Name.insert(std::make_pair("1-Ceren",  std::vector<int>  {3, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("2-Ceren",  std::vector<int>  {3, 32}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("3-Ceren",  std::vector<int>  {4, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("4-Ceren",  std::vector<int>  {4, 32}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("5-Ceren",  std::vector<int>  {4, 15}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("6-Ceren",  std::vector<int>  {3, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("7-Ceren",  std::vector<int>  {3, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("8-Ceren",  std::vector<int>  {4, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("9-Ceren",  std::vector<int>  {4, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("10-Ceren", std::vector<int>  {4, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("11-Ceren", std::vector<int>  {3, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("12-Ceren", std::vector<int>  {3, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("13-Ceren", std::vector<int>  {4, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("14-Ceren", std::vector<int>  {4, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("15-Ceren", std::vector<int>  {4, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("16-Ceren", std::vector<int>  {3, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("17-Ceren", std::vector<int>  {3, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("18-Ceren", std::vector<int>  {4, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("19-Ceren", std::vector<int>  {4, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("20-Ceren", std::vector<int>  {4, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("21-Ceren", std::vector<int>  {3, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("22-Ceren", std::vector<int>  {3, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("23-Ceren", std::vector<int>  {3, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("24-Ceren", std::vector<int>  {3, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("25-Ceren", std::vector<int>  {3, 7} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("26-Ceren", std::vector<int>  {3, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("27-Ceren", std::vector<int>  {3, 24}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("28-Ceren", std::vector<int>  {4, 7} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("29-Ceren", std::vector<int>  {4, 8} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("30-Ceren", std::vector<int>  {4, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("31-Ceren", std::vector<int>  {3, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("32-Ceren", std::vector<int>  {3, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("33-Ceren", std::vector<int>  {4, 5} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("34-Ceren", std::vector<int>  {4, 6} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("35-Ceren", std::vector<int>  {4, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("36-Ceren", std::vector<int>  {3, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("37-Ceren", std::vector<int>  {3, 20}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("38-Ceren", std::vector<int>  {4, 3} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("39-Ceren", std::vector<int>  {4, 4} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("40-Ceren", std::vector<int>  {4, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("41-Ceren", std::vector<int>  {3, 8} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("42-Ceren", std::vector<int>  {3, 5} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("43-Ceren", std::vector<int>  {3, 6} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("44-Ceren", std::vector<int>  {3, 3} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("45-Ceren", std::vector<int>  {3, 4} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("46-Ceren", std::vector<int>  {3, 2} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("47-Ceren", std::vector<int>  {3, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("48-Ceren", std::vector<int>  {4, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("49-Ceren", std::vector<int>  {4, 2} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("50-Ceren", std::vector<int>  {4, 20}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("1-Scint",  std::vector<int>  {1, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("2-Scint",  std::vector<int>  {1, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("3-Scint",  std::vector<int>  {1, 32}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("4-Scint",  std::vector<int>  {2, 15}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("5-Scint",  std::vector<int>  {2, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("6-Scint",  std::vector<int>  {1, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("7-Scint",  std::vector<int>  {1, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("8-Scint",  std::vector<int>  {1, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("9-Scint",  std::vector<int>  {2, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("10-Scint", std::vector<int>  {2, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("11-Scint", std::vector<int>  {1, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("12-Scint", std::vector<int>  {1, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("13-Scint", std::vector<int>  {1, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("14-Scint", std::vector<int>  {2, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("15-Scint", std::vector<int>  {2, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("16-Scint", std::vector<int>  {1, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("17-Scint", std::vector<int>  {1, 7}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("18-Scint", std::vector<int>  {1, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("19-Scint", std::vector<int>  {2, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("20-Scint", std::vector<int>  {2, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("21-Scint", std::vector<int>  {2, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("22-Scint", std::vector<int>  {2, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("23-Scint", std::vector<int>  {2, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("24-Scint", std::vector<int>  {2, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("25-Scint", std::vector<int>  {2, 24} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("26-Scint", std::vector<int>  {1, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("27-Scint", std::vector<int>  {1, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("28-Scint", std::vector<int>  {1, 24} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("29-Scint", std::vector<int>  {2, 7} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("30-Scint", std::vector<int>  {2, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("31-Scint", std::vector<int>  {1, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("32-Scint", std::vector<int>  {1, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("33-Scint", std::vector<int>  {1, 22} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("34-Scint", std::vector<int>  {2, 5} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("35-Scint", std::vector<int>  {2, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("36-Scint", std::vector<int>  {1, 4}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("37-Scint", std::vector<int>  {1, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("38-Scint", std::vector<int>  {1, 20} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("39-Scint", std::vector<int>  {2, 3} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("40-Scint", std::vector<int>  {2, 4}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("41-Scint", std::vector<int>  {2, 23} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("42-Scint", std::vector<int>  {2, 21} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("43-Scint", std::vector<int>  {2, 22} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("44-Scint", std::vector<int>  {2, 19} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("45-Scint", std::vector<int>  {2, 20} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("46-Scint", std::vector<int>  {1, 2} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("47-Scint", std::vector<int>  {1, 3}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("48-Scint", std::vector<int>  {1, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("49-Scint", std::vector<int>  {2, 18} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("50-Scint", std::vector<int>  {2, 2}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("T1", std::vector<int> {8, 24}));  
    map_btw_MIDCH_and_Name.insert(std::make_pair("T2", std::vector<int> {9, 24})); 
    map_btw_MIDCH_and_Name.insert(std::make_pair("T3", std::vector<int> {8, 19})); 
    map_btw_MIDCH_and_Name.insert(std::make_pair("T4", std::vector<int> {9, 19})); 

    map_btw_MIDCH_and_Name.insert(std::make_pair("C1", std::vector<int> {12, 8}));  
    map_btw_MIDCH_and_Name.insert(std::make_pair("C2", std::vector<int> {12, 16}));  

    map_btw_MIDCH_and_Name.insert(std::make_pair("T1N", std::vector<int> {8, 7}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("T2N", std::vector<int> {9, 7}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("T3N", std::vector<int> {9, 15})); 
    map_btw_MIDCH_and_Name.insert(std::make_pair("Coin", std::vector<int> {8, 15}));

    return map_btw_MIDCH_and_Name;
}

void printProgress(const int currentStep, const int totalStep)
{
    // print progress
    float progress = (float)currentStep / totalStep;
    int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; i++)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "]  " << currentStep << "/" << totalStep << "  " << int(progress * 100.0) << "%\r";
    std::cout.flush();
}