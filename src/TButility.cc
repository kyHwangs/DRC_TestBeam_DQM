#include "TButility.h"

#include <iostream>
#include <stdexcept>
#include <fstream>

TBdetector::detid TButility::detid(int tid) const
{

  // if ( static_cast<int>(TBdetector::detid::DWC1analogue) <= tid &&
  //      static_cast<int>(TBdetector::detid::DWC1digital) > tid )
  //   return TBdetector::detid::DWC1analogue;
  // if ( static_cast<int>(TBdetector::detid::DWC1digital) <= tid &&
  //      static_cast<int>(TBdetector::detid::DWC2analogue) > tid )
  //   return TBdetector::detid::DWC1digital;
  // if ( static_cast<int>(TBdetector::detid::DWC2analogue) <= tid &&
  //      static_cast<int>(TBdetector::detid::DWC2digital) > tid )
  //   return TBdetector::detid::DWC2analogue;
  // if ( static_cast<int>(TBdetector::detid::DWC2digital) <= tid &&
  //      static_cast<int>(TBdetector::detid::extTrig1) > tid )
  //   return TBdetector::detid::DWC2digital;

  if (static_cast<int>(TBdetector::detid::aux) == tid)
    return TBdetector::detid::aux;
  if (static_cast<int>(TBdetector::detid::ext) == tid)
    return TBdetector::detid::ext;
  if (static_cast<int>(TBdetector::detid::ceren) == tid)
    return TBdetector::detid::ceren;
  if (static_cast<int>(TBdetector::detid::SFHS) == tid)
    return TBdetector::detid::SFHS;
  if (static_cast<int>(TBdetector::detid::LEGO) == tid)
    return TBdetector::detid::LEGO;
  if (static_cast<int>(TBdetector::detid::MCPPMT_gen) == tid)
    return TBdetector::detid::MCPPMT_gen;
  if (static_cast<int>(TBdetector::detid::MCPPMT) == tid)
    return TBdetector::detid::MCPPMT;
  if (static_cast<int>(TBdetector::detid::SiPM) == tid)
    return TBdetector::detid::SiPM;

  return TBdetector::detid::nulldet;
}

void TButility::loading(const std::string &path)
{

  int mid;
  int ch;
  int cases;
  int nChannel;
  int isCeren;
  int row;
  int column;

  std::cout << "Loading mapping file : " << path << std::endl;

  TChain *mapChain = new TChain("mapping");
  mapChain->Add((TString)path);

  mapChain->SetBranchAddress("mid", &mid);
  mapChain->SetBranchAddress("ch", &ch);
  mapChain->SetBranchAddress("cases", &cases);
  mapChain->SetBranchAddress("nChannel", &nChannel);
  mapChain->SetBranchAddress("isCeren", &isCeren);
  mapChain->SetBranchAddress("row", &row);
  mapChain->SetBranchAddress("column", &column);

  for (int i = 0; i < mapChain->GetEntries(); i++)
  {
    mapChain->GetEntry(i);

    auto cid = TBcid(mid, ch);

    // if ( module==3 ) {
    //   TBdetector::detid adetid = this->detid(tower);
    //   auto det = TBdetector(adetid);

    //   // column starts from 0 for all auxiliary detectors
    //   int col = tower - static_cast<int>(adetid);
    //   det.encodeColumn(col);

    //   mapping_.insert(std::make_pair(cid,det));

    //   continue;
    // }

    if (cases == 0)
    {
      TBdetector det = TBdetector(TBdetector::detid::nulldet);
      mapping_.insert(std::make_pair(cid, det));

      continue;
    }

    if (cases == 13 || cases == 14)
    {
      TBdetector det;
      if (cases == 13)
        det = TBdetector(TBdetector::detid::MCPPMT);
      if (cases == 14)
        det = TBdetector(TBdetector::detid::SiPM);

      det.encodeModule(cases, nChannel, static_cast<bool>(isCeren));
      det.encodeMultiCh(row, column);

      mapping_.insert(std::make_pair(cid, det));

      continue;
    }

    TBdetector det;
    if (cases == 10)
      det = TBdetector(TBdetector::detid::SFHS);
    if (cases == 11)
      det = TBdetector(TBdetector::detid::LEGO);
    if (cases == 12)
      det = TBdetector(TBdetector::detid::MCPPMT_gen);

    if (cases == 4)
      det = TBdetector(TBdetector::detid::ext);
    if (cases == 6)
      det = TBdetector(TBdetector::detid::ceren);

    det.encodeModule(cases, nChannel, static_cast<bool>(isCeren));

    mapping_.insert(std::make_pair(cid, det));
  }

  delete mapChain;
}

TBdetector TButility::find(const TBcid &cid) const
{
  if (mapping_.find(cid) == mapping_.end())
    return TBdetector(TBdetector::detid::nulldet);

  return mapping_.at(cid);
}

void TButility::loadped(const std::string &path)
{

  TH2F *pedMap = (TH2F *)(TFile::Open((TString)path)->Get("pedestal"));

  float ped;

  for (int mid = 1; mid <= 15; mid++)
  {
    for (int ch = 1; ch <= 32; ch++)
    {

      ped = pedMap->GetBinContent(mid, ch);

      auto cid = TBcid(mid, ch);
      pedmap_.insert(std::make_pair(cid, ped));
    }
  }

  delete pedMap;
};

float TButility::retrievePed(const TBcid &cid) const
{
  if (pedmap_.find(cid) == pedmap_.end())
  {
    cid.print();
    throw std::runtime_error("TButility - cannot find pedestal of the TBcid!");
  }

  return pedmap_.at(cid);
}

TBcid TButility::getcid(TBdetector::detid did) const
{

  for (auto detInfo : mapping_)
    if (detInfo.second.det() == did)
      return detInfo.first;

  return TBcid(0, 0);
}

TBcid TButility::getcid(TBdetector::detid did, int module, int tower, bool isCeren) const
{

  for (auto detInfo : mapping_)
    if (detInfo.second.det() == did && detInfo.second.module() == module && detInfo.second.tower() == tower && detInfo.second.isCeren() == isCeren)
      return detInfo.first;

  return TBcid(0, 0);
}

TBcid TButility::getcid(int module, int tower, bool isCeren) const
{

  for (auto detInfo : mapping_)
    if (detInfo.second.module() == module && detInfo.second.tower() == tower && detInfo.second.isCeren() == isCeren)
      return detInfo.first;

  return TBcid(0, 0);
}

TBcid TButility::getcid(int did, int module, int tower, bool isCeren) const
{

  TBdetector::detid detid = TButility::detid(did);

  for (auto detInfo : mapping_)
    if (detInfo.second.det() == detid && detInfo.second.module() == module && detInfo.second.tower() == tower && detInfo.second.isCeren() == isCeren)
      return detInfo.first;

  return TBcid(0, 0);
}

TBcid TButility::getcid(TBdetector::detid did, int module, int tower, int column, int row, bool isCeren) const
{

  for (auto detInfo : mapping_)
    if (detInfo.second.det() == did && detInfo.second.module() == module && detInfo.second.tower() == tower && detInfo.second.column() == column && detInfo.second.row() == row && detInfo.second.isCeren() == isCeren)
      return detInfo.first;

  return TBcid(0, 0);
}

TBcid TButility::getcid(TBdetector::detid did, int tower, int column, int row, bool isCeren) const
{

  for (auto detInfo : mapping_)
    if (detInfo.second.det() == did && detInfo.second.column() == column && detInfo.second.row() == row && detInfo.second.isCeren() == isCeren)
      return detInfo.first;

  return TBcid(0, 0);
}