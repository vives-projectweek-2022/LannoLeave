#pragma once

#include <queue>
#include <array>
#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <hardware/irq.h>
#include <hardware/resets.h>

namespace Lannooleaf {

  class Spi_slave {

    public:
      static void initialize(uint mosi, uint miso, uint clk, uint cs);

      static uint8_t pop(void) {
        uint8_t value = Get()._read_fifo.front();
        Get()._read_fifo.pop();
        return value;
      }

      static void push(uint8_t value) {
        Get()._write_fifo.push(value);
      }

      static bool empty(void) {
        return Get()._read_fifo.empty();
      }


    private:
      static Spi_slave& Get(void) {
        static Spi_slave instance;
        return instance;
      }
    
    private:
      static void spi_irq_handler(void);
    
    private:
      std::queue<uint8_t> _read_fifo;
      std::queue<uint8_t> _write_fifo;
      
    private:
      Spi_slave() {}
      ~Spi_slave(){}
    
    private:
      Spi_slave(Spi_slave const&)      = delete;
      void operator=(Spi_slave const&) = delete;

  };

}
