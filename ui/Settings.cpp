#include "Settings.h"
#include <QFile>
#include <QSettings>

Settings settings;

void Settings::loadSettingsOrDefaults() {
    // Set the default values below
    settings.treetype = TT_MONOPODIAL;
    settings.leaftype = LT_LONG;
    settings.param_a = .9;
    settings.param_b = .6;
    settings.param_c = 45;
    settings.param_d = 45;
    settings.param_e = 137.5;

    settings.tropism_constant = 0;
    settings.tropism_vector = glm::vec3(0,-1,0);

    settings.use_flowers = false;
    settings.use_leaves = false;
    settings.leaf_size = glm::vec3(.01, .5, .05);

    settings.branch_base_radius = .5;
    settings.branch_top_radius = .5;
}
