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
      Leaf(uint8_t address);
      ~Leaf();
  
    public:
      virtual void update(void);

    private:
      virtual void init(void);
      typedef void (*i2c_slave_handler_t)(i2c_inst_t *i2c, i2c_slave_event_t event);

    protected:
      bool reserved_addr(uint8_t addr);

    protected:
      uint8_t address;

    public:
      enum SelectPin {
        ONE=4,
        TWO,
        THREE,
        FOUR,
        FIVE=9
      };

  };

}
