// src/view/toggle_boot_flash_menu.h
#pragma once
#include <smooth_ui_toolkit.h>
#include "../factory_test/factory_test.h"

class ToggleBootFlashMenu {
public:
    ToggleBootFlashMenu(FactoryTest* ft);
    void openMenu();
    void update(uint32_t ms);
    bool isActive() const { return _isActive; }
    void setActive(bool active) { _isActive = active; }

private:
    FactoryTest* _ft;
    bool _waitingRelease = false;
    bool _isPressing = false;
    int  _last_enc_pos = 0;
    bool _isActive = false;
    unsigned long _pressStartTime = 0;

    bool _flashOnBoot = false; // current setting

    unsigned long _lastRender = 0;
    unsigned long _renderInterval = 50;

    void _drawUI();
    void _saveSetting();
    void _exitMenu();
}; 