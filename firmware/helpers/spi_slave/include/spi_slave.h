#pragma once

#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <hardware/spi.h>

namespace LannoLeaf {

  class Spi_slave {

    public:
      Spi_slave(uint mosi, uint miso, uint clk, uint cs);
      Spi_slave();

    public:
      uint8_t read_command(void);

    public:
      void read_data(uint8_t *data_buffer, size_t len);
      void write_data(uint8_t *data_buffer, size_t len);

  };

}
