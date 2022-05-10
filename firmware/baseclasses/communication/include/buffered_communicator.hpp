/**
 * @file buffered_communicator.hpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Base interface class for communication classes
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

#include <stdint.h>
#include <pico/util/queue.h>

namespace Lannooleaf::baseclasses {

  /**
   * @brief  
   * Base class to be inherited by communication protocol classes such as (spi, i2c, uart, ...)
   * to give buffered read an write functionality/interface
   */
  class BufferedCommunicator {

    public:
      BufferedCommunicator();

    public:
      /**
       * @brief Read a byte from receive queue
       * 
       * @return First byte in the queue
       */
      uint8_t read_byte(void);

      /**
       * @brief Write a byte to the send queue
       * 
       * @param byte 
       * Byte to send
       */
      void write_byte(uint8_t byte);

      /**
       * @brief Check whether receive queeu has been writen to
       * 
       * @return true 
       * Receive queue has data
       * @return false 
       * Receive queue does not have data
       */
      bool readable(void);

      /**
       * @brief Clears both receive and sendbuffers
       */
      void reset(void);

    public:
      queue_t read_fifo;
      queue_t write_fifo;
    
  };

}
