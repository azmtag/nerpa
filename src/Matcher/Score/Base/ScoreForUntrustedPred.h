//
// Created by olga on 07.12.19.
//

#ifndef NERPA_SCOREFORUNTRUSTEDPRED_H
#define NERPA_SCOREFORUNTRUSTEDPRED_H

#include "Matcher/Score/Base/Score.h"

namespace matcher {
    class ScoreForUntrustedPred : public Score {
    private:
        double prob_threshold = 300;
    public:
        ScoreForUntrustedPred(std::unique_ptr<Score> base) : Score(std::move(base)) {}
        ScoreForUntrustedPred(std::unique_ptr<Score> base, const std::string& predictor) : Score(std::move(base)) {
            if (predictor == "MINOWA") {
                prob_threshold = 300.;
            } else if (predictor == "NRPSPREDICTOR2") {
                prob_threshold = 80.;
            } else if (predictor == "SANDPUMA") {
                prob_threshold = 80.;
            } else if (predictor == "PRISM") {
                prob_threshold = 400.;
            }
        }

        double
        aaScore(const nrpsprediction::AminoacidPrediction &apred, const aminoacid::Aminoacid &aminoacid) const override {
            auto AAprobs = apred.getAAPrediction();
            assert(baseScore != nullptr);
            auto aa_score = baseScore->aaScore(apred, aminoacid);
            if (AAprobs[0].prob < 300) {
                return std::max(0., aa_score);
            }

            return aa_score;
        }

        std::pair<double, aminoacid::Aminoacid> getTheBestAAInPred(const nrpsprediction::AminoacidPrediction &apred,
                                                                   const aminoacid::Aminoacid &aminoacid,
                                                                   nrpsprediction::AminoacidPrediction::AminoacidProb &probRes,
                                                                   std::pair<int, int> &posRes) const override {
            auto AAprobs = apred.getAAPrediction();
            assert(baseScore != nullptr);
            auto best_aa = baseScore->getTheBestAAInPred(apred, aminoacid, probRes, posRes);

            if (AAprobs[0].prob < 300) {
                if (best_aa.first < 0) {
                    return std::make_pair(0., aminoacid::Aminoacid("none"));
                }
            }

            return best_aa;
        };

        double EPS = 1e-5;
    };
}


#endif //NERPA_SCOREFORUNTRUSTEDPRED_H
