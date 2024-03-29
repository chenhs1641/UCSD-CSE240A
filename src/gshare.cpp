//========================================================//
//  gshare.cpp                                            //
//  Source file for the Gshare Branch Predictor           //
//========================================================//
#include "predictor.h"
#include "gshare.h"
#include <bits/stdc++.h>
#include <iostream>

GsharePredictor::GsharePredictor(int ghb, int bpt, int vbs) : Predictor(bpt, vbs) { 
    ghistoryBits = ghb;
    ghistoryRegister = 0;
    gMask = 0;
    for (int i = 0; i < ghistoryBits; i ++) {
        gMask <<= 1;
        gMask |= 1;
    }
    if (verbose == 1) {
        std::cout<<std::hex<<gMask<<std::endl; //debug: print the gMask value
    }
    globalPredictionTable = new uint8_t[1 << ghistoryBits];
    memset(globalPredictionTable, WN, 1 << ghistoryBits);
}

uint8_t GsharePredictor::make_prediction(uint32_t pc) {
    uint32_t gIndex = (ghistoryRegister ^ pc) & gMask;
    return globalPredictionTable[gIndex] > WN;
}

void GsharePredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t gIndex = (ghistoryRegister ^ pc) & gMask;
    if (outcome == TAKEN && globalPredictionTable[gIndex] < ST) {
        globalPredictionTable[gIndex] ++;
    } else if (outcome == NOTTAKEN && globalPredictionTable[gIndex] > SN) {
        globalPredictionTable[gIndex] --;
    }
    ghistoryRegister <<= 1;
    ghistoryRegister &= gMask;
    ghistoryRegister ^= outcome & 1;
}