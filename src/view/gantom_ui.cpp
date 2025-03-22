// src/view/gantom_ui.cpp
#include "gantom_ui.h"
#include <Arduino.h>

static FactoryTest* _ft = nullptr;
static GantomLauncher* _launcher = nullptr;
static GantomBootSplash* _bootSplash = nullptr;

void gantom_ui_create(FactoryTest* ft)
{
    _ft = ft;
    
    // Create boot splash and show it
    _bootSplash = new GantomBootSplash(ft);
    _bootSplash->show(2000); // Show for 2 seconds or until interaction
    
    // Create the main launcher menu
    _launcher = new GantomLauncher(ft);
}

void gantom_ui_update()
{
    // Update the main launcher menu and sub-menus
    if (_launcher) {
        _launcher->update(millis());
    }
} 