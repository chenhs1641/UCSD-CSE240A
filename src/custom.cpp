// custom.cpp

#include "predictor.h"
#include "custom.h"
#include <cmath>
#include <cstdlib>

CustomPredictor::CustomPredictor(int ghb, int lhb, int pcb, int bpt, int vbs)
    : Predictor::Predictor(bpt, vbs), ghistoryBits(ghb), lhistoryBits(lhb), pcIndexBits(pcb)
{
    tableSize = 1 << pcIndexBits;
    ghr = 0;
    lht = std::vector<uint32_t>(tableSize, 0);
    perceptronTable = std::vector<std::vector<int8_t>>(tableSize, std::vector<int8_t>(ghistoryBits + lhistoryBits + 1, 0));
}

uint8_t CustomPredictor::make_prediction(uint32_t pc)
{
    uint32_t perceptronIndex = HashPC(pc);
    int32_t prediction = perceptronTable[perceptronIndex][0];

    for (int i = 0; i < ghistoryBits; i++) {
        prediction += ((ghr >> i) & 1) ? perceptronTable[perceptronIndex][i + 1] : -perceptronTable[perceptronIndex][i + 1];
    }

    for (int i = 0; i < lhistoryBits; i++) {
        prediction += ((lht[perceptronIndex] >> i) & 1) ? perceptronTable[perceptronIndex][ghistoryBits + i + 1] : -perceptronTable[perceptronIndex][ghistoryBits + i + 1];
    }

    perceptronSteps = std::abs(prediction);
    return (prediction >= 0) ? TAKEN : NOTTAKEN;
}

void CustomPredictor::train_predictor(uint32_t pc, uint8_t outcome)
{
    uint32_t perceptronIndex = HashPC(pc);
    uint8_t predDir = make_prediction(pc);

    if (outcome != predDir || perceptronSteps <= THRESHOLD) {
        int8_t updateVal = (outcome == TAKEN) ? 1 : -1;
        perceptronTable[perceptronIndex][0] = std::clamp(static_cast<int>(perceptronTable[perceptronIndex][0]) + updateVal, -128, 127);

        for (int i = 0; i < ghistoryBits; i++) {
            bool condition = (outcome == TAKEN && ((ghr >> i) & 1)) || (outcome == NOTTAKEN && !((ghr >> i) & 1));
            updateVal = condition ? 1 : -1;
            perceptronTable[perceptronIndex][i + 1] = std::clamp(static_cast<int>(perceptronTable[perceptronIndex][i + 1]) + updateVal, -THRESHOLD, THRESHOLD);
        }

        for (int i = 0; i < lhistoryBits; i++) {
            bool condition = (outcome == TAKEN && ((lht[perceptronIndex] >> i) & 1)) || (outcome == NOTTAKEN && !((lht[perceptronIndex] >> i) & 1));
            updateVal = condition ? 1 : -1;
            perceptronTable[perceptronIndex][ghistoryBits + i + 1] = std::clamp(static_cast<int>(perceptronTable[perceptronIndex][ghistoryBits + i + 1]) + updateVal, -THRESHOLD, THRESHOLD);
        }
    }

    ghr = ((ghr << 1) | (outcome == TAKEN)) & ((1 << ghistoryBits) - 1);
    lht[perceptronIndex] = ((lht[perceptronIndex] << 1) | (outcome == TAKEN)) & ((1 << lhistoryBits) - 1);
}

uint32_t CustomPredictor::HashPC(uint32_t pc)
{
    return pc & ((1 << pcIndexBits) - 1);
}