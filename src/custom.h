//========================================================//
//  custom.h                                              //
//  Header file for the custom Branch Predictor           //
//========================================================//
#include "predictor.h"

#define MAX_INT 10000
#define MIN_INT -10000

class CustomPredictor : public Predictor {
private:
    int ghistoryBits; // Number of bits used for Global History, also used for F
    int pcIndexBits;  // Number of bits used for PC index
    // self defined:
    uint64_t ghistoryRegister;
    uint64_t gMask;
    uint32_t pcMask;
    int threshold;
    std::unordered_map<uint32_t, int*> functionCoefficients;
    int vec_mul(int* vecF);
public:
    CustomPredictor(int ghb, int pcb, int bpt, int vbs);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};