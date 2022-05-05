/**
 * @file controller.cpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Source file to controller.h
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
#include <controller.h>

namespace Lannooleaf {

  Controller::Controller(i2c_inst_t * i2c_leaf_inst, uint sda_pin, uint scl_pin) : leaf_master(i2c_leaf_inst, sda_pin, scl_pin) {
    for (auto pin : all_select_pins)
      gpio_set_dir((uint)pin, GPIO_OUT);

    graph.add_node(I2C_CONTOLLER_PLACEHOLDER_ADDRESS);
  }

  Controller::~Controller() { }

  // Good luck ;)
  void Controller::device_discovery(void) {
    bool controller_done = false;
    
    std::function <void(Node*)> search = [&](Node* node) {
      // Take note of which nodes have been visited
      visited.push_back(node -> i2c_address);

      for (select_pins pin : all_select_pins) {
        set_select_pin((uint)pin, true, node->i2c_address);
        sleep_ms(10); //! DO NOT REMOVE !

        uint8_t next_address = get_next_available_address();

        std::array<uint8_t, 2> set_next_address = { (uint8_t)slave_commands::set_i2c_address, next_address };
        leaf_master.send_data(GENCALLADR, 
                              set_next_address.data(), 
                              set_next_address.size());
        sleep_ms(SLEEP_TIME);

        const uint8_t ping = (uint8_t)slave_commands::ping;
        leaf_master.send_data(next_address, 
                              &ping, 
                              1);
        sleep_ms(SLEEP_TIME);

        std::array<uint8_t, 1> pong = { 0 };
        leaf_master.get_data( next_address, 
                              pong.data(), 
                              pong.size());

        // If response add node to graph
        if (pong.at(0) == 0xa5) {
          graph.add_node(next_address); 
        }
        else next_address = UNCONFIGUREDADDRESS;

        // If current node is controller an it found another device add edge from controller to that device
        if (node->i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS && next_address != UNCONFIGUREDADDRESS) 
          graph.add_edge(I2C_CONTOLLER_PLACEHOLDER_ADDRESS, sel_pin_to_side(pin), next_address);
        
        set_select_pin((uint)pin, false, node->i2c_address);
        
        if (node->i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS && pin == select_pins::F) controller_done = true;
        if (!controller_done) continue;

        for (auto [i2c_address, node] : graph.map) {
          // Call search function on next avaiable node if it has not been visited yet
          if (std::find(visited.begin(), visited.end(), i2c_address) == visited.end()) search(node);
        }
      };
    };

    search(graph.map[I2C_CONTOLLER_PLACEHOLDER_ADDRESS]);
  }

  void Controller::topology_discovery(void) {
    for (auto [i2c_address, node] : graph.map) {
      for (select_pins pin : all_select_pins) {
        set_select_pin((uint)pin, true, i2c_address);
      
        // Send message to all slaves to take note of neighbor when one of select pins is high
        std::array<uint8_t, 2> message = { (uint8_t)slave_commands::is_neighbor, i2c_address };
        leaf_master.send_data(GENCALLADR, 
                              message.data(), 
                              message.size());
        sleep_ms(SLEEP_TIME);

        set_select_pin((uint)pin, false, i2c_address);
      }
    }

    for (auto [i2c_address, node] : graph.map) {
      // Skip controller we added controller edges in device discovery
      if (i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) continue;

      const uint8_t size_msg = (uint8_t) slave_commands::get_neigbor_size;
      leaf_master.send_data(i2c_address,
                            &size_msg,
                            1);
      sleep_ms(SLEEP_TIME);
    
      uint8_t neighbor_count = 0;
      leaf_master.get_data( i2c_address, 
                            &neighbor_count, 
                            1);
      sleep_ms(SLEEP_TIME);

      const u_int8_t info_msg = (uint8_t) slave_commands::get_neighbor_information;
      leaf_master.send_data(i2c_address, 
                            &info_msg, 
                            1);
      sleep_ms(SLEEP_TIME);

      std::vector<std::pair<uint8_t, uint8_t>> neighbors;
      for (int i = 0; i < neighbor_count; i++) {
        uint8_t neighbor;
        uint8_t n_side;

        leaf_master.get_data( i2c_address,
                              &neighbor,
                              1);
        sleep_ms(SLEEP_TIME);

        leaf_master.get_data( i2c_address,
                              &n_side,
                              1);
        sleep_ms(SLEEP_TIME);

        neighbors.push_back(std::make_pair(neighbor, n_side));
      }
      
      for (auto [neighbor, side] : neighbors) {
        graph.add_edge( i2c_address, 
                        sel_pin_state_to_side(side), 
                        neighbor);
      }
    }

    graph.prepare_data();
  }

  //* ------------------------------ Controller_helper_functions ------------------------------ *//

  void Controller::set_select_pin(uint pin, bool value, uint8_t address) {
    if (address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS)
      gpio_put(pin, value);
    else {
      std::array<uint8_t, 3> message = { (uint8_t)slave_commands::set_sel_pin, pin, value };
      leaf_master.send_data(address, 
                            message.data(), 
                            message.size());
      sleep_ms(SLEEP_TIME);
    }
  }

  uint8_t Controller::get_next_available_address(void) {
    for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
      if (graph.map.find(addr) == graph.map.end() && !reserved_addr(addr)) {
        return addr;
      }
    }
    return (uint8_t) UNCONFIGUREDADDRESS;
  }


  // * -------------------------------- Spi_command_handeling --------------------------------- *∕∕

  void Controller::add_controller_handlers(CommandHandler* handler) {
    handler->add_handler((uint8_t)controller_commands::hello_message, [&](){
      SPISlave::push(0x00); // 0x00 Indicates start of writen data

      const char hello[] = "HelloSpi!";
      for (char byte : hello) SPISlave::push(byte);
    });
    
    handler->add_handler((uint8_t)controller_commands::get_adj_list_size, [&](){
      SPISlave::push(0x00);
      uint16_t size = graph.to_vector().size();

      // Split unit16_t into 2 unit8_t
      uint8_t size_0 = size >> 8;
      uint8_t size_1 = size;

      SPISlave::push(size_0);
      SPISlave::push(size_1);
    });

    handler->add_handler((uint8_t)controller_commands::get_adj_list, [&](){
      SPISlave::push(0x00);
      for (auto byte : graph.to_vector()) {
        SPISlave::push(byte);
      }
    });

    handler->add_handler((uint8_t)controller_commands::set_all, [&]{
      uint8_t red, green, blue;
      red = SPISlave::pop();
      green = SPISlave::pop();
      blue = SPISlave::pop();
      
      ledstrip.fill(PicoLed::RGB(red, green, blue));
      ledstrip.show();

      std::array<uint8_t, 4> message = { (uint8_t)slave_commands::set_all_led, red, green, blue };
      leaf_master.send_data(GENCALLADR, 
                            message.data(), 
                            message.size());
    });

    handler->add_handler((uint8_t)controller_commands::set_leaf_led, [&](){
      uint8_t address, led, red, green, blue;
      address = SPISlave::pop();
      led = SPISlave::pop();
      red = SPISlave::pop();
      green = SPISlave::pop();
      blue = SPISlave::pop();

      if (address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
        ledstrip.setPixelColor(led, PicoLed::RGB(red, green, blue));
        ledstrip.show();
      } else {
        std::array<uint8_t, 5> message = { (uint8_t)slave_commands::set_led, led, red, green, blue };
        leaf_master.send_data(address, message.data(), message.size());
      }
    });

    handler->add_handler((uint8_t)controller_commands::set_led_string, [&](){
      uint8_t address = SPISlave::pop();
      std::array<Color, 16> color_string;
      
      for (int i = 0; i < color_string.size(); i++) {
        uint8_t red = SPISlave::pop();
        uint8_t green = SPISlave::pop();
        uint8_t blue = SPISlave::pop();
        
        color_string[i] = {
          red, green, blue
        };
      }
      
      if (address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
        int i = 0;
        for (auto [red, green, blue] : color_string) {
          ledstrip.setPixelColor(i, PicoLed::RGB(red, green, blue));
          i++;
        }
        ledstrip.show();
      } else {
        std::vector<uint8_t> message;
        message.push_back((uint8_t)slave_commands::set_led_string);

        for (auto [red, green, blue] : color_string) {
          message.push_back(red);
          message.push_back(green);
          message.push_back(blue);
        }
        
        leaf_master.send_data(address, message.data(), message.size());
      }
    });
  }

}
