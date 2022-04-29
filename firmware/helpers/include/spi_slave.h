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

  class Spi_slave {
    public:
      /** \brief Initialize spi slave*/
      static void initialize(uint mosi, uint miso, uint clk, uint cs);

      /** \returns First value in internal read_fifo, will block when fifo is empty*/
      static uint8_t pop(void) {
        uint8_t value;
        queue_remove_blocking(&Get()._read_fifo, &value);
        return value;
      }

      /** \brief Add a value to internal write_fifo*/
      static void push(uint8_t value) {
        queue_add_blocking(&Get()._write_fifo, &value);
      }

      /** \returns boolean true if internal read_fifo is empty, false if not*/
      static bool empty(void) {
        return queue_is_empty(&Get()._read_fifo);
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
