#ifndef RAJENDRAN_CHAUDHURI_H
#define RAJENDRAN_CHAUDHURI_H

#include "Heuristic.h"

class RajendranChaudhuri : public Heuristic {
public:
    explicit RajendranChaudhuri(const Problem &prob) : problem(prob) {}
    std::vector<int> solve() override;

private:
    const Problem &problem;
    int bestInsertionPosition(const std::vector<int> &partialSchedule, int job);
};

#endif // RAJENDRAN_CHAUDHURI_H
