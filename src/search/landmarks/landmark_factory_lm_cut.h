//
// Created by Enya Scheurer on 01/11/2026.
//

#ifndef LANDMARK_FACTORY_LM_CUT_H
#define LANDMARK_FACTORY_LM_CUT_H

#include "landmark_factory.h"

namespace landmarks {
class LandmarkFactoryLMCut : public LandmarkFactory {
    void generate_goal_landmarks(const TaskProxy &task_proxy) const;
    void generate_all_atomic_landmarks(
        const TaskProxy &task_proxy) const;
    virtual void generate_landmarks(
        const std::shared_ptr<AbstractTask> &task) override;

public:
    LandmarkFactoryLMCut(
        const std::shared_ptr<AbstractTask> &task,
        utils::Verbosity verbosity);

    virtual bool supports_conditional_effects() const override;
};
}
#endif //LANDMARK_FACTORY_LM_CUT_H
