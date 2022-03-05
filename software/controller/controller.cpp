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
        PRINT_A("scanning node: 0x%02x ", node->i2c_address);
        PRINT_A("on pin: %i\r\n", pin);

        if (node -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
          gpio_put(pin, true);
        } else {
          this -> leaf_master.send_slave_message(node -> i2c_address, {
            slave_set_sel_pin,
            2,
            { (uint8_t)pin, 1 }
          });
        }

        sleep_ms(10);

        this -> assign_new_address();

        if (node -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
          gpio_put(pin, false);
        } else {
          this -> leaf_master.send_slave_message(node -> i2c_address, {
            slave_set_sel_pin,
            2,
            { (uint8_t)pin, 0 }
          });
        }

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
      PRINT_A("Scanning 0x%02x for neigbors\r\n", itr -> second -> i2c_address);
      for (select_pins pin : all_select_pins) {
        if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
          gpio_put(pin, true);
        } else {
          leaf_master.send_slave_message(itr -> second -> i2c_address, {
            slave_set_sel_pin,
            2,
            { (uint8_t)pin, 1 }
          });
        }

        std::map <uint8_t, Node*>::iterator itr2;
        for (itr2 = this -> graph.map.begin(); itr2 != this -> graph.map.end(); itr2++) {
          if (itr -> second -> i2c_address == itr2 -> second -> i2c_address) continue;

          if (itr2 -> second -> i2c_address != I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
            leaf_master.send_slave_message(itr2 -> second -> i2c_address, {
              slave_get_sel_pin,
              0,
              { }
            });

            leaf_master.get_slave_data(itr2 -> second -> i2c_address, 1);

            if (leaf_master.memory(0)) {
              PRINT_A("Neighbor found 0x%02x adding edge\r\n", itr2 -> second -> i2c_address);
              graph.add_edge(itr -> second -> i2c_address, sel_pin_to_side((select_pins) pin), itr2 -> second -> i2c_address);
            }
            leaf_master.reset_mem();
          } else {
            for (select_pins pin2 : all_select_pins) {
              gpio_set_dir(pin2, GPIO_IN);
              sleep_ms(SLEEP_TIME);
              if (gpio_get(pin2)) {
                PRINT_A("Neighbor found 0x%02x adding edge\r\n", itr2 -> second -> i2c_address);
                graph.add_edge(itr -> second -> i2c_address, sel_pin_to_side((select_pins) pin), itr2 -> second -> i2c_address);
              }
              gpio_set_dir(pin2, GPIO_OUT);
            }
          }
        }

        if (itr -> second -> i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
          gpio_put(pin, false);
        } else {
          leaf_master.send_slave_message(itr -> second -> i2c_address, {
            slave_set_sel_pin,
            2,
            { (uint8_t)pin, 0 }
          });
        }
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

  void Controller::assign_new_address(void) {
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

    if (leaf_master.memory(0) == 0xA5 && leaf_master.memory(1) == 0x5A) {
      PRINT_A("Device discovered assigned address 0x%02x to device\r\n", next_address);
      leaf_master.reset_mem();
      graph.add_node(next_address);
    }
  }

  uint8_t Controller::get_next_available_address(void) {
    for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
      if (graph.map.find(addr) == graph.map.end()) {
        if (!reserved_addr(addr)) return addr;
      }
    }
    return (uint8_t) UNCONFIGUREDADDRESS;
  }

}
