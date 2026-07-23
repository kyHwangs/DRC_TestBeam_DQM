#ifndef TBmonit_h
#define TBmonit_h 1

#include <string>
#include <iostream>

// #include <boost/python.hpp>

#include "TFile.h"
#include "TPad.h"
#include "TBconfig.h"
#include "TBread.h"

template <class T>
class TBmonit
{
public:
  TBmonit(const std::string &fConfig_, int fRunNum_);
  ~TBmonit() {}

  void Loop();

  void LoopFast(); //FIXME!! Fast engine should be integrated into TBplotengine, and work using template!!

  void SetMaxEvent(int fMaxEvent_) { fMaxEvent = fMaxEvent_; }
  void SetMaxFile(int fMaxFile_) { fMaxFile = fMaxFile_; }

private:
  TBconfig fConfig;

  int fRunNum;
  int fMaxEvent;
  int fMaxFile;
};

#endif
