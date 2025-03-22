/**
 * @file main.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-06-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "factory_test/factory_test.h"
#include "view/gantom_ui.h"

static FactoryTest ft;

void setup()
{
    ft.init();
    gantom_ui_create(&ft);
}

void loop() { gantom_ui_update(); }
