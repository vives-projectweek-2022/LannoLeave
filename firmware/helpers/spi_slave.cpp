/**
 * @file spi_slave.cpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Source file to spi_slave.h
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

#include "spi_slave.h"

namespace Lannooleaf {

  void Spi_slave::initialize(uint mosi, uint miso, uint clk, uint cs) {
    static bool initialized = false;
    
    if (!initialized) {
      queue_init_with_spinlock(&Get()._read_fifo, 1, 200, 1);
      queue_init_with_spinlock(&Get()._write_fifo, 1, 200, 2);

      gpio_init(cs);

      gpio_set_function(mosi, GPIO_FUNC_SPI);
      gpio_set_function(miso, GPIO_FUNC_SPI);
      gpio_set_function(clk, GPIO_FUNC_SPI);
      gpio_set_function(cs, GPIO_FUNC_SPI);

      reset_block(RESETS_RESET_SPI0_BITS);
      unreset_block_wait(RESETS_RESET_SPI0_BITS);  

      spi_set_baudrate(spi0, 1000000);
    
      hw_set_bits(&spi_get_hw(spi0)->dmacr, SPI_SSPDMACR_TXDMAE_BITS | SPI_SSPDMACR_RXDMAE_BITS);

      spi_set_slave(spi0, true);

      // 8 bit transfers / SPI Mode3 / MSB first
      spi_set_format(spi0, 
      8,
      SPI_CPOL_1,
      SPI_CPHA_1,
      SPI_MSB_FIRST);

      hw_set_bits(&spi_get_hw(spi0)->imsc, SPI_SSPIMSC_TXIM_BITS);
      irq_set_exclusive_handler(SPI0_IRQ, spi_irq_handler);
      irq_set_enabled(SPI0_IRQ, true);

      irq_set_priority(SPI0_IRQ, PICO_HIGHEST_IRQ_PRIORITY);

      gpio_set_irq_enabled_with_callback(cs, GPIO_IRQ_EDGE_RISE, true, cs_callback);

      // Enable spi
      hw_set_bits(&spi_get_hw(spi0)->cr1, SPI_SSPCR1_SSE_BITS);

      initialized = true;
    }
  }

  void __not_in_flash_func(Spi_slave::cs_callback)(uint gpio, uint32_t events) {
    while (spi_get_hw(spi0)->sr & 0x04) {  
      uint8_t value = spi_get_hw(spi0)->dr;
      queue_try_add(&Spi_slave::Get()._read_fifo, &value);
    }
  }

  void __not_in_flash_func(Spi_slave::spi_irq_handler)(void) {
    uint32_t status = spi_get_hw(spi0)->mis;
    
    // Tx fifo half empty or less
    while (status & SPI_SSPMIS_TXMIS_BITS) {
      // Adding 0xff if writefifo is empty
      while (spi_get_hw(spi0)->sr & 0x04) {  
        uint8_t value = spi_get_hw(spi0)->dr;
        queue_try_add(&Spi_slave::Get()._read_fifo, &value);
      }

      uint8_t value = 0xff;

      queue_try_remove(&Get()._write_fifo, &value);

      spi_get_hw(spi0)->dr = value;

      status = spi_get_hw(spi0)->mis;
    }
  }

}
