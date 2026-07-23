#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"


int mapping_generator(std::string inputMap) {

	int mid;
	int ch;
	int cases;
	int nChannel;
	int isCeren;
	int row;
	int column;

	TTree* aTree = new TTree("mapping", "mapping");

	aTree->Branch("mid", &mid);
	aTree->Branch("ch", &ch);
	aTree->Branch("cases", &cases);
	aTree->Branch("nChannel", &nChannel);
	aTree->Branch("isCeren", &isCeren);
	aTree->Branch("row", &row);
	aTree->Branch("column", &column);

  std::ifstream in;
  in.open(inputMap + ".csv", std::ios::in);
  while (true) {
    in >> mid >> ch >> cases >> nChannel >> isCeren >> row >> column;
    if (!in.good()) break;
    aTree->Fill();
  }
  in.close();


  TFile* aFile = new TFile((TString)(inputMap + ".root"), "RECREATE");
  aTree->Write();
  aFile->Close();



  return 1;
}