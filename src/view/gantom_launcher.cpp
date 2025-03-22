#include "gantom_launcher.h"
#include "dmx_address_menu.h"
#include "static_look_menu.h"
#include "toggle_boot_flash_menu.h"
#include <Arduino.h>

GantomLauncher::GantomLauncher(FactoryTest* ft)
  : _ft(ft)
{
    _inSubMenu = false;
    _selectedItem = 0; // Initialize selected item index
    
    // Menu items with good spacing and size for the screen
    addOption(); // Add DMX Address option
    setLastKeyframe({15, 30, 150, 35});
    
    addOption(); // Add Static Look option
    setLastKeyframe({15, 70, 150, 35});
    
    addOption(); // Add Flash Boot option
    setLastKeyframe({15, 110, 150, 35});
    
    addOption(); // Add Power Off option
    setLastKeyframe({15, 150, 150, 35});

    // Config animations
    setConfig().renderInterval = 20;
    setConfig().readInputInterval = 50;
    setPositionDuration(600);
    setPositionTransitionPath(EasingPath::easeOutBack);
    setShapeDuration(400);

    // Create sub-menu objects
    _dmxAddressMenu = new DMXAddressMenu(ft);
    _staticLookMenu = new StaticLookMenu(ft);
    _toggleBootFlashMenu = new ToggleBootFlashMenu(ft);
}

GantomLauncher::~GantomLauncher()
{
    delete _dmxAddressMenu;
    delete _staticLookMenu;
    delete _toggleBootFlashMenu;
}

void GantomLauncher::onReadInput()
{
    if (isOpening() || _inSubMenu) return;

    // Check encoder
    _ft->_check_encoder(true);
    
    // If we have a new position, see which direction
    int newPos = _ft->_enc.getPosition();
    if (newPos != _last_enc_position) {
        if (newPos < _last_enc_position) {
            goLast();
            _selectedItem = (_selectedItem - 1 + 4) % 4; // Update selected item
        } else {
            goNext();
            _selectedItem = (_selectedItem + 1) % 4; // Update selected item
        }
        _last_enc_position = newPos;
    }

    static unsigned long lastButtonChangeTime = 0;
    static bool wasButtonPressed = false;
    unsigned long currentTime = millis();
    bool buttonState = !_ft->_btn_pwr.read(); // true when pressed
    
    // Only process button state changes after debounce period
    if (buttonState != wasButtonPressed && (currentTime - lastButtonChangeTime > 50)) {
        lastButtonChangeTime = currentTime;
        wasButtonPressed = buttonState;
        
        if (buttonState) {
            // Button was just pressed - handle the press state
            _ft->_tone(2500, 50); // beep
            _is_pressing = true; // Track the press state
            press({0, 12, 240, 52}); // Trigger the press animation
        } else {
            // Button was just released - only handle release state
            if (_is_pressing) {
                _is_pressing = false;
                release(); // Trigger the release animation
                
                // Only trigger the onClick event once, on button release
                // This is intentionally added here and not in a separate handler
                if (!isOpening()) { // Avoid double-triggering when already opening
                    onClick(); // This is where we explicitly call onClick to open the menu
                }
            }
        }
    }
}

