#pragma once

#include <stdint.h>
#include <stdio.h>
#include <hardware/spi.h>
#include <pico/stdlib.h>

namespace LannoLeaf {

  class Spi_command_handler {

    public:
      Spi_command_handler(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t cs);
      Spi_command_handler();

    public:
      uint8_t read_command(void);

    public:
      void read_data(uint8_t *data_buffer, size_t len);
      void write_data(uint8_t *data_buffer, size_t len);

  };

}
