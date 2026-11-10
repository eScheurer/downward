//
// Created by Enya Scheurer on 01/11/2026.
//

#ifndef LANDMARK_FACTORY_LM_CUT_H
#define LANDMARK_FACTORY_LM_CUT_H

#include "landmark_factory.h"
#include <unordered_set>

namespace landmarks {
class LandmarkFactoryLMCut : public LandmarkFactory {
    void generate_lm_cut_landmarks(const TaskProxy &task_proxy) const;
    virtual void generate_landmarks(
        const std::shared_ptr<AbstractTask> &task) override;

    std::unordered_set<int> first_achievers;
    std::unordered_set<int> possible_achievers;



public:
    LandmarkFactoryLMCut(
        const std::shared_ptr<AbstractTask> &task,
        utils::Verbosity verbosity);

    virtual bool supports_conditional_effects() const override;
};
}
#endif //LANDMARK_FACTORY_LM_CUT_H
