#pragma once

#include <vector>
#include <stdint.h>
#include <unordered_map>
#include <functional>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "commands.h"
#include "leaf.h"
#include "graph.h"

namespace LannoLeaf {

  class Controller : public Leaf {

    public:
      Controller(uint8_t address, i2c_inst_t * i2c);
      ~Controller();

    public:
      virtual void initialize(void) override;
      virtual void update(void) override;

    public:
      void search_topology(void);

    private:
      void scan_i2c_devices(void);
      void assign_new_address(void);

    private:
      void send_slave_message(uint8_t slave_address, message msg);
      void get_slave_data(uint8_t slave_address, size_t lenght);

    private:
      Graph graph;
      uint8_t memory[8];
      uint8_t last_write_status_code;

    private:  
      uint8_t get_next_available_address(void);
  
  };
  
}
