//========================================================//
//  predictor.h                                           //
//  Header file for the Branch Predictor                  //
//                                                        //
//  Includes function prototypes and global predictor     //
//  variables and defines                                 //
//========================================================//

#ifndef PREDICTOR_H
#define PREDICTOR_H
#include <bits/stdc++.h>

//------------------------------------//
//      Global Predictor Defines      //
//------------------------------------//
#define NOTTAKEN  0
#define TAKEN     1

// The Different Predictor Types
#define STATIC      0
#define GSHARE      1
#define TOURNAMENT  2
#define CUSTOM      3

// Definitions for 2-bit counters
#define SN  0			// predict NT, strong not taken
#define WN  1			// predict NT, weak not taken
#define WT  2			// predict T, weak taken
#define ST  3			// predict T, strong taken

// Def 2-bit take which
#define GLOBAL 0
#define WEAK_GLOBAL 1
#define WEAK_LOCAL 2
#define LOCAL 3

class Predictor {
//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//
protected:
    // Given
    const char *studentName = "NAME";
    const char *studentID   = "PID";
    const char *email       = "EMAIL";
    const char *bpName[4] = { "Static", "Gshare", "Tournament", "Custom" };
    int bpType;       // Branch Prediction Type
    int verbose;

//------------------------------------//
//    Predictor Function Prototypes   //
//------------------------------------//
public:
    Predictor(int bpt, int vbs); // Initialize the predictor
    virtual uint8_t make_prediction(uint32_t pc); // Make a prediction for conditional branch instruction at PC 'pc'. Returning TAKEN indicates a prediction of taken; returning NOTTAKEN indicates a prediction of not taken
    virtual void train_predictor(uint32_t pc, uint8_t outcome); // Train the predictor the last executed branch at PC 'pc' and with outcome 'outcome' (true indicates that the branch was taken, false indicates that the branch was not taken)
};

#endif
