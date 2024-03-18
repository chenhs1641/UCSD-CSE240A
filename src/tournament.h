//========================================================//
//  tournament.h                                          //
//  Header file for the Tournament Branch Predictor       //
//========================================================//
#include "predictor.h"

class TournamentPredictor : public Predictor {
private:
    int ghistoryBits; // Number of bits used for Global History
    int lhistoryBits; // Number of bits used for Local History
    int pcIndexBits;  // Number of bits used for PC index
    // self defined:
    int ghistoryRegister;
    uint32_t gMask;
    uint32_t lMask;
    uint32_t pcMask;
    std::unordered_map<uint32_t, uint8_t> chooser;
    std::unordered_map<uint32_t, uint8_t> globalBranchHistoryTable;
    std::unordered_map<uint32_t, uint8_t> localBranchHistoryTable;
public:
    TournamentPredictor(int ghb, int lhb, int pcb, int bpt, int vbs);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};