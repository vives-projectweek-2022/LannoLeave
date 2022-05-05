/**
 * @file i2c_slave.h
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Singelton class to use ri pi pico as i2c slave
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

#include <helper_funcs_var.h>

#include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <hardware/i2c.h>

#include <i2c_slave_lib.h>

namespace Lannooleaf {

  class I2c_slave {
    
    public:
      /**
       * @brief Initialize i2c slave
       * 
       * @param slave_address 
       * address to be initialzed
       * @param i2c 
       * i2c_inst_t pointer (i2c0 or i2c1)
       * @param sda 
       * sda gpio pin
       * @param scl 
       * scl gpio pin
       */
      static void initialize(uint8_t slave_address, i2c_inst_t* i2c, uint sda, uint scl);

      /**
       * @brief Get a value from the receive queue
       * 
       * @return uint8_t 
       */
      static uint8_t pop(void) {
        uint8_t value;
        queue_remove_blocking(&Get().read_fifo, &value);
        return value;
      }

      /**
       * @brief Get current i2c address
       * 
       * @return u_int8_t 
       */
      static u_int8_t address(void) {
        return Get()._address;
      }

      /**
       * @brief Assingn a new i2c address
       * 
       * @param address 
       * address to use
       */
      static void address(uint8_t address) {
        i2c_slave_deinit(Get().i2c);
        i2c_slave_init(Get().i2c, address, i2c_irq_callback);
        Get()._address = address;
      }

      /**
       * @brief Add a byte to be push onto the send buffer
       * 
       * @param byte 
       * byte to push
       */
      static void push(uint8_t byte) {
        printf("P\n");
        queue_add_blocking(&Get().write_fifo, &byte);
        printf("D\n");
      }

      /**
       * @brief Check if receive buffer is empty
       * 
       * @return true 
       * buffer is empty
       * @return false 
       * buffer is not empty
       */
      static bool empty(void) {
        return queue_is_empty(&Get().read_fifo);
      }

    private:
      static I2c_slave& Get(void) {
        static I2c_slave instance;
        return instance;
      };

    private:
      static void i2c_irq_callback(i2c_inst_t* i2c, i2c_slave_event_t event);
    
    private:
      queue_t read_fifo;
      queue_t write_fifo;
    
    private:
      i2c_inst_t* i2c;
      uint8_t _address;

    private:
      I2c_slave() {}
      ~I2c_slave(){}

    private:
      I2c_slave(I2c_slave const&)      = delete;
      void operator=(I2c_slave const&) = delete;

  };

}
