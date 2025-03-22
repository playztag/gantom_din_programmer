#pragma once
#include <smooth_ui_toolkit.h>
#include "../factory_test/factory_test.h"

// Forward declarations for the sub-menus
class DMXAddressMenu;
class StaticLookMenu;
class ToggleBootFlashMenu;

using namespace SmoothUIToolKit;
using namespace SmoothUIToolKit::SelectMenu;

class GantomLauncher : public SmoothOptions {
public:
    GantomLauncher(FactoryTest* ft);
    ~GantomLauncher();

    void onReadInput() override;
    void onRender() override;
    void onPress() override;
    void onClick() override;
    void onOpenEnd() override;
    
    void update(uint32_t ms);
    bool isInSubMenu() const { return _inSubMenu; }
    
private:
    FactoryTest* _ft;
    bool _is_pressing = false;
    int  _last_enc_position = 0;
    bool _inSubMenu = false;
    int  _selectedItem = 0;
    
    // Debounce to prevent immediately re-entering a submenu after exit
    unsigned long _lastSubmenuExitTime = 0;
    static const unsigned long SUBMENU_DEBOUNCE_TIME = 500; // ms

    // Sub-menus
    DMXAddressMenu*       _dmxAddressMenu = nullptr;
    StaticLookMenu*       _staticLookMenu = nullptr;
    ToggleBootFlashMenu*  _toggleBootFlashMenu = nullptr;

    // Helpers
    void _openSelectedApp();
}; 