#include <controller.h>

#define I2C_CONTOLLER_PLACEHOLDER_ADDRESS 0xFF

namespace LannoLeaf {

  Controller::Controller(i2c_inst_t * i2c_leaf_inst) { 
    leaf_master.set_i2c_inst(i2c_leaf_inst);
    initialize();
  }

  Controller::~Controller() {}

  void Controller::initialize(void) {
    for (select_pins pin : all_select_pins) {
      gpio_init(pin);
      gpio_set_dir(pin, GPIO_OUT);
    }

    gpio_init(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);

    gpio_init(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    i2c_init(i2c0, BAUDRATE);

    uart_init(uart0, 9600);

    gpio_set_function(12, GPIO_FUNC_UART);
    gpio_set_function(13, GPIO_FUNC_UART);

    graph.add_node(I2C_CONTOLLER_PLACEHOLDER_ADDRESS);
  }

  void Controller::device_discovery(void) {
    std::function <void(Node*)> search = [&](Node* node) {
      this -> visited.push_back(node -> i2c_address);
      for (select_pins pin : all_select_pins) {
        if (node -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) gpio_put(pin, true);
        else this -> leaf_master.send_slave_message(node -> i2c_address, {
          slave_set_sel_pin,
          2,
          { (uint8_t)pin, 1 }
        });

        sleep_ms(10);

        uint8_t next_assigned_address = this -> assign_new_address();

        if (node -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
          gpio_put(pin, false);

          if (next_assigned_address != UNCONFIGUREDADDRESS) {
            graph.add_edge(I2C_CONTOLLER_PLACEHOLDER_ADDRESS, sel_pin_to_side(pin), next_assigned_address);
          }
        } else this -> leaf_master.send_slave_message(node -> i2c_address, {
          slave_set_sel_pin,
          2,
          { (uint8_t)pin, 0 }
        });

        std::map <uint8_t, Node*>::iterator itr;
        for (itr = this -> graph.map.begin(); itr != this -> graph.map.end(); itr++) {
          if (!std::count(this -> visited.begin(), this -> visited.end(), itr -> second -> i2c_address)) {
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

        if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) gpio_put(pin, true);
        else leaf_master.send_slave_message(itr -> second -> i2c_address, {
          slave_set_sel_pin,
          2,
          { (uint8_t)pin, 1 }
        });
      
        leaf_master.send_slave_message(GENCALLADR, {
          slave_is_neighbor,
          1,
          { itr -> second -> i2c_address }
        });

        if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) gpio_put(pin, false);
        else leaf_master.send_slave_message(itr -> second -> i2c_address, {
          slave_set_sel_pin,
          2,
          { (uint8_t)pin, 0 }
        });
      }
    }

    for (itr = this -> graph.map.begin(); itr != this -> graph.map.end(); itr++ ) {
      if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) continue;

      leaf_master.send_slave_message(itr -> second -> i2c_address, {
        slave_neighbor_size,
        0,
        { }
      });

      leaf_master.get_slave_data_no_mem_reset(itr -> second -> i2c_address, 1);

      uint8_t neigbor_count = leaf_master.memory[0];
      leaf_master.get_slave_data(itr -> second -> i2c_address, neigbor_count * 2);

      for (uint8_t i = 0; i <= neigbor_count; i += 2) {
        graph.add_edge(itr -> second -> i2c_address, sel_pin_state_to_side(leaf_master.memory[i + 1]), leaf_master.memory[i]);
      }
    }
  }

  void Controller::reset(void) {
    leaf_master.send_slave_message(GENCALLADR, {
      slave_reset,
      0,
      { }
    });

    graph.clear();
    visited.clear();
    graph.add_node(I2C_CONTOLLER_PLACEHOLDER_ADDRESS);

    device_discovery();
    topology_discovery();
  }

  uint8_t Controller::assign_new_address(void) {
    uint8_t next_address = get_next_available_address();

    leaf_master.send_slave_message(GENCALLADR, {
      slave_set_i2c_address,
      1,
      { next_address }
    });

    leaf_master.send_slave_message(next_address, {
      slave_ping,
      1,
      { 2 }
    });

    leaf_master.get_slave_data(next_address, 2);

    if (leaf_master.memory[0] == 0xA5 && leaf_master.memory[1] == 0x5A) {
      leaf_master.reset_mem();
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
