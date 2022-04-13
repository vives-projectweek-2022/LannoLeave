#include <leaf.h>

namespace Lannooleaf {

  Leaf::Leaf()  { }

  Leaf::~Leaf() { }

  void Leaf::update(void) { 
    update_sel_status();
  } 

  void Leaf::add_leaf_handlers(CommandHandler* handler) {
    handler->add_handler((uint8_t)slave_commands::set_i2c_address, [&](){
      uint8_t new_address = I2c_slave::pop();
      if (I2c_slave::address() == UNCONFIGUREDADDRESS) {
        I2c_slave::address(new_address);
      }
    });

    handler->add_handler((uint8_t)slave_commands::ping, [&](){
      I2c_slave::push(0xa5);
    }); 

    handler->add_handler((uint8_t)slave_commands::set_sel_pin, [&](){
      uint8_t pin   = I2c_slave::pop();
      uint8_t value = I2c_slave::pop();

      if (value) { // Set high
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, true);
      } else {     // Set low
        gpio_put(pin, false);
        gpio_set_dir(pin, GPIO_IN);
      }
    });

    handler->add_handler((uint8_t)slave_commands::get_sel_pin, [&](){
      I2c_slave::push(sel_pin_status());
    });

    handler->add_handler((uint8_t)slave_commands::is_neighbor, [&](){
      uint8_t neighbor = I2c_slave::pop();

      if (sel_pin_status()) {
        neighbors.push_back(std::make_pair(neighbor, (side)sel_pin_status()));
      }
    });

    handler->add_handler((uint8_t)slave_commands::get_neigbor_size, [&](){
      I2c_slave::push(neighbors.size());
    });

    handler->add_handler((uint8_t)slave_commands::get_neighbor_information, [&](){
      for (auto [address, side] : neighbors) {
        I2c_slave::push(address);
        I2c_slave::push((uint8_t)side);
      }
    });

    handler->add_handler((uint8_t)slave_commands::set_led, [&](){
      ledstrip.setPixelColor(I2c_slave::pop(), PicoLed::RGB(I2c_slave::pop(), I2c_slave::pop(), I2c_slave::pop()));
      ledstrip.show();
    });

    handler->add_handler((uint8_t)slave_commands::set_all_led, [&](){
      ledstrip.fill(PicoLed::RGB(I2c_slave::pop(), I2c_slave::pop(), I2c_slave::pop()));
      ledstrip.show();
    });

    handler->add_handler((uint8_t)slave_commands::reset, [&](){
      // TODO: Implement
    });
  }

  void Leaf::update_sel_status(void) {
    _sel_pin_status = 0x00;
    
    uint8_t i = 0;
    for (select_pins pin : all_select_pins) {
      if (!gpio_is_dir_out((uint)pin) && gpio_get((uint)pin)) _sel_pin_status |= 1 << i;
      i++;
    }
  }

}
