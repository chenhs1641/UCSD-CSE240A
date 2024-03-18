//========================================================//
//  tournament.cpp                                        //
//  Source file for the Tournament Branch Predictor       //
//========================================================//
#include "predictor.h"
#include "tournament.h"
#include <bits/stdc++.h>
#include <iostream>

TournamentPredictor::TournamentPredictor(int ghb, int lhb, int pcb, int bpt, int vbs) : Predictor(bpt, vbs) {
    ghistoryBits = ghb;
    lhistoryBits = lhb;
    pcIndexBits = pcb;
    ghistoryRegister = 0;
    gMask = 0;
    for (int i = 0; i < ghistoryBits; i ++) {
        gMask <<= 1;
        gMask |= 1;
    }
    if (verbose == 1) {
        std::cout<<std::hex<<gMask<<std::endl; //debug: print the ghrMask value
    }
    lMask = 0;
    for (int i = 0; i < lhistoryBits; i ++) {
        lMask <<= 1;
        lMask |= 1;
    }
    if (verbose == 1) {
        std::cout<<std::hex<<lMask<<std::endl; //debug: print the ghrMask value
    }
    pcMask = 0;
    for (int i = 0; i < pcIndexBits; i ++) {
        pcMask <<= 1;
        pcMask |= 1;
    }
    if (verbose == 1) {
        std::cout<<std::hex<<pcMask<<std::endl; //debug: print the ghrMask value
    }
}

uint8_t TournamentPredictor::make_prediction(uint32_t pc) {
    uint32_t gIndex = (ghistoryRegister ^ pc) & gMask;
    uint32_t lIndex = pc & lMask;
    uint32_t pcIndex = pc & pcMask;
    if (chooser[pcIndex] < WEAK_LOCAL) { // use global BHT with GHR
        return globalBranchHistoryTable[gIndex];
    } else {
        return localBranchHistoryTable[lIndex];
    };
}

void TournamentPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    // p1 is local, p2 is global
    uint32_t gIndex = (ghistoryRegister ^ pc) & gMask;
    uint32_t lIndex = pc & lMask;
    uint32_t pcIndex = pc & pcMask;
    bool p1c = (localBranchHistoryTable[lIndex]) == outcome;
    bool p2c = (globalBranchHistoryTable[gIndex]) == outcome;
    if (p1c < p2c && chooser[pcIndex] != GLOBAL) {
        chooser[pcIndex] --;
    } else if (p1c > p2c && chooser[pcIndex] != LOCAL) {
        chooser[pcIndex] ++;
    }
    localBranchHistoryTable[lIndex] = outcome;
    globalBranchHistoryTable[gIndex] = outcome;
    ghistoryRegister <<= 1;
    ghistoryRegister &= gMask;
    ghistoryRegister ^= outcome & 1;
}
