#include "Settings.h"
#include <QFile>
#include <QSettings>

Settings settings;

void Settings::loadSettingsOrDefaults() {
    // Set the default values below
    settings.treetype = TT_MONOPODIAL;
    settings.leaftype = LT_LONG;
    settings.leaf_placementype = LPT_TERMINAL;
    settings.param_a = .9;
    settings.param_b = .6;
    settings.param_c = 45;
    settings.param_d = 45;
    settings.param_e = 137.5;

    settings.tropism_constant = 0;
    settings.tropism_vector = glm::vec3(0,-1,0);

    settings.use_flowers = true;
    settings.use_leaves = true;
    settings.leaf_size = .1;
    settings.flower_size = .1;

    settings.branch_base_radius = .5;
    settings.branch_top_radius = .5;

    settings.leaf_placement_thickness = 0;
    settings.maxleaves_per_branch = 2;
    settings.max_displacement_angle = .3;

    settings.iterations = 6;
    settings.prob_of_flower_vs_leaf = 0;
    settings.prob_of_leaf_flower = .5;

    settings.changed_treetype = false;

}
