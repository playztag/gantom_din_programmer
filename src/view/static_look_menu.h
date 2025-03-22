#pragma once
#include <smooth_ui_toolkit.h>
#include "../factory_test/factory_test.h"

class StaticLookMenu {
public:
    StaticLookMenu(FactoryTest* ft);
    void openMenu();
    void update(unsigned long ms);
    bool isActive() const { return _isActive; }
    void setActive(bool active) { _isActive = active; }

private:
    FactoryTest* _ft;
    bool _waitingRelease = false;
    bool _isPressing = false;
    int  _last_enc_pos = 0;
    int  _channelIndex = 1; // 1..8
    uint8_t _channelValues[8] = {0};
    bool _isActive = false;
    unsigned long _pressStartTime = 0;

    // 0=choose channel #, 1=edit channel value
    int  _state = 0; 

    unsigned long _lastRender = 0;
    const unsigned long _renderInterval = 50;

    void _drawUI();
    void applyStaticLook();
    void _exitMenu();

    // For checking button double click
    unsigned long _lastClickTime = 0;
}; 