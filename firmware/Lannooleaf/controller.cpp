#include <controller.h>

namespace Lannooleaf {

  Controller::Controller(i2c_inst_t * i2c_leaf_inst, uint sda_pin, uint scl_pin):
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

        if (node -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) 
          gpio_put((uint)pin, true);
        else {
          std::array<uint8_t, 3> message = { (uint8_t)slave_commands::set_sel_pin, (uint8_t)pin, 1 };
          leaf_master.send_data(node->i2c_address, 
                                message.data(), 
                                message.size());
        }

        sleep_ms(10); //! DO NOT REMOVE !

        uint8_t next_assigned_address = this -> assign_new_address();


        if (node -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
          gpio_put((uint)pin, false);

          if (next_assigned_address != UNCONFIGUREDADDRESS) 
            graph.add_edge(I2C_CONTOLLER_PLACEHOLDER_ADDRESS, sel_pin_to_side(pin), next_assigned_address);

        } else {
          std::array<uint8_t, 3> message = { (uint8_t)slave_commands::set_sel_pin, (uint8_t)pin, 0 };
          leaf_master.send_data(node->i2c_address, 
                                message.data(), 
                                message.size());
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
          std::array<uint8_t, 3> message = { (uint8_t)slave_commands::set_sel_pin, (uint8_t)pin, 1 };
          leaf_master.send_data(itr->second->i2c_address,
                                message.data(), 
                                message.size());
        }
      
        // Send message to all slaves to save neighbor when one of select pins is high
        std::array<uint8_t, 2> message = { (uint8_t)slave_commands::is_neighbor, itr->second->i2c_address };
        leaf_master.send_data(GENCALLADR, 
                              message.data(), 
                              message.size());

        // Set select pin low
        if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) 
          gpio_put((uint)pin, false);
        else {
          std::array<uint8_t, 3> message = { (uint8_t)slave_commands::set_sel_pin, (uint8_t)pin, 0 };
          leaf_master.send_data(itr->second->i2c_address, 
                                message.data(), 
                                message.size());
        }
      }
    }

    for (itr = this -> graph.map.begin(); itr != this -> graph.map.end(); itr++ ) {
      // Skip controller
      if (itr->second->i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) continue;

      constexpr uint8_t size_msg = (uint8_t) slave_commands::get_neigbor_size;
      leaf_master.send_data(itr->second->i2c_address,
                            &size_msg,
                            1);

      uint8_t neighbor_count = 0;
      leaf_master.get_data( itr->second->i2c_address, 
                            &neighbor_count, 
                            1);

      constexpr u_int8_t info_msg = (uint8_t) slave_commands::get_neighbor_information;
      leaf_master.send_data(itr->second->i2c_address, 
                            &info_msg, 
                            1);

      std::vector<std::pair<uint8_t, side>> neighbors;
      for (int i = 0; i < neighbor_count; i++) {
        uint8_t neighbor;
        uint8_t n_side;

        leaf_master.get_data( itr->second->i2c_address,
                              &neighbor,
                              1);

        leaf_master.get_data( itr->second->i2c_address,
                              &n_side,
                              1);

        neighbors.push_back(std::make_pair(neighbor, sel_pin_state_to_side(n_side)));
      }
      
      printf("Neigbor count of 0x%02x = %i\n", itr->second->i2c_address, neighbor_count);
      for (std::pair<uint8_t, side> pair : neighbors) {
        graph.add_edge( itr->second->i2c_address, 
                        pair.second, 
                        pair.first);
      }

      printf("Done\n");
    }
  }

  void Controller::reset(void) {
    constexpr uint8_t message = (uint8_t)slave_commands::reset;
    leaf_master.send_data(GENCALLADR, 
                          &message, 
                          1);

    graph.clear();
    graph.add_node(I2C_CONTOLLER_PLACEHOLDER_ADDRESS);

    device_discovery();
    topology_discovery();
  }

  void Controller::add_controller_handlers(CommandHandler* handler) {
    handler->add_handler(
    (uint8_t)controller_commands::hello_message, [&](){
      Spi_slave::push(0x00); // 0x00 Indicates start of writen data

      const char hello[] = "HelloSpi!";
      for (char byte : hello) Spi_slave::push(byte);
    });
    
    handler->add_handler((uint8_t)controller_commands::set_all, [&]{
      PicoLed::Color color = PicoLed::RGB(Spi_slave::pop(), Spi_slave::pop(), Spi_slave::pop());

      ledstrip.fill(color);
      ledstrip.show();

      std::array<uint8_t, 4> message = { (uint8_t)slave_commands::set_all_led, color.red, color.green, color.blue };
      leaf_master.send_data(GENCALLADR, 
                            message.data(), 
                            message.size());

    });
  }

  uint8_t Controller::assign_new_address(void) {
    uint8_t next_address = get_next_available_address();

    std::array<uint8_t, 2> set_next_address = { (uint8_t)slave_commands::set_i2c_address, next_address };
    leaf_master.send_data(GENCALLADR, 
                          set_next_address.data(), 
                          set_next_address.size());

    constexpr uint8_t ping = (uint8_t)slave_commands::ping;
    leaf_master.send_data(next_address, 
                          &ping, 
                          1);

    std::array<uint8_t, 1> pong;
    leaf_master.get_data( next_address, 
                          pong.data(), 
                          pong.size());

    if (pong.at(0) == 0xa5) {
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
