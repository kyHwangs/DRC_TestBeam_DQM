#include "TBntupler.h"

TBntupler::TBntupler(std::string fInPath_, std::string fOutPath_, std::string fOutFileName_, int fRunNumber_, int fMaxEntry_, int fEntryPerFile_)
    : fInPath(fInPath_), fOutPath(fOutPath_), fOutFileName(fOutFileName_), fRunNumber(fRunNumber_), fMaxEntry(fMaxEntry_), fEntryPerFile(fEntryPerFile_)
{
    fMIDtoUse = {1, 2, 3, 4, 8, 9, 12};
}

void TBntupler::ntuplizeWaveform()
{
    // Prepare raw file reader
    TBread<TBwaveform> *file_reader = new TBread<TBwaveform>(fRunNumber, fMaxEntry, -1, fInPath, fMIDtoUse);

    // Set maximum number of entries, if not set, use total entries
    int entries = fMaxEntry;
    if (fMaxEntry == -1)
        entries = file_reader->GetMaxEvent();

    // Split output files
    // totalFileNumber is # of output files, and
    // fEntryPerOutputFile is entry per each output files
    int totalFileNumber = 1;
    fEntryPerOutputFile = {entries};
    if (!((fEntryPerFile == -1) || (fEntryPerFile > entries)))
    {
        fEntryPerOutputFile.clear();
        totalFileNumber = (int)entries / fEntryPerFile;
        if (!(((int)entries % fEntryPerFile) == 0))
            totalFileNumber += 1;
        for (int idx = 0; idx < totalFileNumber; idx++)
        {
            fEntryPerOutputFile.push_back(fEntryPerFile);
        }
        fEntryPerOutputFile[totalFileNumber - 1] = (int)entries % fEntryPerFile;
    }
    std::cout << "Total # of output files : " << totalFileNumber << std::endl;
    std::cout << "Total # of event per file : " << fEntryPerFile << std::endl;

    // Prepare ROOT TTree to store TBevt class
    int totalEvtIdx = 0;
    TBevt<TBwaveform> anEvent;
    TTree *rootTree = new TTree("event", "waveform events");
    rootTree->Branch("TBevt", &anEvent);
    rootTree->SetAutoSave(0);
    // Loop over events and store TBevt<TBwaveform> in root file
    for (int fileIdx = 0; fileIdx < totalFileNumber; fileIdx++)
    {
        int fileEntry = fEntryPerOutputFile.at(fileIdx);
        for (int iEvt = 0; iEvt < fileEntry; iEvt++)
        {
            anEvent = file_reader->GetAnEvent();
            rootTree->Fill();
            printProgress(totalEvtIdx, entries);
            totalEvtIdx++;
        }
        std::string rootFileName = (fOutPath + fOutFileName + "_" + std::to_string(fileIdx) + ".root");
        std::cout << "Splitting output root file with name : " << rootFileName << std::endl;

        TFile *outputRootFile = TFile::Open(rootFileName.c_str(), "RECREATE");
        outputRootFile->cd();
        rootTree->Write();
        outputRootFile->Close();
        delete outputRootFile;
        rootTree->Reset();
        rootTree->SetAutoSave(0);
    }
    delete rootTree;
}

void TBntupler::ntuplizeFastmode()
{
    // Prepare raw file reader
    TBread<TBfastmode> *file_reader = new TBread<TBfastmode>(fRunNumber, fMaxEntry, -1, fInPath, fMIDtoUse);

    // Set maximum number of entries, if not set, use total entries
    int entries = fMaxEntry;
    if (fMaxEntry == -1)
        entries = file_reader->GetMaxEvent();

    // Split output files
    // totalFileNumber is # of output files, and
    // fEntryPerOutputFile is entry per each output files
    int totalFileNumber = 1;
    fEntryPerOutputFile = {entries};
    if (!((fEntryPerFile == -1) || (fEntryPerFile > entries)))
    {
        fEntryPerOutputFile.clear();
        totalFileNumber = (int)entries / fEntryPerFile;
        if (!(((int)entries % fEntryPerFile) == 0))
            totalFileNumber += 1;
        for (int idx = 0; idx < totalFileNumber; idx++)
        {
            fEntryPerOutputFile.push_back(fEntryPerFile);
        }
        fEntryPerOutputFile[totalFileNumber - 1] = (int)entries % fEntryPerFile;
    }
    std::cout << "Total # of output files : " << totalFileNumber << std::endl;
    std::cout << "Total # of event per file : " << fEntryPerFile << std::endl;

    // Prepare ROOT TTree to store TBevt class
    int totalEvtIdx = 0;
    TBevt<TBfastmode> anEvent;
    TTree *rootTree = new TTree("event", "fastmode events");
    rootTree->Branch("TBevt", &anEvent);
    rootTree->SetAutoSave(0);
    // Loop over events and store TBevt<TBwaveform> in root file
    for (int fileIdx = 0; fileIdx < totalFileNumber; fileIdx++)
    {
        int fileEntry = fEntryPerOutputFile.at(fileIdx);
        for (int iEvt = 0; iEvt < fileEntry; iEvt++)
        {
            anEvent = file_reader->GetAnEvent();
            rootTree->Fill();
            printProgress(totalEvtIdx, entries);
            totalEvtIdx++;
        }
        std::string rootFileName = (fOutPath + fOutFileName + "_" + std::to_string(fileIdx) + ".root");
        std::cout << "Splitting output root file with name : " << rootFileName << std::endl;
        TFile *outputRootFile = TFile::Open(rootFileName.c_str(), "RECREATE");
        outputRootFile->cd();
        rootTree->Write();
        outputRootFile->Close();
        delete outputRootFile;
        rootTree->Reset();
        rootTree->SetAutoSave(0);
    }
    delete rootTree;
}

void TBntupler::printProgress(const int currentStep, const int totalStep)
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