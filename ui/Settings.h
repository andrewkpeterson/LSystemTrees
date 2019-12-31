#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

struct Settings {
    // Loads settings from disk, or fills in default values if no saved settings exist.
    void loadSettingsOrDefaults();

    bool use_shadows;
    bool use_ambient_occlusion;
    bool display_ssao_map;
    float cameraNear;
    float cameraFar;
    float cameraFov;
    float power;
    float radius;
    int kernel_size;


};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
