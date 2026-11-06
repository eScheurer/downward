//
// Created by Enya Scheurer on 01/11/2026.
//
#include "landmark_factory_lm_cut.h"
#include "../heuristics/lm_cut_landmarks.h"
#include "../task_proxy.h"
#include "../plugins/plugin.h"

namespace landmarks {
    LandmarkFactoryLMCut::LandmarkFactoryLMCut(
        const std::shared_ptr<AbstractTask> &task,
        utils::Verbosity verbosity)
        : LandmarkFactory(task, verbosity) {}

    void LandmarkFactoryLMCut::generate_goal_landmarks( //brauchts wahrscheinlich nicht
        const TaskProxy &task_proxy) const {
        //TODO: implement
    }

    void LandmarkFactoryLMCut::generate_all_atomic_landmarks(const TaskProxy &task_proxy) const {
        //TODO: implement
    }

    void LandmarkFactoryLMCut::generate_landmarks(const std::shared_ptr<AbstractTask> &task) {
        //TODO: implement
    }

    bool LandmarkFactoryLMCut::supports_conditional_effects() const {
        return false;
    }

    class LandmarkFactoryLMCutFeature
        : public plugins::TypedFeature<TaskIndependentLandmarkFactory> {
    public:
        LandmarkFactoryLMCutFeature() : TypedFeature("lm_cut") {
            add_landmark_factory_options_to_feature(*this);
        }

    virtual std::shared_ptr<TaskIndependentLandmarkFactory> create_component(
        const plugins::Options &opts) const override {
        return components::make_auto_task_independent_component<
            LandmarkFactoryLMCut, LandmarkFactory>(
            get_landmark_factory_arguments_from_options(opts));
        }
    };
    static plugins::FeaturePlugin<LandmarkFactoryLMCutFeature> _plugin;
}

