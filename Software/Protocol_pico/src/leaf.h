#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

namespace LannoLeaf {

  class Leaf {

    public:
      Leaf(uint8_t address);
      ~Leaf();
  
    public:
      void update(void);

    private:
      void init(void);

    private:
      uint8_t address;

    private:
      enum SelectPin {
        UP = 24,
        DOWN,
        LEFT,
        RIGHT
      };

  };

}
