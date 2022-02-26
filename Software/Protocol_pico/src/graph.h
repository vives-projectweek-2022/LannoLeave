#pragma once

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <map>

namespace LannoLeaf {

  struct Node {
    std::vector<Node *> adj;
    uint8_t i2c_address;
    Node(uint8_t address) : i2c_address(address) {}
  };

  class Graph {

    public:
      Graph();
      ~Graph();

    public:
      typedef std::map<uint8_t, Node *> nmap;
      nmap map;

    public:
      void add_node(const uint8_t&);
      void add_edge(const uint8_t& from, const uint8_t& to);

    public:
      void print(void);

  };  

}
