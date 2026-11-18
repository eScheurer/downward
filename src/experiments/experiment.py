#! /usr/bin/env python

"""
This script uses the FastDownwardExperiment class, which makes the script very
concise, but harder to extend once you go beyond "standard" experiments.
Therefore, we recommend using the Experiment class directly. See the script in
examples/downward/2020-09-11-B-bounded-cost.py for an example.
"""

import os

import custom_parser
import project

REPO = project.get_repo_base()
BENCHMARKS_DIR = os.environ["DOWNWARD_BENCHMARKS"]
SCP_LOGIN = "scheur0004" #"myname@myserver.com"
REMOTE_REPOS_DIR = "/infai/username/projects"
# If REVISION_CACHE is None, the default "./data/revision-cache/" is used.
REVISION_CACHE = os.environ.get("DOWNWARD_REVISION_CACHE")
if project.REMOTE:
    SUITE = project.SUITE_OPTIMAL_STRIPS # SUITE_OPTIMAL_STRIPS???
    ENV = project.BaselSlurmEnvironment(email="scheur0004@login12.scicore.unibas.ch") #my.name@myhost.ch"
else:
    SUITE = ["blocks:probBLOCKS-4-0.pddl", "airport:p01-airport1-p1.pddl", "gripper:prob01.pddl"]
    ENV = project.LocalEnvironment(processes=2)

CONFIGS = [
    ("lmcut", ["--search", "astar(lmcut())"]),
    ("action_lm_cut", ["--search", "astar(landmark_cost_partitioning(lm_cut()))"]
    )
]
BUILD_OPTIONS = []
DRIVER_OPTIONS = []  # TODO: 30min for big test? + mem limit "--overall-memory-limit", "8G"

REV_NICKS = [
    ("developement", ""),
]
ATTRIBUTES = [
    "error",
    "total_time",
    "coverage",
    "expansions",
    "memory",
    "dead_ends",
    "evaluations",
    "generated",
    "reopened",
    "search_time",
    project.EVALUATIONS_PER_TIME,
]

exp = project.FastDownwardExperiment(environment=ENV, revision_cache=REVISION_CACHE)
for config_nick, config in CONFIGS:
    for rev, rev_nick in REV_NICKS:
        algo_name = f"{rev_nick}:{config_nick}" if rev_nick else config_nick
        exp.add_algorithm(
            algo_name,
            REPO,
            rev,
            config,
            build_options=BUILD_OPTIONS,
            driver_options=DRIVER_OPTIONS,
        )
exp.add_suite(BENCHMARKS_DIR, SUITE)

exp.add_parser(exp.EXITCODE_PARSER)
exp.add_parser(exp.TRANSLATOR_PARSER)
exp.add_parser(exp.SINGLE_SEARCH_PARSER)
exp.add_parser(custom_parser.get_parser())
exp.add_parser(exp.PLANNER_PARSER)

exp.add_step("build", exp.build)
exp.add_step("start", exp.start_runs)
exp.add_step("parse", exp.parse)
exp.add_fetcher(name="fetch")

project.add_absolute_report(
    exp, attributes=ATTRIBUTES, filter=[project.add_evaluations_per_time]
)

# The following block of code was made by AI. It solves a macOS-specific problem and removes the memory limit for local usage.
# TODO: proper AI declaration
def strip_memory_limit(opts):
    result, skip = [], False
    for opt in opts:
        if skip:
            skip = False
        elif opt == "--overall-memory-limit":
            skip = True
        else:
            result.append(opt)
    return result
if not project.REMOTE:
    for algo in exp._algorithms.values():
        algo.driver_options = strip_memory_limit(algo.driver_options)



if not project.REMOTE:
    project.add_scp_step(exp, SCP_LOGIN, REMOTE_REPOS_DIR)

attributes = ["expansions"]
pairs = [
    ("lmcut", "action_lm_cut"),
]
suffix = "-rel" if project.RELATIVE else ""
for algo1, algo2 in pairs:
    for attr in attributes:
        exp.add_report(
            project.ScatterPlotReport(
                relative=project.RELATIVE,
                get_category=None if project.TEX else lambda run1, run2: run1["domain"],
                attributes=[attr],
                filter_algorithm=[algo1, algo2],
                filter=[project.add_evaluations_per_time],
                format="tex" if project.TEX else "png",
            ),
            name=f"{exp.name}-{algo1}-vs-{algo2}-{attr}{suffix}",
        )

project.add_compress_exp_dir_step(exp)

exp.run_steps()
