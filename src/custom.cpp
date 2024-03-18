//========================================================//
//  custom.cpp                                            //
//  Source file for the custom Branch Predictor           //
//========================================================//
#include "predictor.h"
#include "custom.h"
#include <bits/stdc++.h>
#include <iostream>

int CustomPredictor::vec_mul(int* vecF) {
    int sum = vecF[ghistoryBits];
    for (int i = 0; i < ghistoryBits; i ++) {
        if ((ghistoryRegister >> i & 1) == 1) {
            sum += vecF[i];
        } else {
            sum -= vecF[i];
        }
    }
    return sum;
}

CustomPredictor::CustomPredictor(int ghb, int pcb, int bpt, int vbs) : Predictor::Predictor(bpt, vbs) {
    ghistoryBits = ghb;
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
    pcMask = 0;
    for (int i = 0; i < pcIndexBits; i ++) {
        pcMask <<= 1;
        pcMask |= 1;
    }
    if (verbose == 1) {
        std::cout<<std::hex<<pcMask<<std::endl; //debug: print the ghrMask value
    }
    threshold = 0;
}

uint8_t CustomPredictor::make_prediction(uint32_t pc) {
    uint32_t fIndex = pc & pcMask;
    if (functionCoefficients.find(fIndex) == functionCoefficients.end()) {
        return NOTTAKEN;
    }
    int* vecF = functionCoefficients[fIndex];
    int mul = vec_mul(vecF);
    if (mul > threshold) {
        return TAKEN;
    } else {
        return NOTTAKEN;
    }
}

void CustomPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t fIndex = pc & pcMask;
    if (functionCoefficients.find(fIndex) == functionCoefficients.end()) {
        functionCoefficients[fIndex] = new int[ghistoryBits + 1];
        memset(functionCoefficients[fIndex], 0, (ghistoryBits + 1) * sizeof(int));
    }
    uint8_t pred = make_prediction(pc);
    if (outcome != pred) {
        if (outcome == 1) {
            for (int i = 0; i < ghistoryBits; i ++) {
                if ((ghistoryRegister >> i & 1) == 1) {
                    if (functionCoefficients[fIndex][i] < MAX_INT) {
                        functionCoefficients[fIndex][i] ++;
                    }
                } else {
                    if (functionCoefficients[fIndex][i] > MIN_INT) {
                        functionCoefficients[fIndex][i] --;
                    }
                }
            }
            if (functionCoefficients[fIndex][ghistoryBits] < MAX_INT) {
                functionCoefficients[fIndex][ghistoryBits] ++;
            }
        } else {
            for (int i = 0; i < ghistoryBits; i ++) {
                if ((ghistoryRegister >> i & 1) == 1) {
                    if (functionCoefficients[fIndex][i] > MIN_INT) {
                        functionCoefficients[fIndex][i] --;
                    }
                } else {
                    if (functionCoefficients[fIndex][i] < MAX_INT) {
                        functionCoefficients[fIndex][i] ++;
                    }
                }
            }
            if (functionCoefficients[fIndex][ghistoryBits] > MIN_INT) {
                functionCoefficients[fIndex][ghistoryBits] --;
            }
        }
    }
    ghistoryRegister <<= 1;
    ghistoryRegister &= gMask;
    ghistoryRegister ^= outcome & 1;
}