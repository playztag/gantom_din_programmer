#pragma once
#include <smooth_ui_toolkit.h>
#include "../factory_test/factory_test.h"

using namespace SmoothUIToolKit;
using namespace SmoothUIToolKit::SelectMenu;

class DMXAddressMenu {
public:
    DMXAddressMenu(FactoryTest* ft);
    void openMenu();
    void update(uint32_t ms);
    bool isActive() const { return _isActive; }
    void setActive(bool active) { _isActive = active; }

private:
    FactoryTest* _ft;
    int _dmxAddress = 1; // default
    bool _waitingRelease = false;
    bool _isPressing = false;
    int  _last_enc_pos = 0;
    bool _isActive = false;
    bool _editMode = false;
    unsigned long _pressStartTime = 0;
    
    void _drawUI();
    void _exitMenu();
    void applyAddress(int address);
}; 