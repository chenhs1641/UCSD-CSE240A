// custom.h

#ifndef CUSTOM_H
#define CUSTOM_H

#include "predictor.h"
#include <vector>
#include <cstdint>

constexpr int THRESHOLD = 127;

class CustomPredictor : public Predictor {
private:
    int ghistoryBits;
    int lhistoryBits;
    int pcIndexBits;
    int tableSize;
    uint32_t ghr;
    std::vector<uint32_t> lht;
    std::vector<std::vector<int8_t>> perceptronTable;
    int perceptronSteps;

    uint32_t HashPC(uint32_t pc);

public:
    CustomPredictor(int ghb, int lhb, int pcb, int bpt, int vbs);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

#endif