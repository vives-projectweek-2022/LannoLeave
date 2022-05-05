/**
 * @file helper_funcs_var.cpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Sourec file to helper_funcs_var.h
 * @version 0.1
 * @date 2022-05-04
 * 
 * @copyright Copyright 2022 Joey De Smet
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *         http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "include/helper_funcs_var.h"

namespace Lannooleaf {

  void set_alive_led(void) {
    const uint led_pin = 25;
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin, true);
  }

  void discover_animation(PicoLed::PicoLedController* ledstrip, Color color) {
    for (int i = 0; i < LED_LENGTH; i++) {
      ledstrip->setPixelColor(i, PicoLed::RGB(color.red, color.green, color.blue));
      if (i > 0) ledstrip->setPixelColor(i-1, PicoLed::RGB(0, 0, 0));
      ledstrip->show();
      sleep_ms(250);
    }

    for (int i = LED_LENGTH; i >= 0; i--) {
      ledstrip->setPixelColor(i, PicoLed::RGB(color.red, color.green, color.blue));
      ledstrip->show();
      sleep_ms(25);
    }
  }

  bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78 || addr == 0x08;
  }

  side sel_pin_to_side(select_pins pin) {
    switch (pin) {
    case select_pins::A:
      return side::a;

    case select_pins::B:
      return side::b;

    case select_pins::C:
      return side::c;

    case select_pins::D:
      return side::d;

    case select_pins::E:
      return side::e;

    case select_pins::F:
      return side::f;

    default:
      return side::invalid;
    }
  }

  side sel_pin_state_to_side(uint8_t state) {
    switch (state) {
    case 0x01:
      return side::a;

    case 0x02:
      return side::b;

    case 0x04:
      return side::c;

    case 0x08:
      return side::d;

    case 0x10:
      return side::e;

    case 0x20:
      return side::f;

    default:
      return side::invalid;
    }
  }

  char side_to_char(side side) {
    switch (side){
    case side::a:
      return 'a';

    case side::b:
      return 'b';
    
    case side::c:
      return 'c';

    case side::d:
      return 'd';

    case side::e:
      return 'e';

    case side::f:
      return 'f';

    default:
      return 'X';
    }
  }

}
