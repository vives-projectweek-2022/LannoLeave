#pragma once

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "commands.h"
#include "libs/i2c_slave.h"
#include "libs/i2c_fifo.h"

namespace LannoLeaf {

  class Leaf {

    public:
      Leaf(uint8_t address, i2c_inst_t * i2c);
      ~Leaf();
  
    public:
      virtual void initialize(void);
      virtual void update(void);

    protected:
      bool reserved_addr(uint8_t addr);

    protected:
      void address(uint8_t address);

    private:
      void update_sel_status(void);
      void handle_data(void);

    private:
      uint8_t sel_pin_status;
      bool slave_initialized = false;

    protected:
      uint8_t _address;
      i2c_inst_t * i2c;

    public:
      typedef enum select_pins {
        A=16,
        B,
        C,
        D,
        E
      } select_pins;

  };

  static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event);
}
