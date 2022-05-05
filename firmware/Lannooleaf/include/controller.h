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

#include <graph.h>
#include <commands.h>
#include <spi_slave.h>
#include <i2c_master.h>
#include <command_handler.h>

#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>

namespace Lannooleaf {

  class Controller {

    public:
      Controller(i2c_inst_t * i2c_leaf_inst, uint sda_pin, uint scl_pin);
      ~Controller();

    public:
      /**
       * @brief Start the device discovery algorithm
       */
      void device_discovery(void);

      /**
       * @brief Starts topology discovery algorithm
       */
      void topology_discovery(void);

      /**
       * @brief Adds handles to commandhandler
       * 
       * @param handler 
       * Pointer to the instantiated CommandHandler class
       */
      void add_controller_handlers(CommandHandler* handler);

    private:  
      /**
       * @brief Get the next available i2c address
       * ined reference to `Lannooleaf::discover_animation(PicoLed::PicoLedController*, Lannooleaf::Color)'lable
       */
      uint8_t get_next_available_address(void);

    private:
      /**
       * @brief Send i2c message to set a select pin
       * 
       * @param pin 
       * Select pin to set
       * @param value 
       * true for high, false for low
       * @param address 
       * i2c address to send message to
       */
      void set_select_pin(uint pin, bool value, uint8_t address);

    private:
      std::vector<uint8_t> visited;

    public:
      Graph graph;
      I2CMaster leaf_master;

    public:
      PicoLed::PicoLedController ledstrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);
  
  };
  
}
