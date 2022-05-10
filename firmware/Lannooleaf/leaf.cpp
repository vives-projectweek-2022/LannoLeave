/**
 * @file leaf.cpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Source file to leaf.h
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

#include <leaf.hpp>

namespace Lannooleaf {

  Leaf::Leaf() 
  : CommandHandler(&slave), slave(UNCONFIGUREDADDRESS ,i2c0, SDA, SCL, BAUDRATE) {
    add_handlers();
  }

  Leaf::~Leaf() { }

  void Leaf::update() {
    update_sel_status();
    
    if (slave.readable()) {
      try {
        handel_command(slave.read_byte());
      } catch (std::runtime_error& e) {
        printf(e.what());
      }
    }
  }

  void Leaf::update_sel_status(void) {
    _sel_pin_status = 0;

    uint8_t i = 0;
    for (select_pins pin : all_select_pins) {
      if (!gpio_is_dir_out((uint)pin) && gpio_get((uint)pin)) _sel_pin_status |= 1 << i;
      i++;
    }
  }

  void Leaf::add_handlers(void) {
    add_handler((uint8_t)slave_commands::set_i2c_address, [&](baseclasses::BufferedCommunicator* com){
      uint8_t new_address = com->read_byte();

      if (slave.address() == UNCONFIGUREDADDRESS && _sel_pin_status) slave.address(new_address);
    });

    add_handler((uint8_t)slave_commands::ping, [&](baseclasses::BufferedCommunicator* com){
      com->write_byte(0xa5);
    }); 

    add_handler((uint8_t)slave_commands::set_sel_pin, [&](baseclasses::BufferedCommunicator* com){
      uint8_t pin   = com->read_byte();
      uint8_t value = com->read_byte();

      if (value) { // Set high
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, true);
      } else {     // Set low
        gpio_put(pin, false);
        gpio_set_dir(pin, GPIO_IN);
      }
    });

    add_handler((uint8_t)slave_commands::get_sel_pin, [&](baseclasses::BufferedCommunicator* com){
      com->write_byte(_sel_pin_status);
    });

    add_handler((uint8_t)slave_commands::is_neighbor, [&](baseclasses::BufferedCommunicator* com){
      uint8_t neighbor = com->read_byte();     
      if (_sel_pin_status) 
        neighbors.push_back(std::make_pair(neighbor, _sel_pin_status)); 
    });

    add_handler((uint8_t)slave_commands::get_neigbor_size, [&](baseclasses::BufferedCommunicator* com){
      com->write_byte(neighbors.size());
    });

    add_handler((uint8_t)slave_commands::get_neighbor_information, [&](baseclasses::BufferedCommunicator* com){
      for (auto [address, side] : neighbors) {
        com->write_byte(address);
        com->write_byte((uint8_t)side);
      }
    });

    add_handler((uint8_t)slave_commands::set_led, [&](baseclasses::BufferedCommunicator* com){
      uint8_t led, red, green, blue;
      led = com->read_byte();
      red = com->read_byte();
      green = com->read_byte();
      blue = com->read_byte();

      ledstrip.setPixelColor(led, PicoLed::RGB(red, green, blue));
      ledstrip.show();
    });

    add_handler((uint8_t)slave_commands::set_all_led, [&](baseclasses::BufferedCommunicator* com){
      uint8_t red, green, blue;
      red = com->read_byte();
      green = com->read_byte();
      blue = com->read_byte();

      ledstrip.fill(PicoLed::RGB(red, green, blue));
      ledstrip.show();
    });

    add_handler((uint8_t)slave_commands::set_led_string, [&](baseclasses::BufferedCommunicator* com){
      std::array<Color, 16> color_string;
      int i = 0;
      for (auto [red, green, blue] : color_string) {
        red = com->read_byte();
        green = com->read_byte();
        blue = com->read_byte();

        ledstrip.setPixelColor(i, PicoLed::RGB(red, green, blue));
        i++;
      }
      ledstrip.show();
    });

    add_handler((uint8_t)slave_commands::discovery_done, [&](baseclasses::BufferedCommunicator* com){
      discover_animation(&ledstrip, {100, 50, 0});
    });

    add_handler((uint8_t)slave_commands::reset, [&](baseclasses::BufferedCommunicator* com){
      // TODO: Implement
    });
  }

}
