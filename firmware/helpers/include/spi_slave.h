/**
 * @file spi_slave.h
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Singelton class to use ri pi pico as spi slave
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
#include <stdio.h>
#include <stdint.h>

#include <helper_funcs_var.h>

#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <hardware/irq.h>
#include <hardware/resets.h>
#include <pico/util/queue.h>

namespace Lannooleaf {

  template<auto T = 0>
  class slave {
    static_assert(T == 0 || T == 1);
  };

  class Spi_slave {
    public:
      /**
       * @brief Initialize spi slave
       * 
       * @param mosi 
       * mosi gpio pin
       * @param miso 
       * miso gpio pin
       * @param clk 
       * clk gpio pin
       * @param cs 
       * cs gpio pin
       */
      static void initialize(uint mosi, uint miso, uint clk, uint cs);

      /**
       * @brief Get a value from receive buffer
       * 
       * @return uint8_t 
       */
      static uint8_t pop(void) {
        uint8_t value;
        queue_remove_blocking(&Get()._read_fifo, &value);
        return value;
      }

      /**
       * @brief Push a value to send buffer
       * 
       * @param value 
       * value to push
       */
      static void push(uint8_t value) {
        queue_add_blocking(&Get()._write_fifo, &value);
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
        return queue_is_empty(&Get()._read_fifo);
      }

      /**
       * @brief Clears both receive and sendbuffers
       */
      static void reset(void) {
        queue_free(&Get()._read_fifo);
        queue_free(&Get()._write_fifo);
      }

    private:
      static Spi_slave& Get(void) {
        static Spi_slave instance;
        return instance;
      }
    
    private:
      static void spi_irq_handler(void);
      static void cs_callback(uint gpio, uint32_t events);
    
    private:
      queue_t _read_fifo;
      queue_t _write_fifo;
      
    private:
      Spi_slave() {}
      ~Spi_slave(){}
    
    private:
      Spi_slave(Spi_slave const&)      = delete;
      void operator=(Spi_slave const&) = delete;

  };

}
