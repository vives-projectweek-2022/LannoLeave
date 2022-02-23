#pragma once

#include <vector>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "commands.h"
#include "leaf.h"

namespace LannoLeaf {

  class Controller : public Leaf {

    public:
      Controller(uint8_t address);
      ~Controller();

    public:
      virtual void initialize(void) override;
      virtual void update(void);

    protected:
      bool reserved_addr(uint8_t addr);

    public:
      void scan_i2c_devices(void);
      void send_slave_message(uint8_t slave_address, commands cmd);
    
    // TODO: Make this private when no longer needed for debugging
    public:
      std::vector<uint8_t> connected_devices;
      uint8_t response_buffer[8] = {0};    
  
  };
  
}
