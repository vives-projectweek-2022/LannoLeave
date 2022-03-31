#include <controller.h>

namespace Lannooleaf {

  Controller::Controller(i2c_inst_t * i2c_leaf_inst, uint sda_pin, uint scl_pin, uint mosi, uint miso, uint clk, uint cs):
  leaf_master(i2c_leaf_inst, sda_pin, scl_pin) { 
    
    for (select_pins pin : all_select_pins) {
      gpio_init((uint)pin);
      gpio_set_dir((uint)pin, GPIO_OUT);
    }

    graph.add_node(I2C_CONTOLLER_PLACEHOLDER_ADDRESS);
  }

  Controller::~Controller() { }

  void Controller::device_discovery(void) {
    std::vector<uint8_t> visited;

    bool controller_done = false;

    std::function <void(Node*)> search = [&](Node* node) {
      visited.push_back(node -> i2c_address);
      for (select_pins pin : all_select_pins) {
        if (node -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) gpio_put((uint)pin, true);
        else {
          uint8_t message[8] = { (uint8_t)slave_commands::set_sel_pin, (uint8_t)pin, 1 };
          leaf_master.send_data(node->i2c_address, message);
        }

        sleep_ms(10);

        uint8_t next_assigned_address = this -> assign_new_address();

        if (node -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
          gpio_put((uint)pin, false);

          if (next_assigned_address != UNCONFIGUREDADDRESS) graph.add_edge(I2C_CONTOLLER_PLACEHOLDER_ADDRESS, sel_pin_to_side(pin), next_assigned_address);

        } else {
          uint8_t message[8] = { (uint8_t)slave_commands::set_sel_pin, (uint8_t)pin, 0 };
          leaf_master.send_data(node->i2c_address, message);
        }

        std::map <uint8_t, Node*>::iterator itr;
        for (itr = this -> graph.map.begin(); itr != this -> graph.map.end(); itr++) {
          if (!controller_done) {
            controller_done = (node->i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS && pin == select_pins::F);
            continue;
          }

          if (!std::count(visited.begin(), visited.end(), itr -> second -> i2c_address)) {
            search(itr -> second);
          }
        }
      };
    };

    search(graph.map[I2C_CONTOLLER_PLACEHOLDER_ADDRESS]);
  }

  void Controller::topology_discovery(void) {
    std::map <uint8_t, Node*>::iterator itr;
    for (itr = this -> graph.map.begin(); itr != this -> graph.map.end(); itr++) {
      for (select_pins pin : all_select_pins) {

        // Set select pin high
        if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) gpio_put((uint)pin, true);
        else {
          uint8_t message[8] = { (uint8_t)slave_commands::set_sel_pin, (uint8_t)pin, 1 };
          leaf_master.send_data(itr->second->i2c_address, message);
        }
      
        // Send message to all slaves to save neighbor when one of select pins is high
        uint8_t message[8] = { (uint8_t)slave_commands::is_neighbor, itr->second->i2c_address };
        leaf_master.send_data(GENCALLADR, message);

        // Set select pin low
        if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) gpio_put((uint)pin, false);
        else {
          uint8_t message[8] = { (uint8_t)slave_commands::set_sel_pin, (uint8_t)pin, 0 };
          leaf_master.send_data(itr->second->i2c_address, message);
        }
      }
    }

    for (itr = this -> graph.map.begin(); itr != this -> graph.map.end(); itr++ ) {
      // Skip controller
      if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) continue;

      // Ask slave to ready neighbordata
      uint8_t message[8] = { (uint8_t)slave_commands::get_neighbor_information };
      leaf_master.send_data(itr->second->i2c_address, message);

      // Get the data
      std::array<uint8_t, 8> neighbors = leaf_master.get_data(itr->second->i2c_address);
      std::array<uint8_t, 8> side = leaf_master.get_data(itr->second->i2c_address);

      for (uint8_t i = 0; i < neighbors[7]; i++) {
        graph.add_edge(itr->second->i2c_address, sel_pin_state_to_side(side[i]), neighbors[i]);
      }
    }
  }

  void Controller::reset(void) {
    uint8_t message[8] = { (uint8_t)slave_commands::reset };
    leaf_master.send_data(GENCALLADR, message);

    graph.clear();
    graph.add_node(I2C_CONTOLLER_PLACEHOLDER_ADDRESS);

    device_discovery();
    topology_discovery();
  }

  uint8_t Controller::assign_new_address(void) {
    uint8_t next_address = get_next_available_address();

    uint8_t set_next_address[8] = { (uint8_t)slave_commands::set_i2c_address, next_address };
    leaf_master.send_data(GENCALLADR, set_next_address);

    uint8_t ping[8] = { (uint8_t)slave_commands::ping };
    leaf_master.send_data(next_address, ping);

    if (leaf_master.get_data(next_address).at(0) == 'H') {
      graph.add_node(next_address);
      return next_address;
    }

    return UNCONFIGUREDADDRESS;
  }

  uint8_t Controller::get_next_available_address(void) {
    for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
      if (graph.map.find(addr) == graph.map.end() && !reserved_addr(addr)) {
        return addr;
      }
    }
    return (uint8_t) UNCONFIGUREDADDRESS;
  }

}
