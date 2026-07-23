#include "TBfastengine.h"

TBfastengine::TBfastengine(const YAML::Node fNodePlot_, int fRunNum_, TButility fUtility_)
		: fNodePlot(fNodePlot_), fRunNum(fRunNum_), fUtility(fUtility_), fCaseName("")
{
	init();
}

void TBfastengine::init()
{
	fUtility.loading("/u/user/swkim/DRC/2023_TB/TB2023/mapping/mapping_TB2021July_v1.root");
	std::cout << "starting INIT" << std::endl;

	for (const auto &aCase : fNodePlot["Cases"])
	{
		std::cout << " - Loading : " << aCase.first.as<std::string>() << std::endl;

		std::string plotName = aCase.first.as<std::string>();
		std::string pretty = "";

    if (plotName.find("EXT") != std::string::npos)
			pretty = plotName.substr(4, plotName.length() - 4);

		if (plotName.find("CEREN") != std::string::npos)
			pretty = plotName.substr(6, plotName.length() - 6);

		if (plotName.find("SFHS") != std::string::npos)
			pretty = plotName.substr(5, plotName.length() - 5);

		if (plotName.find("LEGO") != std::string::npos)
			pretty = plotName.substr(5, plotName.length() - 5);

		if (plotName.find("MCPPMT") != std::string::npos)
      pretty = plotName.substr(7, plotName.length() - 7);

		if (plotName.find("LEGO") != std::string::npos)
			pretty = plotName.substr(plotName.find("LEGO") + 5, 8);

		TBcid aCid = getCid(plotName);		// For plot CID
		std::vector<std::pair<PlotInfo, fastPlotter *>> aVec; // For plot storing per channel

		for (const auto &aPlot : aCase.second)
		{
			std::cout << " --- Generating : " << aPlot.first.as<std::string>() << "...   ";

			PlotInfo plot_enum = getPlotInfo(aPlot.first.as<std::string>());
			fastPlotter *plot = getPlot(pretty, aCid, plot_enum, aPlot.second);

			aVec.push_back(std::make_pair(plot_enum, plot));

			std::cout << "Done!" << std::endl;
		}

		fPlotSet.insert(std::make_pair(aCid, aVec));
	}

	fMIDtoLoad.clear();
	for (auto aPlotter : fPlotSet)
	{
		fCIDtoPlot.push_back(aPlotter.first);
		int amid = aPlotter.first.mid();

		bool isMatch = false;
		for (int i = 0; i < fMIDtoLoad.size(); i++)
			if (fMIDtoLoad.at(i) == amid)
				isMatch = true;

		if (!isMatch)
			fMIDtoLoad.push_back(amid);
	}

	PrintInfo();
}

TBcid TBfastengine::getCid(std::string input)
{
	if (input.find("EXT") != std::string::npos)
	{
		int nChannel = 0;

		if (input.find("T1") != std::string::npos)
			nChannel = 1;

		if (input.find("T2") != std::string::npos)
			nChannel = 2;

		if (input.find("Coin") != std::string::npos)
			nChannel = 3;

		return fUtility.getcid(TBdetector::detid::ext, 4, nChannel, 0);
	}

	if (input.find("CEREN") != std::string::npos)
	{
		int nChannel = 0;

		if (input.find("C1") != std::string::npos)
			nChannel = 1;

		if (input.find("C2") != std::string::npos)
			nChannel = 2;

		return fUtility.getcid(TBdetector::detid::ceren, 6, nChannel, 0);
	}

	if (input.find("MID") != std::string::npos && input.find("CH") != std::string::npos)
	{
		int mid = std::stoi(input.substr(input.find("MID") + 3, input.find("CH") - input.find("MID") - 4));
		int ch = std::stoi(input.substr(input.find("CH") + 2, input.size() - input.find("CH") - 2));

		if (mid > 15 || mid < 1)
			throw std::runtime_error("TBplotengine::getCid - allowed MID {1..15}, input is : " + std::to_string(mid));

		if (ch > 32 || ch < 1)
			throw std::runtime_error("TBplotengine::getCid - allowed CH {1..32}, input is : " + std::to_string(ch));

		return TBcid(mid, ch);
	}

	if (input.find("LEGO") != std::string::npos)
	{
		int nChannel = -1;
		int isCeren = -1;
		if (input.find("L1") != std::string::npos)
			nChannel = 1;

		if (input.find("L2") != std::string::npos)
			nChannel = 2;

		if (input.find("L3") != std::string::npos)
			nChannel = 3;

		if (input.find("L4") != std::string::npos)
			nChannel = 4;

		if (input.find("Ceren") != std::string::npos)
			isCeren = 1;

		if (input.find("Scint") != std::string::npos)
			isCeren = 0;

		return fUtility.getcid(TBdetector::detid::LEGO, 11, nChannel, isCeren);
	}

	if (input.find("SFHS") != std::string::npos)
	{
		int nChannel = -1;
		int isCeren = -1;
		if (input.find("HW") != std::string::npos)
			nChannel = 0;

		if (input.find("H1") != std::string::npos)
			nChannel = 1;

		if (input.find("H2") != std::string::npos)
			nChannel = 2;

		if (input.find("H3") != std::string::npos)
			nChannel = 3;

		if (input.find("Ceren") != std::string::npos)
			isCeren = 1;

		if (input.find("Scint") != std::string::npos)
			isCeren = 0;

		return fUtility.getcid(TBdetector::detid::SFHS, 10, nChannel, isCeren);
	}

	if (input.find("MCPPMT") != std::string::npos)
	{
		int nChannel = -1;
		int isCeren = -1;
		if (input.find("mid") != std::string::npos)
			nChannel = 0;

		if (input.find("W1") != std::string::npos)
			nChannel = 1;

		if (input.find("W2") != std::string::npos)
			nChannel = 2;

		if (input.find("W3") != std::string::npos)
			nChannel = 3;

		if (input.find("Ceren") != std::string::npos)
			isCeren = 1;

		if (input.find("Scint") != std::string::npos)
			isCeren = 0;

		return fUtility.getcid(TBdetector::detid::MCPPMT_gen, 12, nChannel, isCeren);
	}

	if (input.find("3D") != std::string::npos)
	{
		int isCeren = -1;
		if (input.find("Ceren") != std::string::npos)
			isCeren = 1;
		if (input.find("Scint") != std::string::npos)
			isCeren = 0;

		int Ch = std::stoi( input.substr(input.find_first_of("-") + 1, input.find_last_of("-") - input.find_first_of("-") - 1) );
		int nCol = ( 5 - (int)( (Ch-1) % 5 ));
		int nRow = ( (int)( (Ch - 1)/5 )+ 1);

		return fUtility.getcid(TBdetector::detid::MCPPMT, 13, nCol, nRow, isCeren);
	}
	
	return TBcid(1, 1);
}

