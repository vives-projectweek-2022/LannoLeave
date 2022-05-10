/**
 * @file spi_slave.hpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief SPISlave class
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

#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <hardware/irq.h>

#include "spi.hpp"
#include "buffered_communicator.hpp"

namespace Lannooleaf {

  /**
   * @brief Spi slave class inherits from SPI and BufferedCommunicator
   * On creation the hardware will be set up to be used as spi slave
   * The BufferedCommunicator interface can be used to access received and to send data
   */
  class SPISlave : public baseclasses::SPI, public baseclasses::BufferedCommunicator {

    public:
      SPISlave(spi_inst_t* spi, uint mosi, uint miso, uint clk, uint cs, uint frequency);

    private:
      inline static queue_t* s0_read_fifo  = nullptr;
      inline static queue_t* s0_write_fifo = nullptr;

      static void __not_in_flash_func(spi0_irq_handel)(void);

  };

}
