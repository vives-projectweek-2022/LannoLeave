#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <pico/multicore.h>

#include <commands.h>
#include <controller.h>
#include <helper_funcs_var.h>
#include <spi_slave.h>

#include <PicoLed.hpp>

#define VERSION 1

#define SDA 8
#define SCL 9
#define MOSI 0
#define MISO 3
#define CLK 2
#define CS 1

using namespace Lannooleaf;

volatile bool timer_triggered = false;

Controller controller(i2c0, SDA, SCL, MOSI, MISO, CLK, CS);

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

void spi_core(void) {

  // Initialze spi on core 1 to handel interupts on core 1
  Spi_slave::Get().initialize(0, 3, 2, 1);

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_leaf_led, [&]() {
    const Packet& pkt = Spi_slave::Get().fifo.front();

  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_all, [&](){
    const Packet& pkt = Spi_slave::Get().fifo.front();

    controller.ledstrip.fill(PicoLed::RGB(pkt.read_buffer[1], pkt.read_buffer[2], pkt.read_buffer[3]));
    controller.ledstrip.show();
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_leaf_led, [&](){

  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_all, [&](){

  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_random, [&](){
    // TODO: implement
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::version, [&](){
    uint8_t version = VERSION;
  });

  while (true) {
    tight_loop_contents();
  }
}

int main() {
  stdio_init_all();
  set_alive_led();

  sleep_ms(2000);

  controller.device_discovery();
  controller.topology_discovery();

  PRINT(controller.graph.to_string().c_str());
  PRINT("\n");

  multicore_launch_core1(spi_core);

  while (true) {
    // Handel incomming commands from core 0
    if (!Spi_slave::Get().fifo.empty()) {
      controller.c_command_handler.handel_command(Spi_slave::Get().fifo.front().read_buffer[0]);
      Spi_slave::Get().fifo.pop();
    } 
  }
}

