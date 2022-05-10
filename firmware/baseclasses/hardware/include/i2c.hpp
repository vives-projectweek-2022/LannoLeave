/**
 * @file i2c.hpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief I2C base class
 * @version 0.1
 * @date 2022-05-07
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

#include <stdexcept>

#include <pico/stdlib.h>
#include <hardware/i2c.h>

namespace Lannooleaf::baseclasses {

  /**
   * @brief Base class of Ri Pi Pico I2C
   * 
   * @throws runtime_error when trying to initialize already in use hardware
   */
  class I2C {

    public:
      I2C (i2c_inst_t* i2c, uint sda, uint scl, uint frequency);
      ~I2C();

    protected:
      i2c_inst_t* i2c;
      uint sda;
      uint scl;

  };

}
