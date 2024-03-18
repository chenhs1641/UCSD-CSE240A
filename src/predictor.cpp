//========================================================//
//  predictor.cpp                                         //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include <iostream>
#include "predictor.h"


//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
Predictor::Predictor(int bpt, int vbs) {
    // Initialize Branch Predictor Data Structures
    bpType = bpt;
    verbose = vbs;
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//

uint8_t Predictor::make_prediction(uint32_t pc) {
    return TAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//

void Predictor::train_predictor(uint32_t pc, uint8_t outcome) {
  
}
