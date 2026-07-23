#ifndef TBntupler_h
#define TBntupler_h 1

#include <vector>
#include <string>
#include <iostream>

#include "TBevt.h"
#include "TBread.h"
#include "TBmid.h"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

class TBntupler
{
public:
  TBntupler(std::string fInPath_, std::string fOutPath_, std::string fOutFileName_, int fRunNumber_, int fMaxEntry_, int fEntryPerFile_);
  // ~TBntupler() { delete fOutFile; }
  ~TBntupler() {}

  int getTotalEntry() { return fTotalEntry; }
  int getTotalNumOfFile() { return fNumOfOutput; }

  void setRunNumber(int fRunNumber_) { fRunNumber = fRunNumber_; }
  void setMaxEntry(int fMaxEntry_) { fMaxEntry = fMaxEntry_; }
  void setEntryPerFile(int fEntryPerFile_) { fEntryPerFile = fEntryPerFile_; }
  void setTotalNumOfFile(int fNumOfOutput_) { fNumOfOutput = fNumOfOutput_; }
  void setInputPath(std::string fInPath_) { fInPath = fInPath_; }
  void setOutputPath(std::string fOutPath_) { fOutPath = fOutPath_; }
  void setOutFileName(std::string fOutFileName_) { fOutFileName = fOutFileName_; }

  void ntuplizeWaveform();
  void ntuplizeFastmode();

  void printProgress(const int currentStep, const int totalStep);

private:
  std::string fInPath;
  std::string fOutPath;
  std::string fOutFileName;

  int fRunNumber;
  int fMaxEntry;
  int fEntryPerFile;
  int fTotalEntry;
  int fNumOfOutput;

  std::vector<int> fEntryPerOutputFile;
  std::vector<int> fMIDtoUse;
  // TFile* fOutFile;
};

#endif
