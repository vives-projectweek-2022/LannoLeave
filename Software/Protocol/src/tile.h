#pragma once
#include "mbed.h"

namespace LannooLeaf {

  class Tile {

    private:
      DigitalInOut* select[4];

    public:
      Tile(void);
      ~Tile();

    public:
      void update(void);
    
  };

}