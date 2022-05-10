/**
 * @file controller.h
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Class to run device/topology discovery and pass commands to connected leafs / Set led colors
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

#pragma once

#include <map>
#include <vector>
#include <stdint.h>
#include <functional>

#include <PicoLed.hpp>

#include <graph.hpp>
#include <commands.hpp>
#include <spi_slave.hpp>
#include <i2c_master.hpp>
#include <packet_handler.hpp>


#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>

namespace Lannooleaf {

  class Controller : public PacketHandler {

    public:
      Controller(i2c_inst_t * i2c_leaf_inst, uint sda_pin, uint scl_pin);
      ~Controller();

    public:
      virtual void __not_in_flash_func(update)(void);

    private:
      void device_discovery(void);
      void topology_discovery(void);
      void add_handlers(void);

      void set_select_pin(uint pin, bool value, uint8_t address);

      uint8_t get_next_available_address(void);


    private:
      std::vector<uint8_t> visited;

    public:
      Graph graph;
      SPISlave slave;
      I2CMaster leaf_master;

    public:
      PicoLed::PicoLedController ledstrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);
  
  };
  
}
