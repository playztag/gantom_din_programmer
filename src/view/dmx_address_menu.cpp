#include "dmx_address_menu.h"
#include <Arduino.h>

DMXAddressMenu::DMXAddressMenu(FactoryTest* ft)
  : _ft(ft)
{
    // Initialize other member variables
    _dmxAddress = 1;
    _isActive = false;
    _waitingRelease = false;
    _isPressing = false;
    _last_enc_pos = 0;
}

void DMXAddressMenu::openMenu()
{
    // Reset address if needed
    _dmxAddress = 1;
    // Reset encoder position
    _ft->_enc.setPosition(0);
    _last_enc_pos = 0;
    _isActive = true;
    _waitingRelease = false;
    _isPressing = false;
    _pressStartTime = 0;
    
    // Initial display
    _drawUI();
}

void DMXAddressMenu::update(uint32_t ms)
{
    if (!_isActive) return;
    
    // Check for input at regular intervals
    static uint32_t lastInputCheck = 0;
    if (ms - lastInputCheck > 50) {
        lastInputCheck = ms;
        
        // Check encoder
        _ft->_check_encoder(false);
        int newPos = _ft->_enc.getPosition();
        if (newPos != _last_enc_pos) {
            // Adjust address
            if (newPos < _last_enc_pos) {
                _dmxAddress--;
            } else {
                _dmxAddress++;
            }
            // Clamp
            if (_dmxAddress < 1) _dmxAddress = 1;
            if (_dmxAddress > 512) _dmxAddress = 512;  // or 506 for Gantom7
            
            _last_enc_pos = newPos;
            _ft->_tone(3500, 20); // Feedback tone
        }

        // Check power button
        if (!_ft->_btn_pwr.read()) {
            if (!_waitingRelease) {
                _ft->_tone(2500, 50);
                _waitingRelease = true;
                _isPressing = true;
                _pressStartTime = ms;
            } else if (_isPressing) {
                // Check for long press
                if (ms - _pressStartTime > 800) { // 800ms threshold for long press
                    // Long press detected - exit menu
                    _ft->_tone(3000, 100);
                    _exitMenu();
                    return;
                }
            }
        } else {
            if (_isPressing) {
                _isPressing = false;
                // Short press - apply address but stay in menu
                if (ms - _pressStartTime < 800) {
                    applyAddress(_dmxAddress);
                    // Don't exit menu, stay here to allow more adjustments
                }
            }
            _waitingRelease = false;
        }
    }
    
    // Update display
    _drawUI();
}

void DMXAddressMenu::_drawUI()
{
    _ft->_canvas->fillScreen(0x651F);  // Dark blue background

    // Header
    _ft->_canvas->fillRect(0, 0, _ft->_canvas->width(), 25, 0x001F);
    _ft->_canvas->setTextColor(0xFFFF);
    _ft->_canvas->setFont(&fonts::Font0);
    _ft->_canvas->setTextSize(1);
    _ft->_canvas->drawCenterString("DMX Address", _ft->_canvas->width() / 2, 9);

    // Current DMX address display
    _ft->_canvas->setTextColor(0xFFFF);
    _ft->_canvas->setTextSize(3);
    
    char addrBuf[10];
    snprintf(addrBuf, sizeof(addrBuf), "%d", _dmxAddress);
    _ft->_canvas->drawCenterString(addrBuf, _ft->_canvas->width() / 2, 50);

    // Instructions
    _ft->_canvas->setTextSize(1);
    
    _ft->_canvas->drawCenterString("Turn: Adjust Address", _ft->_canvas->width() / 2, 100);
    _ft->_canvas->drawCenterString("Press: Apply Address", _ft->_canvas->width() / 2, 115);
    _ft->_canvas->drawCenterString("Long Press: Exit", _ft->_canvas->width() / 2, 130);

    _ft->_canvas_update();
}

void DMXAddressMenu::applyAddress(int address)
{
    // Here is where you would actually talk to the fixture
    // e.g. send DMX set address command
    Serial.printf("Applying DMX address: %d\n", address);
    
    // Show success message
    _ft->_canvas->fillScreen(0xB8DBD9);
    _ft->_canvas->setTextColor(0x385B59);
    _ft->_canvas->setFont(&fonts::efontCN_16);
    _ft->_canvas->drawCenterString("Address Set!", _ft->_canvas->width() / 2, 40);
    
    char buf[32];
    snprintf(buf, sizeof(buf), "DMX: %03d", address);
    _ft->_canvas->drawCenterString(buf, _ft->_canvas->width() / 2, 70);
    
    _ft->_canvas_update();
    
    // Success tone
    _ft->_tone(4000, 100);
    
    delay(1000); // Short delay to show the success message
}

void DMXAddressMenu::_exitMenu()
{
    _isActive = false;
} 