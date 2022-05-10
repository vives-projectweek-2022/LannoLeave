/**
 * @file graph.h
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Graph datastructure class
 * @version 0.1
 * @date 2022-05-04
 * 
 * @copyright Copyright 2022 Joey De Smet
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *         http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdint.h>

#include <helper_funcs_var.hpp>

namespace Lannooleaf {
  /**
   * @brief Stuct to hold i2c address and adjecentcies
   */
  struct Node {
    Node(uint8_t address): 
      i2c_address(address) { }

    typedef std::pair<side, Node*> node_edge;
    std::vector<node_edge> adj;
    
    uint8_t i2c_address;
  };

  class Graph {

    public:
      Graph();
      ~Graph();

    public:
      typedef std::map<uint8_t, Node*> nodemap;
      nodemap map;

    public:
      /**
       * @brief Add a node to the graph
       * 
       * @param i2c_address 
       * i2c address of the node
       */
      void add_node(const uint8_t& i2c_address);

      /**
       * @brief Add a edge between two nodes
       * 
       * @param from 
       * i2c address of from node
       * @param from_side 
       * side to add to edge
       * @param to 
       * i2c address of to node
       */
      void add_edge(const uint8_t& from, const side& from_side, const uint8_t& to);

      /**
       * @brief Serialized graph to be send to higher end using spi
       */
      void prepare_data(void);

    public:
      /**
       * @brief Get reference to vector of serialized graph representation
       * 
       * @return std::vector<uint8_t>& 
       */
      std::vector<uint8_t>& to_vector(void) { return vector; };

    public:
      /**
       * @brief Delete all element in graph
       */
      void clear(void);
      
    public:
      /**
       * @brief Creates string representation of grapg
       * 
       * @return 
       * string representation of graph
       */
      std::string to_string(void);

    private:
      std::vector<uint8_t> vector;

  };

}