void GantomLauncher::onRender()
{
    // Draw background
    _ft->_canvas->fillScreen(0x0861);  // Dark background
    
    // Draw header - make slightly taller and ensure it's above everything
    _ft->_canvas->fillRect(0, 0, _ft->_canvas->width(), 28, 0x0000);
    _ft->_canvas->setTextColor(0xFFFF);
    _ft->_canvas->setFont(&fonts::Font0);
    _ft->_canvas->setTextSize(1);
    _ft->_canvas->drawCenterString("GANTOM DMX TOOL", _ft->_canvas->width() / 2, 10);
    
    // Menu items
    const char* menuLabels[4] = {
        "DMX Address",
        "Static Look",
        "Flash Boot",
        "Power Off"
    };
    
    uint16_t menuColors[4] = {
        0x651F,  // Blue for DMX Address
        0x87C3,  // Green for Static Look
        0xCE9E,  // Orange for Flash Boot
        0xE861   // Red for Power Off
    };
    
    // Draw each menu item
    for (int i = 0; i < 4; i++) {
        // Get the current frame for this option
        int matchingIndex;
        getMatchingOptionIndex(i, matchingIndex);
        auto frame = getOptionCurrentFrame(matchingIndex);
        
        int x = frame.x;
        int y = frame.y;
        int w = frame.w;
        int h = frame.h;
        
        // Fill background 
        _ft->_canvas->fillRoundRect(x, y, w, h, 5, menuColors[i]);
        
        // If this is the selected item, draw a highlight
        if (i == _selectedItem) {
            // Draw selection indicator (arrow)
            _ft->_canvas->fillTriangle(
                x - 10, y + h/2,     // Point
                x - 5, y + h/2 - 5,  // Top
                x - 5, y + h/2 + 5   // Bottom
            , 0xFFFF);
            
            // Draw border around selected option
            _ft->_canvas->drawRoundRect(x-1, y-1, w+2, h+2, 5, 0xFFFF);
            _ft->_canvas->drawRoundRect(x-2, y-2, w+4, h+4, 7, 0xFFFF);
        }
        
        // Draw text - improve vertical centering
        _ft->_canvas->setTextColor(0xFFFF);
        _ft->_canvas->setFont(&fonts::Font0);
        _ft->_canvas->setTextSize(1);
        
        // Calculate vertical center of the button for better text positioning
        int textY = y + (h / 2) - 4; // Adjust for better vertical centering
        _ft->_canvas->drawCenterString(menuLabels[i], x + w/2, textY);
    }
    
    _ft->_canvas_update();
}

void GantomLauncher::onPress()
{
    // Animate the "press" effect
    setDuration(200);
    setTransitionPath(EasingPath::easeOutQuad);
}

void GantomLauncher::onClick()
{
    // Animate "open" effect
    setDuration(300);
    setTransitionPath(EasingPath::easeOutQuad);

    open({-20, -20, 280, 175});
}

void GantomLauncher::onOpenEnd()
{
    // Here we do the logic
    _openSelectedApp();

    // Reset animations
    setPositionDuration(600);
    setPositionTransitionPath(EasingPath::easeOutBack);
    setShapeDuration(400);

    // Close the main menu
    close();
    
    // Reset encoder if you want
    _ft->_enc.setPosition(_last_enc_position);
}

void GantomLauncher::_openSelectedApp()
{
    unsigned long currentTime = millis();
    
    // Check if we recently exited a submenu - if so, don't immediately enter another
    if (currentTime - _lastSubmenuExitTime < SUBMENU_DEBOUNCE_TIME) {
        return; // Skip opening a new submenu, too soon after exiting
    }
    
    switch (_selectedItem) {
    case 0:
        // Go to DMX Address sub-menu
        _dmxAddressMenu->openMenu();
        _dmxAddressMenu->setActive(true);
        _inSubMenu = true;
        break;
    case 1:
        // Go to Set Static Look
        _staticLookMenu->openMenu();
        _staticLookMenu->setActive(true);
        _inSubMenu = true;
        break;
    case 2:
        // Toggle flash boot
        _toggleBootFlashMenu->openMenu();
        _toggleBootFlashMenu->setActive(true);
        _inSubMenu = true;
        break;
    case 3:
        // Power off
        _ft->_power_off();
        break;
    }
}

void GantomLauncher::update(uint32_t ms)
{
    // Only update main menu if no submenu is active
    if (!_inSubMenu) {
        SmoothOptions::update(ms);
    } else {
        // Update active submenu
        if (_dmxAddressMenu->isActive()) {
            _dmxAddressMenu->update(ms);
            if (!_dmxAddressMenu->isActive()) {
                _inSubMenu = false;
                _lastSubmenuExitTime = ms;  // Record when menu was exited
            }
        } else if (_staticLookMenu->isActive()) {
            _staticLookMenu->update(ms);
            if (!_staticLookMenu->isActive()) {
                _inSubMenu = false;
                _lastSubmenuExitTime = ms;  // Record when menu was exited
            }
        } else if (_toggleBootFlashMenu->isActive()) {
            _toggleBootFlashMenu->update(ms);
            if (!_toggleBootFlashMenu->isActive()) {
                _inSubMenu = false;
                _lastSubmenuExitTime = ms;  // Record when menu was exited
            }
        }
    }
} 