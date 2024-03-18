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
    choicePredictionTable = new uint8_t[1 << ghistoryBits];
    memset(choicePredictionTable, WEAK_GLOBAL, 1 << ghistoryBits);
    globalPredictionTable = new uint8_t[1 << ghistoryBits];
    memset(globalPredictionTable, WN, 1 << ghistoryBits);
    localBranchHistoryTable = new uint32_t[1 << pcIndexBits];
    memset(localBranchHistoryTable, 0, (1 << pcIndexBits) * sizeof(uint32_t));
    localPredictionTable = new uint8_t[1 << lhistoryBits];
    memset(localPredictionTable, WN, 1 << lhistoryBits);
}

uint8_t TournamentPredictor::make_prediction(uint32_t pc) {
    uint32_t gIndex = ghistoryRegister & gMask;
    uint32_t pcIndex = pc & pcMask;
    if (choicePredictionTable[gIndex] < WEAK_LOCAL) { // use global BHT with GHR
        return globalPredictionTable[gIndex] > WN;
    } else {
        uint32_t lIndex = localBranchHistoryTable[pcIndex] & lMask;
        return localPredictionTable[lIndex] > WN;
    };
}

void TournamentPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    // p1 is local, p2 is global
    uint32_t gIndex = ghistoryRegister & gMask;
    uint32_t pcIndex = pc & pcMask;
    uint32_t lIndex = localBranchHistoryTable[pcIndex] & lMask;
    bool p1c = (localPredictionTable[lIndex] > WN) == bool(outcome);
    bool p2c = (globalPredictionTable[gIndex] > WN) == bool(outcome);
    if (p1c < p2c && choicePredictionTable[gIndex] != GLOBAL) {
        choicePredictionTable[gIndex] --;
    } else if (p1c > p2c && choicePredictionTable[gIndex] != LOCAL) {
        choicePredictionTable[gIndex] ++;
    }
    if (outcome == TAKEN) {
        if (globalPredictionTable[gIndex] < ST) {
            globalPredictionTable[gIndex] ++;
        }
        if (localPredictionTable[lIndex] < ST) {
            localPredictionTable[lIndex] ++;
        }
    } else {
        if (globalPredictionTable[gIndex] > SN) {
            globalPredictionTable[gIndex] --;
        }
        if (localPredictionTable[lIndex] > SN) {
            localPredictionTable[lIndex] --;
        }
    }
    localBranchHistoryTable[pcIndex] <<= 1;
    localBranchHistoryTable[pcIndex] &= lMask;
    localBranchHistoryTable[pcIndex] ^= outcome & 1;
    ghistoryRegister <<= 1;
    ghistoryRegister &= gMask;
    ghistoryRegister ^= outcome & 1;
}
