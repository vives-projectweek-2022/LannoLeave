class Node {
  constructor(address) {
    this.address = address;
    this.adj = new Map();
  }
}

export default class Graph {

  constructor() {
    this.map = new Map();
  }

  AddNode(address) {
    if (!this.map.has(address)) {
      this.map.set(address, new Node(address));
    }
  }

  AddEdge(from, from_side, to) {
    if (this.map.has(from) && this.map.has(to)) {
      let node_from = this.map.get(from);
      let node_to = this.map.get(to);

      node_from.adj.set(from_side, node_to);
    }
  }
  
  Get() {
    return this.map.get(0xff);
  }

}