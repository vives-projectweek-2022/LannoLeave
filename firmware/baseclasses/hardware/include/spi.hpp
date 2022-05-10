/**
 * @file spi.hpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief SPI base class
 * @version 0.1
 * @date 2022-05-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <stdint.h>
#include <stdexcept>

#include <hardware/spi.h>
#include <hardware/irq.h>
#include <pico/stdlib.h>

namespace Lannooleaf::baseclasses {

  /**
   * @brief Base class for the Ri Pi Pico spi
   * 
   * @throws runtime_error when trying to initialize already in use hardware
   * 
   */
  class SPI {

    public:
      SPI(spi_inst_t* spi, uint mosi, uint miso, uint clk, uint cs, uint frequency);
      ~SPI();

    protected:
      spi_inst_t* spi;
      uint mosi;
      uint miso;
      uint clk;
      uint cs;

  };

}
