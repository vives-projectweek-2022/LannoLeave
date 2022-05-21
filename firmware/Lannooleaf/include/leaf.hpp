/**
 * @file leaf.h
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Class to receive/handel commands from controller / Set colors of leds
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
#include <stdio.h>
#include <stdint.h>
#include <functional>

#include <PicoLed.hpp>

#include <commands.hpp>
#include <i2c_slave.hpp>
#include <helper_funcs_var.hpp>

#include <updatable.hpp>
#include <command_handler.hpp>
#include <i2c_slave.hpp>

#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>

namespace Lannooleaf {

  class Leaf : public CommandHandler, public interfaces::IUpdatable  {

    public:
      Leaf();
      ~Leaf();

    public:
      virtual void update(void);

    public:
      PicoLed::PicoLedController ledstrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);

    private:
      void update_sel_status(void);
      void add_handlers(void);

    private:
      I2CSlave slave;

    private:
      uint8_t _sel_pin_status;
      std::vector<std::pair<uint8_t, uint8_t>> neighbors;
 
  };

}
