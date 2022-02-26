#include "graph.h"

namespace LannoLeaf {

  Graph::Graph() { }
  
  Graph::~Graph() { 
    std::map<uint8_t, Node *>::iterator i;
    for (i = map.begin(); i != map.end(); i++) {
      delete i->second;
    }
  }

  void Graph::add_node(const uint8_t &address) {
    nmap::iterator itr = map.find(address);

    if (itr == map.end()) {
      Node *node;
      node = new Node(address);
      map[address] = node;
      return;
    }
  }

  void Graph::add_edge(const uint8_t& from, const uint8_t& to) {
    Node *f = (map.find(from)->second);
    Node *t = (map.find(to)->second);
    f->adj.push_back(t);
    t->adj.push_back(f);
  }

  void Graph::print(void) {
    std::map<uint8_t, Node *>::iterator i;
    for (i = map.begin(); i != map.end(); i++) {
      printf("Node: 0x%02x\r\n", i->second->i2c_address);

      printf("Adjecent nodes: ");
      for (Node * adj_node : i->second->adj) {
        printf("0x%02x, ", adj_node->i2c_address);
      }

      printf("\r\n");
    }
  }

}
