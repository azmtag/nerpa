//
// Created by olga on 22.01.19.
//

#include "Score.h"

bool matcher::Score::getScoreForSegment(const std::vector<aminoacid::Aminoacids::Aminoacid>& amns,
                        const nrpsprediction::NRPsPart& part, double& score) const {
    std::vector<nrpsprediction::AminoacidPrediction> aminoacid_predictions = part.getAminoacidsPrediction();
    int cnt_mismatch = 0;
    int g = 0;
    double segscor = 0;
    for (int j = 0; j < (int)aminoacid_predictions.size() && cnt_mismatch < 2; ++j) {
        if (!aminoacid_predictions[j].contain(amns[j])) {
            cnt_mismatch += 1;
        }

        segscor += aaScore(aminoacid_predictions[j], amns[j]);
    }

    if (cnt_mismatch == 0 || (cnt_mismatch == 1 && aminoacid_predictions.size() > 4)) {
        score = segscor;
        return true;
    }

    return false;
}

double matcher::Score::aaScore(const nrpsprediction::AminoacidPrediction &apred,
                               const aminoacid::Aminoacids::Aminoacid &aminoacid) const {
    double posscore[100];
    double curscore = 1;
    for (int i = 0; i < 100; ++i) {
        posscore[i] = curscore;
        curscore /= 1.25;
    }
    std::pair<int, int> position = apred.getAmnAcidPos(aminoacid);
    nrpsprediction::AminoacidPrediction::AminoacidProb prob = apred.getAminoacid(aminoacid);

    if (position.first == -1) {
        return -1;
    } else {
        int mdpos = (position.first + position.second)/2;
        if (mdpos >= 10) {
            return 0;
        }
        return prob.prob/100. * posscore[mdpos];
    }
}