TBfastengine::PlotInfo TBfastengine::getPlotInfo(std::string input)
{
	if (input == "integrated ADC")
		return PlotInfo::kIntADC;

	if (input == "timing")
		return PlotInfo::kTiming;
}

fastPlotter *TBfastengine::getPlot(std::string plotName, TBcid cid, TBfastengine::PlotInfo plot_enum, const YAML::Node node)
{
	// For drawing integrated ADC
	if (plot_enum == TBfastengine::PlotInfo::kIntADC)
	{
		TString name = "fast_IntADC_" + plotName;

		int nBin = node["nBin"].as<int>();
		float xMin = node["xMin"].as<float>();
		float xMax = node["xMax"].as<float>();

		fastPlotter *aPlotter = new fIntADC();
		aPlotter->SetHisto(new TH1F(name, (TString)(";Int. ADC; N_{events}"), nBin, xMin, xMax));

		return aPlotter;
	}

	// For drawing fast mode timing
	if (plot_enum == TBfastengine::PlotInfo::kTiming)
	{
		TString name = "fast_timing_Mid" + std::to_string(cid.mid()) + "Ch" + std::to_string(cid.channel());

		int nBin = node["nBin"].as<int>();
		float xMin = node["xMin"].as<float>();
		float xMax = node["xMax"].as<float>();
		int timeWindow = node["timeWindow"].as<int>();

		fastPlotter *aPlotter = new fTiming();
		aPlotter->Set(timeWindow);
		aPlotter->SetHisto(new TH1F(name, (TString)(";ps; N_{events}"), nBin, xMin, xMax));

		return aPlotter;
	}
	return new fastPlotter();
}

void TBfastengine::PrintInfo()
{

	std::cout << " " << std::endl;
	std::cout << " ### Plots to draw ### " << std::endl;
	for (auto aSet : fPlotSet)
	{

		std::cout << " ------------------------------ " << std::endl;
		std::cout << " - ";
		aSet.first.print();

		for (auto aPlot : aSet.second)
		{
			std::cout << " -- | " << aPlot.first << " " << aPlot.second->GetHisto()->GetName() << std::endl;
		}
	}
	std::cout << " ============================== " << std::endl;
	std::cout << " " << std::endl;
}

void TBfastengine::Fill(TBevt<TBfastmode> anEvent)
{
	for (int i = 0; i < fCIDtoPlot.size(); i++)
	{
		for (int j = 0; j < fPlotSet.at(fCIDtoPlot.at(i)).size(); j++)
		{
			TBfastengine::PlotInfo plot_type = fPlotSet.at(fCIDtoPlot.at(i)).at(j).first;
			if (plot_type == TBfastengine::PlotInfo::kIntADC)
			{
				fPlotSet.at(fCIDtoPlot.at(i)).at(j).second->Fill(anEvent.GetData(fCIDtoPlot.at(i)).adc());
			}
			if (plot_type == TBfastengine::PlotInfo::kTiming)
			{
				fPlotSet.at(fCIDtoPlot.at(i)).at(j).second->Fill(anEvent.GetData(fCIDtoPlot.at(i)).timing());
			}
		}
	}
}

void TBfastengine::SaveAs(TString output)
{
	output = (TString)("./ROOT/"+ output);
	TFile *outoutFile = new TFile(output, "RECREATE");

	outoutFile->cd();
	for (int i = 0; i < fCIDtoPlot.size(); i++)
		for (int j = 0; j < fPlotSet.at(fCIDtoPlot.at(i)).size(); j++)
			fPlotSet.at(fCIDtoPlot.at(i)).at(j).second->GetHisto()->Write();

	outoutFile->Close();
}
