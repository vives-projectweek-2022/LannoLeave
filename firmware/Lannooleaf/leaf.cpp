#include <leaf.h>

namespace Lannooleaf {

  Leaf::Leaf()  { }

  Leaf::~Leaf() { }

  void Leaf::add_leaf_handlers(CommandHandler* handler) {
    handler->add_handler((uint8_t)slave_commands::set_i2c_address, [&](){
      uint8_t new_address = I2c_slave::pop();

      if (I2c_slave::address() == UNCONFIGUREDADDRESS && this->sel_pin_status()) I2c_slave::address(new_address);
    });

    handler->add_handler((uint8_t)slave_commands::ping, [&](){
      I2c_slave::push(0xa5);
    }); 

    handler->add_handler((uint8_t)slave_commands::set_sel_pin, [&](){
      uint8_t pin   = I2c_slave::pop();
      uint8_t value = I2c_slave::pop();

      printf("Set sel pin: %i to %i\n", pin, value);

      if (value) { // Set high
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, true);
      } else {     // Set low
        gpio_put(pin, false);
        gpio_set_dir(pin, GPIO_IN);
      }
    });

    handler->add_handler((uint8_t)slave_commands::get_sel_pin, [&](){
      I2c_slave::push(this->sel_pin_status());
    });

    handler->add_handler((uint8_t)slave_commands::is_neighbor, [&](){
      uint8_t neighbor = I2c_slave::pop();     
      if (this->sel_pin_status()) 
        this->neighbors.push_back(std::make_pair(neighbor, this->sel_pin_status())); 
    });

    handler->add_handler((uint8_t)slave_commands::get_neigbor_size, [&](){
      printf("Neig size: %i\n", this->neighbors.size());
      I2c_slave::push(this->neighbors.size());
    });

    handler->add_handler((uint8_t)slave_commands::get_neighbor_information, [&](){
      printf("Neighbor information\n");
      for (auto [address, side] : neighbors) {
        I2c_slave::push(address);
        I2c_slave::push((uint8_t)side);
      }
    });

    handler->add_handler((uint8_t)slave_commands::set_led, [&](){
      uint8_t led, red, green, blue;
      led = I2c_slave::pop();
      red = I2c_slave::pop();
      green = I2c_slave::pop();
      blue = I2c_slave::pop();

      ledstrip.setPixelColor(led, PicoLed::RGB(red, green, blue));
      ledstrip.show();
    });

    handler->add_handler((uint8_t)slave_commands::set_all_led, [&](){
      uint8_t red, green, blue;
      red = I2c_slave::pop();
      green = I2c_slave::pop();
      blue = I2c_slave::pop();

      ledstrip.fill(PicoLed::RGB(red, green, blue));
      ledstrip.show();
    });

    handler->add_handler((uint8_t)slave_commands::set_led_string, [&](){
      std::array<Color, 16> color_string;
      int i = 0;
      for (auto [red, green, blue] : color_string) {
        red = I2c_slave::pop();
        green = I2c_slave::pop();
        blue = I2c_slave::pop();

        ledstrip.setPixelColor(i, PicoLed::RGB(red, green, blue));
        i++;
      }
      ledstrip.show();
    });

    handler->add_handler((uint8_t)slave_commands::discovery_done, [&](){
      discover_animation(&ledstrip, {100, 50, 0});
    });

    handler->add_handler((uint8_t)slave_commands::reset, [&](){
      // TODO: Implement
    });
  }

  void Leaf::update_sel_status(void) {
    _sel_pin_status = 0;

    uint8_t i = 0;
    for (select_pins pin : all_select_pins) {
      if (!gpio_is_dir_out((uint)pin) && gpio_get((uint)pin)) _sel_pin_status |= 1 << i;
      i++;
    }
  }

}
