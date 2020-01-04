#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "glm.hpp"

enum TreeType {
    TT_MONOPODIAL, TT_SYMPODIAL, TT_TERNARY
};

enum LeafType {
    LT_LONG, LT_MAPLE
};

enum LeafPlacementType {
     LPT_TERMINAL, LPT_RANDOM
};

struct Settings {
    // Loads settings from disk, or fills in default values if no saved settings exist.
    void loadSettingsOrDefaults();

    float cameraNear;
    float cameraFar;
    float cameraFov;

    float param_a;
    float param_b;
    float param_c;
    float param_d;
    float param_e;

    float tropism_constant;
    glm::vec3 tropism_vector;

    bool use_leaves;
    bool use_flowers;
    glm::vec3 leaf_size;

    float branch_base_radius;
    float branch_top_radius;

    TreeType treetype;

    LeafType leaftype;

    LeafPlacementType leaf_placementype;

    float leaf_placement_thickness;

    int iterations;

    bool changed_treetype;

};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
