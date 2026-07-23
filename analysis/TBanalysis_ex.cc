#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "TBevt.h"
#include "TButility.h"

#include "functions.cc"

#include "TStyle.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"

// Run the script by
// ./TBanalysis_ex <run number> <max entry>
// For example, ./TBanalysis_ex 4234 -1
// <max entry> == -1 means run all the entries

int main(int argc, char** argv) {
    gStyle->SetOptFit(1);

    int runNum = std::stoi(argv[1]);
    int maxEntry = std::stoi(argv[2]);

    // Load mapping, pedestal info is not available for now, will be updated
    auto map = getModuleConfigMap();
    std::vector<int> MCP_sum_C_MID_and_Ch = map.at("mid-Ceren"); // MCPPMT summation C ch.
    std::vector<int> MCP_sum_S_MID_and_Ch = map.at("mid-Scint"); // MCPPMT summation S ch.
    std::vector<int> C_counter_1_MID_and_Ch = map.at("C1"); // Cerenkov counter 1
    std::vector<int> C_counter_2_MID_and_Ch = map.at("C2"); // Cerenkov counter 2

    // Map returns MID and Ch. in form of std::vector<int> { MID, Ch. }
    TBcid MCP_sum_C_cid = TBcid(MCP_sum_C_MID_and_Ch.at(0), MCP_sum_C_MID_and_Ch.at(1));
    TBcid MCP_sum_S_cid = TBcid(MCP_sum_S_MID_and_Ch.at(0), MCP_sum_S_MID_and_Ch.at(1));
    // Also prepare Ceren counter cid for PID
    TBcid C_counter_1_cid = TBcid(C_counter_1_MID_and_Ch.at(0), C_counter_1_MID_and_Ch.at(1)); // Get Ceren counter info for PID
    TBcid C_counter_2_cid = TBcid(C_counter_2_MID_and_Ch.at(0), C_counter_2_MID_and_Ch.at(1)); // Get Ceren counter info for PID

    // Preparing histograms
    TH1F* Hist_MCP_sum_C_AvgTimeStruc = new TH1F("MCP_sum_C_AvgTimeStruc", "MCP_sum_C_AvgTimeStruc;bin;ADC", 1000, 0, 1000);
    TH1F* Hist_MCP_sum_C_PeakADC      = new TH1F("MCP_sum_C_PeakADC", "MCP_sum_C_PeakADC;PeakADC;Evt", 1024, 0, 4096);
    TH1F* Hist_MCP_sum_C_IntADC       = new TH1F("MCP_sum_C_IntADC", "MCP_sum_C_IntADC;IntADC;Evt", 1000, -5000, 200000);

    TH1F* Hist_MCP_sum_S_AvgTimeStruc = new TH1F("MCP_sum_S_AvgTimeStruc", "MCP_sum_S_AvgTimeStruc;bin;ADC", 1000, 0, 1000);
    TH1F* Hist_MCP_sum_S_PeakADC      = new TH1F("MCP_sum_S_PeakADC", "MCP_sum_S_PeakADC;PeakADC;Evt", 1024, 0, 4096);
    TH1F* Hist_MCP_sum_S_IntADC       = new TH1F("MCP_sum_S_IntADC", "MCP_sum_S_IntADC;IntADC;Evt", 1000, -5000, 200000);

    TH1F* Hist_MCP_sum_C_AvgTimeStruc_cut = new TH1F("MCP_sum_C_AvgTimeStruc_cut", "MCP_sum_C_AvgTimeStruc_cut;bin;ADC", 1000, 0, 1000);
    TH1F* Hist_MCP_sum_C_PeakADC_cut      = new TH1F("MCP_sum_C_PeakADC_cut", "MCP_sum_C_PeakADC_cut;PeakADC;Evt", 1024, 0, 4096);
    TH1F* Hist_MCP_sum_C_IntADC_cut       = new TH1F("MCP_sum_C_IntADC_cut", "MCP_sum_C_IntADC_cut;IntADC;Evt", 1000, -5000, 200000);

    TH1F* Hist_MCP_sum_S_AvgTimeStruc_cut = new TH1F("MCP_sum_S_AvgTimeStruc_cut", "MCP_sum_S_AvgTimeStruc_cut;bin;ADC", 1000, 0, 1000);
    TH1F* Hist_MCP_sum_S_PeakADC_cut      = new TH1F("MCP_sum_S_PeakADC_cut", "MCP_sum_S_PeakADC_cut;PeakADC;Evt", 1024, 0, 4096);
    TH1F* Hist_MCP_sum_S_IntADC_cut       = new TH1F("MCP_sum_S_IntADC_cut", "MCP_sum_S_IntADC_cut;IntADC;Evt", 1000, -5000, 200000);

    TH1F* Hist_C_counter_1_PeakADC = new TH1F("C_counter_1_PeakADC", "C_counter_1_PeakADC;PeakADC;Evt", 1024, 0, 4096);
    TH1F* Hist_C_counter_2_PeakADC = new TH1F("C_counter_2_PeakADC", "C_counter_2_PeakADC;PeakADC;Evt", 1024, 0, 4096);

    TH1F* Hist_C_counter_1_PeakADC_cut = new TH1F("C_counter_1_PeakADC_cut", "C_counter_1_PeakADC_cut;PeakADC;Evt", 1024, 0, 4096);
    TH1F* Hist_C_counter_2_PeakADC_cut = new TH1F("C_counter_2_PeakADC_cut", "C_counter_2_PeakADC_cut;PeakADC;Evt", 1024, 0, 4096);

    // Load Ntuple using TChain
    TChain* evtChain = new TChain("event");
    for (int fn = 0; fn < 50; fn++) {
        std::string fileName = "ntuple_Run_" + std::to_string(runNum) + "_Wave_" + std::to_string(fn) + ".root";
        std::string filePath = "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2023_DRC_TB_ntuple/Run_"  + std::to_string(runNum) + "/Run_" + std::to_string(runNum) + "_Wave/"+ fileName;
        if ( !access(filePath.c_str(), F_OK) ){
            std::cout << fn << " Ntuple file added to TChain : " << filePath << std::endl;
            evtChain->Add(filePath.c_str());
        }
    }
    TBevt<TBwaveform>* anEvt = new TBevt<TBwaveform>(); // Will use waveform data
    evtChain->SetBranchAddress("TBevt", &anEvt);

    // Check total # of events, limit # of events to process if maxEntry is specified
    int totalEntry = evtChain->GetEntries();
    std::cout << "Total entries : " << totalEntry << std::endl;
    if ( (maxEntry > 0) && (maxEntry < totalEntry) ) {
        totalEntry = maxEntry;
        std::cout << "Will process maximum " << std::to_string(totalEntry) << " events" << std::endl;
    }

    // Container to hold waveform subtracted from pedestal (evt-by-evt)
    std::vector<float> MCP_sum_C_pedCorrectedWave;
    std::vector<float> MCP_sum_S_pedCorrectedWave;
    // PID cut threshold setting
    float C_counter_1_cut = 50.; // Use Peak ADC 50. as C counter 1 cut
    float C_counter_2_cut = 50.; // Use Peak ADC 50. as C counter 2 cut
    // Evt Loop start
    for (int iEvt = 0; iEvt < totalEntry; iEvt++) {
        printProgress(iEvt + 1, totalEntry); // Print progress bar
        evtChain->GetEntry(iEvt); // Get event

        TBwaveform MCP_sum_C_Data = anEvt->GetData(MCP_sum_C_cid); // Get DRC data from event
        TBwaveform MCP_sum_S_Data = anEvt->GetData(MCP_sum_S_cid); // Get DRC data from event
        std::vector<short> MCP_sum_C_waveform = MCP_sum_C_Data.waveform(); // From data, get waveform
        std::vector<short> MCP_sum_S_waveform = MCP_sum_S_Data.waveform(); // From data, get waveform

        TBwaveform C_counter_1_Data = anEvt->GetData(C_counter_1_cid); // Get C counter data from event
        TBwaveform C_counter_2_Data = anEvt->GetData(C_counter_2_cid); // Get C counter data from event
        std::vector<short> C_counter_1_waveform = C_counter_1_Data.waveform(); // From data, get waveform
        std::vector<short> C_counter_2_waveform = C_counter_2_Data.waveform(); // From data, get waveform

        // To manually calculate pedestal, PeakADC and Int ADC, follow recipe of MCPPMT summations channels
        float MCP_sum_C_ped = std::accumulate( MCP_sum_C_waveform.begin() + 1, MCP_sum_C_waveform.begin() + 101, 0.) / 100.; // Use first 100 bins to get prompt pedestal, will be updated
        float MCP_sum_S_ped = std::accumulate( MCP_sum_S_waveform.begin() + 1, MCP_sum_S_waveform.begin() + 101, 0.) / 100.; // Use first 100 bins to get prompt pedestal, will be updated

        MCP_sum_C_pedCorrectedWave.clear();
        MCP_sum_S_pedCorrectedWave.clear();
        for (int bin = 0; bin < 1000; bin++) {
            int waveformBin = bin + 1;
            Hist_MCP_sum_C_AvgTimeStruc->Fill(bin, MCP_sum_C_waveform[waveformBin]); // Fill avg time struc histogram (will be scaled later)
            Hist_MCP_sum_S_AvgTimeStruc->Fill(bin, MCP_sum_S_waveform[waveformBin]); // Fill avg time struc histogram (will be scaled later)

            MCP_sum_C_pedCorrectedWave.push_back(MCP_sum_C_ped - MCP_sum_C_waveform[waveformBin]); // Fill (ped - waveform) in container
            MCP_sum_S_pedCorrectedWave.push_back(MCP_sum_S_ped - MCP_sum_S_waveform[waveformBin]); // Fill (ped - waveform) in container
        }

        // Peak search Start bin : 150 End bin : 300
        // Calculate peak ADC and fill histograms 
        Hist_MCP_sum_C_PeakADC->Fill(*std::max_element(MCP_sum_C_pedCorrectedWave.begin() + 150, MCP_sum_C_pedCorrectedWave.begin() + 300)); // Fill MCPPMT summation ch. PeakADC
        Hist_MCP_sum_S_PeakADC->Fill(*std::max_element(MCP_sum_S_pedCorrectedWave.begin() + 150, MCP_sum_S_pedCorrectedWave.begin() + 300)); // Fill MCPPMT summation ch. PeakADC

        // Int. search Start bin : 150 End bin : 300
        // Calculate Int. ADC and fill histograms 
        Hist_MCP_sum_C_IntADC->Fill(std::accumulate(MCP_sum_C_pedCorrectedWave.begin() + 150, MCP_sum_C_pedCorrectedWave.begin() + 300, 0.)); // Fill MCPPMT summation ch. IntADC
        Hist_MCP_sum_S_IntADC->Fill(std::accumulate(MCP_sum_S_pedCorrectedWave.begin() + 150, MCP_sum_S_pedCorrectedWave.begin() + 300, 0.)); // Fill MCPPMT summation ch. IntADC

        // To calculate PeakADC with pre-defined functions inside function.cc, follow recipe of Ceren. counter
        float C_counter_1_PeakADC = GetPeak(C_counter_1_waveform, 100, 700); // Calculate PeakADC of Ceren. counter 1 with [100, 700) bin
        float C_counter_2_PeakADC = GetPeak(C_counter_2_waveform, 100, 700); // Calculate PeakADC of Ceren. counter 2 with [100, 700) bin
        Hist_C_counter_1_PeakADC->Fill(C_counter_1_PeakADC); // Fill C counter 1 PeakADC hist.
        Hist_C_counter_2_PeakADC->Fill(C_counter_2_PeakADC); // Fill C counter 2 PeakADC hist.

        // PID cut >> will be different for which particle, which gas pressure the Run is using! need to check the PID logic run-by-run
        if ( (C_counter_1_PeakADC < C_counter_1_cut) || (C_counter_2_PeakADC < C_counter_2_cut) ) continue; // This is for selecting e+e-, using over pedestal events of Ceren. counter
        
        Hist_C_counter_1_PeakADC_cut->Fill(C_counter_1_PeakADC); // Fill C counter 1 PeakADC hist. after cut
        Hist_C_counter_2_PeakADC_cut->Fill(C_counter_2_PeakADC); // Fill C counter 1 PeakADC hist. after cut 

        for (int bin = 0; bin < 1000; bin++) {
            int waveformBin = bin + 1;
            Hist_MCP_sum_C_AvgTimeStruc_cut->Fill(bin, MCP_sum_C_waveform[waveformBin]); // Fill avg time struc histogram (will be scaled later) after cut
            Hist_MCP_sum_S_AvgTimeStruc_cut->Fill(bin, MCP_sum_S_waveform[waveformBin]); // Fill avg time struc histogram (will be scaled later) after cut
        }
        // Peak search Start bin : 150 End bin : 300
        // Calculate peak ADC and fill histograms 
        Hist_MCP_sum_C_PeakADC_cut->Fill(*std::max_element(MCP_sum_C_pedCorrectedWave.begin() + 150, MCP_sum_C_pedCorrectedWave.begin() + 300)); // Fill MCPPMT summation ch. PeakADC after cut
        Hist_MCP_sum_S_PeakADC_cut->Fill(*std::max_element(MCP_sum_S_pedCorrectedWave.begin() + 150, MCP_sum_S_pedCorrectedWave.begin() + 300)); // Fill MCPPMT summation ch. PeakADC after cut

        // Int. search Start bin : 150 End bin : 300
        // Calculate Int. ADC and fill histograms 
        Hist_MCP_sum_C_IntADC_cut->Fill(std::accumulate(MCP_sum_C_pedCorrectedWave.begin() + 150, MCP_sum_C_pedCorrectedWave.begin() + 300, 0.)); // Fill MCPPMT summation ch. IntADC after cut
        Hist_MCP_sum_S_IntADC_cut->Fill(std::accumulate(MCP_sum_S_pedCorrectedWave.begin() + 150, MCP_sum_S_pedCorrectedWave.begin() + 300, 0.)); // Fill MCPPMT summation ch. IntADC after cut
    }

    // Save the outputs
    std::string outFile = "./Sample_analysis_Run_" + std::to_string(runNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    // Save before-cut histograms
    Hist_MCP_sum_C_AvgTimeStruc->Scale( (float)(1. / (float) totalEntry));
    Hist_MCP_sum_C_AvgTimeStruc->GetYaxis()->SetRangeUser(-100, 4096);
    Hist_MCP_sum_C_AvgTimeStruc->Write();
    Hist_MCP_sum_C_PeakADC->Write();
    Hist_MCP_sum_C_IntADC->Write();

    Hist_MCP_sum_S_AvgTimeStruc->SetLineColor(kRed);
    Hist_MCP_sum_S_AvgTimeStruc->Scale( (float)(1. / (float) totalEntry));
    Hist_MCP_sum_S_AvgTimeStruc->GetYaxis()->SetRangeUser(-100, 4096);
    Hist_MCP_sum_S_AvgTimeStruc->Write();
    Hist_MCP_sum_S_PeakADC->SetLineColor(kRed);
    Hist_MCP_sum_S_PeakADC->Write();
    Hist_MCP_sum_S_IntADC->SetLineColor(kRed);
    Hist_MCP_sum_S_IntADC->Write();

    Hist_C_counter_1_PeakADC->Write();
    Hist_C_counter_2_PeakADC->Write();

    // Save after-cut histograms
    float passed_evt = Hist_MCP_sum_C_PeakADC_cut->GetEntries();
    Hist_MCP_sum_C_AvgTimeStruc_cut->Scale( (float)(1. / (float) passed_evt));
    Hist_MCP_sum_C_AvgTimeStruc_cut->GetYaxis()->SetRangeUser(-100, 4096);
    Hist_MCP_sum_C_AvgTimeStruc_cut->Write();
    Hist_MCP_sum_C_PeakADC_cut->Write();
    Hist_MCP_sum_C_IntADC_cut->Write();

    Hist_MCP_sum_S_AvgTimeStruc_cut->SetLineColor(kRed);
    Hist_MCP_sum_S_AvgTimeStruc_cut->Scale( (float)(1. / (float) passed_evt));
    Hist_MCP_sum_S_AvgTimeStruc_cut->GetYaxis()->SetRangeUser(-100, 4096);
    Hist_MCP_sum_S_AvgTimeStruc_cut->Write();
    Hist_MCP_sum_S_PeakADC_cut->SetLineColor(kRed);
    Hist_MCP_sum_S_PeakADC_cut->Write();
    Hist_MCP_sum_S_IntADC_cut->SetLineColor(kRed);
    Hist_MCP_sum_S_IntADC_cut->Write();

    Hist_C_counter_1_PeakADC_cut->Write();
    Hist_C_counter_2_PeakADC_cut->Write();

    outputRoot->Close();
}
