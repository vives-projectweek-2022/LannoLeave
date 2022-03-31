#pragma once

#include <queue>
#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <hardware/gpio.h>

namespace Lannooleaf {

  struct Packet {
    uint8_t read_buffer[8];
    uint8_t write_buffer[8];  
  };

  class Spi_slave {

    private:
      Spi_slave(){};
      ~Spi_slave(){};

    public:
      Spi_slave(Spi_slave const&) = delete;
      void operator=(Spi_slave const&) = delete;

    public:
      static Spi_slave& Get(void);

    public:
      std::queue<Packet> fifo;

    public:
      void initialize(uint mosi, uint miso, uint clk, uint cs);
      void add_write_data(uint8_t data[], size_t len);

    private:
      static void cs_callback(uint ce, uint32_t events);

    private:
      uint8_t read_buffer[8]  = {0};
      uint8_t write_buffer[8] = {0};

  };

}
