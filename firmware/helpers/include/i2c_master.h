/**
 * @file i2c_master.h
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Class to use ri pi pico as I²C master
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

#include <array>
#include <stdio.h>
#include <stdint.h>

#include <commands.h>
#include <helper_funcs_var.h>

#include <pico/stdlib.h>
#include <hardware/i2c.h>

namespace Lannooleaf {

  class I2CMaster {

    public:
      I2CMaster(i2c_inst_t* i2c, uint8_t sda_pin, uint scl_pin);
      ~I2CMaster();

    public:
      /** \brief Send data to i2c slave*/
      void send_data(uint8_t slave_address, const uint8_t* src, size_t len);
      /** \brief Receive data from i2c slave*/
      void get_data(uint8_t slave_address, uint8_t* dest, size_t len);

    private:
      i2c_inst_t* i2c;
  };

}
