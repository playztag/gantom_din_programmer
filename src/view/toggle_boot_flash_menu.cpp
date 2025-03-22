// src/view/toggle_boot_flash_menu.cpp
#include "toggle_boot_flash_menu.h"
#include <Arduino.h>

ToggleBootFlashMenu::ToggleBootFlashMenu(FactoryTest* ft)
  : _ft(ft)
{
    // Nothing special to initialize
}

void ToggleBootFlashMenu::openMenu()
{
    // Read current setting from fixture or use default
    _flashOnBoot = false; 
    
    // Reset encoder for clean state
    _ft->_enc.setPosition(0);
    _last_enc_pos = 0;
    _isActive = true;
    
    // Initial display
    _drawUI();
}

void ToggleBootFlashMenu::update(uint32_t ms)
{
    if (!_isActive) return;
    
    static uint32_t lastInputCheck = 0;
    if (ms - lastInputCheck > 50) {
        lastInputCheck = ms;
        
        // Check encoder
        _ft->_check_encoder(false);
        int newPos = _ft->_enc.getPosition();
        if (newPos != _last_enc_pos) {
            // Toggle the setting on any encoder movement
            _flashOnBoot = !_flashOnBoot;
            _ft->_tone(3500, 20); // Feedback tone
            _last_enc_pos = newPos;
        }

        // Check power button
        if (!_ft->_btn_pwr.read()) {
            if (!_waitingRelease) {
                _ft->_tone(2500, 50);
                _waitingRelease = true;
                _isPressing = true;
                _pressStartTime = ms;
            } else if (_isPressing) {
                // Check for long press (800ms)
                if (ms - _pressStartTime > 800) {
                    // Long press detected - exit menu
                    _ft->_tone(3000, 100);
                    _exitMenu();
                    return;
                }
            }
        } else {
            if (_isPressing) {
                _isPressing = false;
                // Short press - apply changes and exit
                if (ms - _pressStartTime < 800) {
                    _saveSetting();
                    _exitMenu();
                    return;
                }
            }
            _waitingRelease = false;
        }
    }

    // Update display at regular intervals
    if (ms - _lastRender > _renderInterval) {
        _lastRender = ms;
        _drawUI();
    }
}

void ToggleBootFlashMenu::_drawUI()
{
    _ft->_canvas->fillScreen(0xCE9E);  // Use orange-ish tint for this menu

    // Header
    _ft->_canvas->fillRect(0, 0, _ft->_canvas->width(), 25, 0x8200);
    _ft->_canvas->setTextColor(0xFFFF);
    _ft->_canvas->setFont(&fonts::Font0);
    _ft->_canvas->setTextSize(1);
    _ft->_canvas->drawCenterString("Flash Boot", _ft->_canvas->width() / 2, 9);

    // Current status
    _ft->_canvas->setTextColor(0x0000);
    _ft->_canvas->setTextSize(2);
    _ft->_canvas->drawCenterString(_flashOnBoot ? "ENABLED" : "DISABLED", _ft->_canvas->width() / 2, 60);

    // Instructions
    _ft->_canvas->setTextSize(1);
    _ft->_canvas->drawCenterString("Turn: Toggle Setting", _ft->_canvas->width() / 2, 100);
    _ft->_canvas->drawCenterString("Press: Save Changes", _ft->_canvas->width() / 2, 115);
    _ft->_canvas->drawCenterString("Long Press: Exit", _ft->_canvas->width() / 2, 130);

    _ft->_canvas_update();
}

void ToggleBootFlashMenu::_saveSetting()
{
    // Here is where you would send the command to the fixture
    if (_flashOnBoot) {
        // Apply code
        Serial.println("Power cycling - fixture should flash on bootup.");
    } else {
        Serial.println("Power cycling - fixture should not flash on bootup.");
    }
    
    // Show success message
    _ft->_canvas->fillScreen(0xF6A4A4);
    _ft->_canvas->setTextColor(0x762424);
    _ft->_canvas->setFont(&fonts::efontCN_16);
    _ft->_canvas->drawCenterString("Setting Saved!", _ft->_canvas->width() / 2, 60);
    _ft->_canvas_update();
    
    // Success tone
    _ft->_tone(4000, 100);
    
    delay(1000); // Show success message briefly
}

void ToggleBootFlashMenu::_exitMenu()
{
    _isActive = false;
} 