#pragma once

#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdint.h>

#include <helper_funcs_var.h>

namespace LannoLeaf {

  struct coordinate {

    int16_t x;
    int16_t y;

    coordinate(int16_t x, int16_t y) : x(x), y(y) { }

    coordinate& operator=(coordinate a) {
      x = a.x;
      y = a.y;
      return *this;
    }

    coordinate operator+(coordinate a) const {
      return coordinate(a.x + x, a.y + y);
    }

    std::string to_string(void) {
      return "[" + std::to_string(x) + ", " + std::to_string(y) + "]"; 
    }

  };

  struct Node {
    Node(uint8_t address): 
      i2c_address(address) { }

    typedef std::pair<side, Node*> node_edge;
    std::vector<node_edge> adj;
    
    uint8_t i2c_address;
    coordinate pos = {0, 0};
  };

  class Graph {

    public:
      Graph();
      ~Graph();

    public:
      typedef std::map<uint8_t, Node*> nodemap;
      nodemap map;

    public:
      void add_node(const uint8_t& i2c_address);
      void add_edge(const uint8_t& from, const side& from_side, const uint8_t& to);

    public:
      void clear(void);
      
    public:
      std::string to_string(void);
      std::string node_to_coords(void);

  };

}
