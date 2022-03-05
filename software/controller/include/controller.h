#pragma once

#include <vector>
#include <stdint.h>
#include <functional>
#include <map>

#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>

#include <i2c_master.h>

#include <commands.h>
#include <graph.h>

namespace LannoLeaf {

  class Controller {

    public:
      Controller(i2c_inst_t * i2c_leaf_inst);
      ~Controller();

    public:
      /** \brief Starts device discorvery algorithm*/
      void device_discovery(void);

      /** \brief Starts topology discovery algorithm*/
      void topology_discovery(void);

      /** \brief Resets all slaves and reruns discovery/topology discovery algorithm*/
      void reset(void);

    private:
      void initialize(void);
      void assign_new_address(void);

    private:  
      uint8_t get_next_available_address(void);

    public:
      Graph graph;
      I2CMaster leaf_master;

    private:
      std::vector<uint8_t> visited;
  
  };
  
}
