#include <stdio.h>
#include <stdint.h>

#include <leaf.h>
#include <commands.h>
#include <helper_funcs_var.h>
#include <i2c_slave.h>

#include <pico/stdlib.h>
#include <pico/multicore.h>

using namespace Lannooleaf;

Leaf leaf;

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

void sel_pin_irq_callback(uint gpio, uint32_t events) {
  I2c_slave::Get().initialize(UNCONFIGUREDADDRESS, i2c0, 8, 9);

  for (select_pins pin : all_select_pins) {
    gpio_set_irq_enabled_with_callback((uint)pin, GPIO_IRQ_EDGE_RISE, false, sel_pin_irq_callback);
  }
}

void i2c_slave_core(void) {

  for (select_pins pin : all_select_pins) {
    gpio_init((uint)pin);
    gpio_set_dir((uint)pin, GPIO_IN);
    gpio_set_irq_enabled_with_callback((uint)pin, GPIO_IRQ_EDGE_RISE, true, sel_pin_irq_callback);
  }

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::set_i2c_address, [&](){
    const std::array<uint8_t, 8>& data = I2c_slave::Get().read_fifo.front();
    if (I2c_slave::address() == UNCONFIGUREDADDRESS) {
      I2c_slave::Get().reassign_address(data[1]);
    } 
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::ping, [&](){
    constexpr std::array<uint8_t, 8> data = { 'H', 'E', 'L', 'L', 'O', 'I', '2', 'C' };
    I2c_slave::Get().write_fifo.push(data);
  }); 

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::set_sel_pin, [&](){
    const std::array<uint8_t, 8>& data = I2c_slave::Get().read_fifo.front();

    if (data[2]) { // Set high
      gpio_set_dir(data[1], GPIO_OUT);
      gpio_put(data[1], true);
    } else { // Set low
      gpio_put(data[1], false);
      gpio_set_dir(data[1], GPIO_IN);
    }
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::get_sel_pin, [&](){
    std::array<uint8_t, 8> data = { leaf.sel_pin_status() };
    I2c_slave::Get().write_fifo.push(data);
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::is_neighbor, [&](){
    const std::array<uint8_t, 8>& data = I2c_slave::Get().read_fifo.front();
    static uint8_t neighbor_count = 0x00;

    if (leaf.sel_pin_status()) {
      neighbor_count++;

      leaf.neighbors[neighbor_count - 1] = data[1];

      leaf.side[neighbor_count - 1] = leaf.sel_pin_status();

      leaf.neighbors[7] = neighbor_count;
    }
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::get_neighbor_information, [&](){
    I2c_slave::Get().write_fifo.push(leaf.neighbors);
    I2c_slave::Get().write_fifo.push(leaf.side); 
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::set_led, [&](){
    // TODO: Implement
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::set_all_led, [&](){
    // TODO: Implement
  });

  leaf.l_command_handler.add_handler((uint8_t)slave_commands::reset, [&](){
    // TODO: Implement
  });

  while (true) {
    tight_loop_contents();
  }
}

int main() {
  stdio_init_all();
  set_alive_led();

  multicore_launch_core1(i2c_slave_core);

  while (true) {
    if (!I2c_slave::Get().read_fifo.empty()) {
      leaf.update();
      leaf.l_command_handler.handel_command(I2c_slave::Get().read_fifo.front()[0]);
      I2c_slave::Get().read_fifo.pop();
    }
  }
}
