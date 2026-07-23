#ifndef TBread_h
#define TBread_h 1

#ifndef _WIN32
#include <unistd.h>
#endif

#include "TBmid.h"
#include "TBevt.h"
#include <string>

template <class T>
class FileController
{
public:
  FileController(int fRunNum_, int fMID_, std::string fBaseDir_, int fMaxFileNum_);
  ~FileController()
  {
    fclose(fRawData);
  }

  TBmid<TBwaveform> ReadWaveformMid();
  TBmid<TBfastmode> ReadFastmodeMid();

  std::string GetFileName();
  std::string GetFileName(TBwaveform fMode_, int aFileNum);
  std::string GetFileName(TBfastmode fMode_, int aFileNum);

  std::string GetCurrentFileName() { return fFileName; }
  int GetMidNum() { return fMID; }
  int GetRunNum() { return fRunNum; }

  int GetCurrentMaxEvent() { return fCurrentMaxEventNum; }
  int GetCurrentEventNum() { return fCurrentEventNum; }
  int GetTotalEventNum() { return fTotalEventNum; }
  int GetTotalMaxEventNum() { return fTotalMaxEventNum; }

  void OpenFile();
  TBmid<T> ReadEvent();
  TBmid<TBwaveform> ReadEvent(TBwaveform fMode_);
  TBmid<TBfastmode> ReadEvent(TBfastmode fMode_);

  void CheckOverflow();

  int GetMaximum();
  int GetMaximum(TBwaveform fMode_, std::string filename);
  int GetMaximum(TBfastmode fMode_, std::string filename);

private:
  void init();

  T fMode;

  FILE *fRawData;
  TBmidbase readMetadata();

  // RawToMid fReader;

  std::string fFileName;
  std::string fBaseDir;
  int fRunNum;
  int fMID;
  int fNextFileNum;
  int fMaxFileNum;

  int fTotalEventNum;
  int fCurrentEventNum;
  int fTotalMaxEventNum;
  int fCurrentMaxEventNum;
};

template <class T>
class TBread
{
public:
  TBread(int fRunNum_, int fMaxEvent_, int fMaxFile_, std::string fBaseDir_, std::vector<int> fMIDMap_);
  ~TBread() {}

  TBevt<T> GetAnEvent();
  int GetMaxEvent() { return fMaxEvent; }

private:
  void init();

  int fRunNum;
  int fMaxEvent;
  int fMaxFile;
  std::string fBaseDir;

  std::vector<int> fMIDMap;

  std::map<int, FileController<T> *> fFileMap;
};

#endif
