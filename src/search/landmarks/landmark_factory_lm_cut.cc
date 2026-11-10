//
// Created by Enya Scheurer on 01/11/2026.
//
#include "landmark_factory_lm_cut.h"
#include "../heuristics/lm_cut_landmarks.h"
#include "../task_proxy.h"
#include "../plugins/plugin.h"

using namespace std;

namespace landmarks {
    LandmarkFactoryLMCut::LandmarkFactoryLMCut(
        const std::shared_ptr<AbstractTask> &task,
        utils::Verbosity verbosity)
        : LandmarkFactory(task, verbosity) {}

    void LandmarkFactoryLMCut::generate_lm_cut_landmarks(const TaskProxy &task_proxy) const {
        lm_cut_heuristic::LandmarkCutLandmarks landmarks(task_proxy, true, true);

        State state = task_proxy.get_initial_state();
        vector<vector<int>> landmarkSet;

        landmarks.compute_landmarks(
            state, nullptr,
            [&landmarkSet](const vector<int> &lm, int){landmarkSet.push_back(lm);}); //The landmark_callback Lambda was constructed with the help of AI. TODO: propper declaration

        for (const vector<int> &L : landmarkSet) { //for each
            Landmark l({}, DISJUNCTIVE_ACTION);
            l.possible_achievers.insert(L.begin(), L.end());
            l.first_achievers.insert(L.begin(), L.end());
            landmark_graph->add_landmark(move(l));
        }
    }

    void LandmarkFactoryLMCut::generate_landmarks(const std::shared_ptr<AbstractTask> &task) {
        TaskProxy task_proxy(*task);
        if (log.is_at_least_normal()) {
            log << "Generating landmarks in the initial state with LM-Cut."
                << endl;
        }
        generate_lm_cut_landmarks(task_proxy);
        achievers_calculated = true;
        landmark_graph->set_landmark_ids();
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

