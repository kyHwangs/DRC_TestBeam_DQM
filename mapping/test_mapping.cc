#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"


struct mapping_info
{
	int mid;
	int ch;
	int cases;
	int nChannel;
	int isCeren;
	int row;
	int column;

	mapping_info(	
		int mid_,
		int ch_,
		int cases_,
		int nChannel_,
		int isCeren_,
		int row_,
		int column_ 
	) :
	mid(mid_),
	ch(ch_),
	cases(cases_),
	nChannel(nChannel_),
	isCeren(isCeren_),
	row(row_),
	column(column_) {}

	bool operator==(mapping_info a) {
		if (a.mid == mid && 
				a.ch == ch &&
				a.cases == cases &&
				a.nChannel == nChannel &&
				a.isCeren == isCeren &&
				a.row == row &&
				a.column == column)
			return true;
		else
			return false;
	}
};


int test_mapping(std::string inputMap) {

	int mid;
	int ch;
	int cases;
	int nChannel;
	int isCeren;
	int row;
	int column;

	std::vector<mapping_info> mappingVec;
	std::ifstream in;
	in.open(inputMap + ".csv", std::ios::in);
	while (true) {
		in >> mid >> ch >> cases >> nChannel >> isCeren >> row >> column;
		if (!in.good()) break;

		mappingVec.push_back(mapping_info(mid, ch, cases, nChannel, isCeren, row, column));
	}
	in.close();
	std::cout << mappingVec.size() << std::endl;
	
	TChain* mapChain = new TChain("mapping");
	mapChain->Add((TString)(inputMap + ".root"));

	mapChain->SetBranchAddress("mid",&mid);
	mapChain->SetBranchAddress("ch",&ch);
	mapChain->SetBranchAddress("cases",&cases);
	mapChain->SetBranchAddress("nChannel",&nChannel);
	mapChain->SetBranchAddress("isCeren",&isCeren);
	mapChain->SetBranchAddress("row",&row);
	mapChain->SetBranchAddress("column",&column);

	for ( int i = 0; i < mapChain->GetEntries(); i++ ) {
		mapChain->GetEntry(i);

		mapping_info aInfo = mapping_info(mid, ch, cases, nChannel, isCeren, row, column);

		if( !(aInfo == mappingVec.at(i)) ) 
			std::cout << i << " is wrong !" << std::endl;
		else
			std::cout << i << " good" << std::endl;

	}


	return 1;
}

