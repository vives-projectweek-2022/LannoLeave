#pragma once
#include "mbed.h"

namespace LannooLeaf {

  class Tile {

    private:
      DigitalInOut* select[4];
      I2CSlave* bus;

    private:
      uint8_t select_state = 0x00;

    public:
      Tile(void);
      ~Tile();

    public:
      void update(void);

    private:
      void update_select_state(void);
    
  };

}