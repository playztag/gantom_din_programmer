#pragma once
#include "../factory_test/factory_test.h"

class GantomBootSplash {
public:
    GantomBootSplash(FactoryTest* ft);
    
    // Shows the splash screen and waits for button/knob or timeout
    void show(uint32_t timeout_ms = 2000);
    
private:
    FactoryTest* _ft;
    
    void _drawSplash();
}; 