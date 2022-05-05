/**
 * @file i2c_master.cpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Sourec file to i2c_master.h
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

#include <i2c_master.h>

namespace Lannooleaf {

  I2CMaster::I2CMaster(i2c_inst_t* i2c, uint8_t sda_pin, uint scl_pin) { 
    this->i2c = i2c; 

    gpio_init(sda_pin);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);

    gpio_init(scl_pin);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(scl_pin);

    i2c_init(i2c, BAUDRATE);
  };

  I2CMaster::~I2CMaster() { }

  void I2CMaster::send_data(uint8_t slave_address, const uint8_t* src, size_t len) {
    i2c_write_blocking(i2c, slave_address, src, len, false);
  }


  void I2CMaster::get_data(uint8_t slave_address, uint8_t* dest, size_t len) {
    i2c_read_blocking(i2c, slave_address, dest, len, false);
  }

}
