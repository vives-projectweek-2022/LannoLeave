/**
 * @file graph.cpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Sourec file to graph.h
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

#include <graph.hpp>

namespace Lannooleaf {

  Graph::Graph() { }

  Graph::~Graph() {
    std::map < uint8_t, Node * > ::iterator i;
    for (i = map.begin(); i != map.end(); i++) {
      delete i -> second;
    }
  }

  void Graph::add_node(const uint8_t & address) {
    nodemap::iterator itr = map.find(address);

    if (itr == map.end()) {
      Node * node;
      node = new Node(address);
      map[address] = node;
      return;
    }
  }

  void Graph::add_edge(const uint8_t & from, const side & from_side, const uint8_t & to) {
    Node * f = (map.find(from) -> second);
    Node * t = (map.find(to) -> second);

    std::pair < side, Node * > from_edge = std::make_pair(from_side, t);

    f -> adj.push_back(from_edge);
  }

  void Graph::clear(void) {
    std::map < uint8_t, Node * > ::iterator i;
    for (i = map.begin(); i != map.end(); i++) {
      delete i -> second;
    }

    map.clear();
  }

  void Graph::prepare_data(void) {
    vector.clear();
    for (auto [address, node] : map) {
      vector.push_back(address);
    }

    if (map.size() ==  1) return;

    vector.push_back(0x00);
    for (auto [address, node] : map) {
      for (auto [side, to] : node->adj) {
        vector.push_back(address);
        vector.push_back((uint8_t)side);
        vector.push_back(to->i2c_address);
      }
    }
  }

  std::string Graph::to_string(void) {
    std::stringstream stream;
    std::map <uint8_t, Node *> ::iterator i;
    for (i = map.begin(); i != map.end(); i++) {
      stream << "Node: "  << std::to_string(i -> second -> i2c_address) << "\r\n";

      stream << "Adjecent nodes: ";
      for (std::vector < std::pair < side, Node * >> ::iterator itr = i -> second -> adj.begin(); itr != i -> second -> adj.end(); ++itr) {
        stream << "side: " << side_to_char(itr -> first) << " -> " << std::to_string(itr -> second -> i2c_address);
      }

      stream << "\r\n";
    }

    return stream.str();
  }

}
