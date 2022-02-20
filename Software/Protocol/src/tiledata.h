#pragma once
#include "mbed.h"

namespace LannooLeaf {
  
  class TileData {

    public:
      uint8_t address = 0x00;
      bool led_status = false;
      uint8_t Red = 0;
      uint8_t Green = 0;
      uint8_t Blue = 0;
  
    public:
      string to_string(void);
  };

}