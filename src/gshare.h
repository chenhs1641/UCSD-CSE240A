//========================================================//
//  gshare.h                                              //
//  Header file for the Gshare Branch Predictor           //
//========================================================//
#include "predictor.h"

class GsharePredictor : public Predictor {
private:
    int ghistoryBits; // Number of bits used for Global History
    // self defined:
    int ghistoryRegister;
    uint32_t gMask;
    uint8_t* globalPredictionTable;
public:
    GsharePredictor(int ghb, int bpt, int vbs);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};
