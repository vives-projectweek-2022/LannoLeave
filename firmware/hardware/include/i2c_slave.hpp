/**
 * @file i2c_slave.hpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief I2C Slave class to use ri pi pico as i2c slave
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

#include <queue>
#include <array>
#include <stdint.h>
#include <stdexcept>

#include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <hardware/i2c.h>

#include <i2c_slave_lib.h>

#include <i2c.hpp>
#include <buffered_communicator.hpp>

namespace Lannooleaf {

  /**
   * @brief Class that inherits from I2C and BufferedCommunicator
   * On creation the hardware will be set up to be used as i2c slave
   * The BufferedCommunicator interface can be used to access received and to send data
   * 
   * @throws runtime_error when there was a atempt to use already in use hardware
   */
  class I2CSlave : public baseclasses::I2C, public baseclasses::BufferedCommunicator {
    
    public:
      I2CSlave(uint8_t slave_address, i2c_inst_t* i2c, uint sda, uint scl, uint frequency);
      ~I2CSlave();

      /**
       * @brief Get current i2c address
       * 
       * @return uint8_t 
       */
      uint8_t address(void) {
        return _address;
      }

      /**
       * @brief Assingn a new i2c address
       * 
       * @param address 
       * address to use
       */
      void address(uint8_t address);
    
    private:
      uint8_t _address;

    private:
      inline static queue_t* s0_read_fifo = nullptr;
      inline static queue_t* s0_write_fifo = nullptr;

      static void __not_in_flash_func(i2c0_irq_callback)(i2c_inst_t* i2c, i2c_slave_event_t event);

  };

}
