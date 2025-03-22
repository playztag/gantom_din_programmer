#include "static_look_menu.h"
#include <Arduino.h>

StaticLookMenu::StaticLookMenu(FactoryTest* ft)
  : _ft(ft)
{
    // Initialize channels
    for (int i = 0; i < 8; i++) {
        _channelValues[i] = 0; // default value
    }
}

void StaticLookMenu::openMenu()
{
    _channelIndex = 1;
    _state = 0; // selecting channel mode
    _ft->_enc.setPosition(0);
    _last_enc_pos = 0;
    _isActive = true;
    
    // Initial display
    _drawUI();
}

void StaticLookMenu::update(unsigned long ms)
{
    if (!_isActive) return;
    
    // Get current encoder position
    int newPos = _ft->_enc.getPosition();
    
    // Handle encoder rotation
    if (newPos != _last_enc_pos) {
        // Provide tactile feedback via tone
        _ft->_tone(2000 + (newPos - _last_enc_pos) * 100, 10);
        
        if (_state == 0) {
            // Channel selection mode
            _channelIndex += (newPos - _last_enc_pos);
            
            // Clamp to valid range (1-8)
            if (_channelIndex < 1) _channelIndex = 1;
            if (_channelIndex > 8) _channelIndex = 8;
        } else {
            // Value adjustment mode
            int currentValue = _channelValues[_channelIndex - 1];
            
            // Apply faster value changing for smoother user experience
            int change = (newPos - _last_enc_pos) * 5;
            currentValue += change;
            
            // Clamp to valid range (0-255)
            if (currentValue < 0) currentValue = 0;
            if (currentValue > 255) currentValue = 255;
            
            _channelValues[_channelIndex - 1] = currentValue;
        }
        
        _last_enc_pos = newPos;
    }
    
    // Handle button press using the factory test's button
    if (!_ft->_btn_pwr.read()) { // Button is pressed when LOW
        if (!_isPressing) {
            _isPressing = true;
            _pressStartTime = millis();
        }
    } else if (_isPressing) {
        // Button released
        unsigned long pressDuration = millis() - _pressStartTime;
        _isPressing = false;
        
        if (pressDuration > 800) {
            // Long press - exit menu
            _ft->_tone(1200, 100);
            _isActive = false;
        } else {
            // Short press - toggle state
            _ft->_tone(2500, 50);
            
            // Check for double click based on timing
            if (millis() - _lastClickTime < 300) {
                // Double-click detected
                _ft->_tone(2000, 50);
                applyStaticLook();
                _isActive = false; // Exit after saving
            } else {
                // Toggle between channel selection and value adjustment
                _state = !_state;
                _lastClickTime = millis();
            }
        }
    }
    
    // Draw UI
    _drawUI();
}

