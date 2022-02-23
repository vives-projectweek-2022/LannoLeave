#pragma once

#include <vector>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "commands.h"
#include "leaf.h"

namespace LannoLeaf {

  struct LeafData {
    u_int8_t i2c_address;
  };

  class Controller {

    public:
      Controller();
      ~Controller();

    public:
      void scan_i2c_devices(void);
    
    public:
      std::vector<uint8_t> connected_devices;
      uint8_t response_buffer[8] = { 0x00 };

    public:
      void send_slave_message(uint8_t slave_address, commands cmd);

    private:
      void init(void);
      bool reserved_addr(uint8_t addr);

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