void StaticLookMenu::_drawUI()
{
    _ft->_canvas->fillScreen(0x87C38F); // Use brightness color

    // Header
    _ft->_canvas->fillRect(0, 0, _ft->_canvas->width(), 25, 0x07430F);
    _ft->_canvas->setTextColor(0x87C38F);
    _ft->_canvas->setFont(&fonts::Font0);
    _ft->_canvas->setTextSize(1);
    _ft->_canvas->drawCenterString("Static Look", _ft->_canvas->width() / 2, 9);

    // Draw all 8 channel bars
    const int barX = 50;      // X position for the start of the bar
    const int barWidth = 150; // Width of each bar
    const int barHeight = 10; // Height of each bar
    const int startY = 35;    // Y position for the first bar
    const int spacing = 12;   // Spacing between bars
    
    for (int i = 0; i < 8; i++) {
        int channelY = startY + (i * spacing);
        int value = _channelValues[i];
        int filledWidth = (value * barWidth) / 255;
        
        // Draw channel label
        _ft->_canvas->setFont(&fonts::Font0);
        _ft->_canvas->setTextSize(1);
        _ft->_canvas->setTextColor(0x07430F);
        char label[4];
        snprintf(label, sizeof(label), "CH%d", i+1);
        _ft->_canvas->drawString(label, 5, channelY + 2);
        
        // Draw bar outline
        _ft->_canvas->drawRect(barX, channelY, barWidth, barHeight, 0x07430F);
        
        // Draw filled portion of the bar
        if (filledWidth > 0) {
            _ft->_canvas->fillRect(barX, channelY, filledWidth, barHeight, 0x07430F);
        }
        
        // Draw percent value
        char valueBuf[10];
        snprintf(valueBuf, sizeof(valueBuf), "%d%%", (value * 100) / 255);
        _ft->_canvas->drawRightString(valueBuf, _ft->_canvas->width() - 5, channelY + 2);
        
        // Highlight the selected channel
        if (i == (_channelIndex - 1)) {
            if (_state == 0) {
                // Channel selection mode - draw an arrow
                _ft->_canvas->fillTriangle(
                    barX - 10, channelY + barHeight/2,
                    barX - 5, channelY,
                    barX - 5, channelY + barHeight, 
                    0x07430F);
            } else {
                // Value adjustment mode - add a cursor
                _ft->_canvas->drawRect(barX-2, channelY-2, barWidth+4, barHeight+4, 0xFFFF);
                _ft->_canvas->fillRect(barX + filledWidth - 2, channelY-4, 4, barHeight+8, 0xFFE0); // Yellow cursor
            }
        }
    }
    
    // Draw instructions at the bottom
    _ft->_canvas->setTextColor(0x07430F);
    
    if (_state == 0) {
        _ft->_canvas->drawCenterString("Turn: Select Channel", _ft->_canvas->width() / 2, 135 - 12);
        _ft->_canvas->drawCenterString("Press: Edit Value", _ft->_canvas->width() / 2, 135);
    } else {
        _ft->_canvas->drawCenterString("Turn: Adjust Value", _ft->_canvas->width() / 2, 135 - 12);
        _ft->_canvas->drawCenterString("Press: Back to Selection", _ft->_canvas->width() / 2, 135);
    }
    
    // Show the current value of the selected channel
    if (_state == 1) {
        char valueBuf[20];
        int val = _channelValues[_channelIndex - 1];
        snprintf(valueBuf, sizeof(valueBuf), "Channel %d: %d (%d%%)", 
                _channelIndex, val, (val * 100) / 255);
        
        _ft->_canvas->fillRect(0, 135 - 24, _ft->_canvas->width(), 12, 0x07430F);
        _ft->_canvas->setTextColor(0x87C38F);
        _ft->_canvas->drawCenterString(valueBuf, _ft->_canvas->width() / 2, 135 - 24);
    }
    
    // Draw saved indicator
    _ft->_canvas->drawString("Long Press: Exit", 5, _ft->_canvas->height() - 12);
    _ft->_canvas->drawRightString("Dbl Click: Save", _ft->_canvas->width() - 5, _ft->_canvas->height() - 12);
    
    _ft->_canvas_update();
}

void StaticLookMenu::applyStaticLook()
{
    // Display saving message
    _ft->_canvas->fillScreen(0x87C38F);
    _ft->_canvas->fillRect(0, 0, _ft->_canvas->width(), 25, 0x07430F);
    _ft->_canvas->setTextColor(0x87C38F);
    _ft->_canvas->setFont(&fonts::Font0);
    _ft->_canvas->setTextSize(1);
    _ft->_canvas->drawCenterString("Static Look", _ft->_canvas->width() / 2, 9);
    
    _ft->_canvas->setTextColor(0x07430F);
    _ft->_canvas->drawCenterString("Saving Channel Values...", _ft->_canvas->width() / 2, 50);
    _ft->_canvas_update();
    
    // Apply changes - set the DMX values
    // Note: In a real implementation, you'd use _ft's methods to set DMX values
    for (int i = 0; i < 8; i++) {
        Serial.printf("CH %d = %d\n", i + 1, _channelValues[i]);
    }
    
    // Play feedback tone
    _ft->_tone(2000, 100);
    delay(50);
    _ft->_tone(2500, 100);
    delay(50);
    _ft->_tone(3000, 100);
    
    // Display confirmation
    _ft->_canvas->setTextColor(0x07430F);
    _ft->_canvas->drawCenterString("Values Saved!", _ft->_canvas->width() / 2, 80);
    _ft->_canvas_update();
    
    // Wait a moment to show the confirmation
    delay(1000);
}

void StaticLookMenu::_exitMenu()
{
    _isActive = false;
} 