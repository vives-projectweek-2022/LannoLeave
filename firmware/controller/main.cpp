#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>
#include <pico/multicore.h>

#include <commands.h>
#include <controller.h>
#include <helper_funcs_var.h>
#include <pixel_mapping.h>

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
PixelMapping mapping(&controller.graph);

void set_alive_led(void) {
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
}

void spi_core(void) {
  Spi_slave spi_slave(0, 2, 3, 1);

  controller.c_command_handler.add_handler((uint8_t)controller_commands::get_matrix_size, [&](context* ctx){
    uint8_t buffer[2];
    buffer[0] = (uint8_t)mapping.get_matrix_size().first;
    buffer[1] = (uint8_t)mapping.get_matrix_size().second;

    spi_slave.write_data(buffer, 2);
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_leaf_led, [&](context* ctx) {
    uint8_t buffer[4];
    spi_slave.read_data(buffer, 4);

    Pixel pixel = mapping.get_pixel(buffer[0]);
    PicoLed::Color color = PicoLed::RGB(buffer[1], buffer[2], buffer[3]);

    if (pixel.i2c_address == UNCONFIGUREDADDRESS) return;

    if (pixel.i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
      controller.ledstrip.setPixelColor(pixel.led.first, color);
      controller.ledstrip.setPixelColor(pixel.led.second, color);
      controller.ledstrip.show();
    } else {
      controller.leaf_master.send_slave_message(pixel.i2c_address, {
        (uint8_t)slave_commands::slave_set_leds,
        5,
        { (uint8_t)pixel.led.first, (uint8_t)pixel.led.second, color.red, color.green, color.blue }
      });
    }
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_all, [&](context* ctx){
    uint8_t buffer[3];
    spi_slave.read_data(buffer, 3);
    
    printf("Sending slave message with color: %02x, %02x, %02x\n", buffer[0], buffer[1], buffer[2]);
    controller.leaf_master.send_slave_message(GENCALLADR, {
      (uint8_t)slave_commands::slave_set_all_led,
      3,
      { buffer[0], buffer[1], buffer[2] }
    });

    printf("Setting own led to %i, %i, %i\n", buffer[0], buffer[1], buffer[2]);
    controller.ledstrip.fill(PicoLed::RGB(buffer[0], buffer[1], buffer[2]));
    controller.ledstrip.show();
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_leaf_led, [&](context* ctx){
    uint8_t buffer;
    spi_slave.read_data(&buffer, 1);

    Pixel pixel = mapping.get_pixel(buffer);

    if (pixel.i2c_address == UNCONFIGUREDADDRESS) return;

    if (pixel.i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
      controller.ledstrip.setPixelColor(pixel.led.first, PicoLed::RGB(0, 0, 0));
      controller.ledstrip.setPixelColor(pixel.led.second, PicoLed::RGB(0, 0, 0));
      controller.ledstrip.show();
    } else {
      controller.leaf_master.send_slave_message(pixel.i2c_address, {
        (uint8_t)slave_commands::slave_set_leds,
        5,
        { (uint8_t)pixel.led.first, (uint8_t)pixel.led.second, 0, 0, 0 }
      });
    }
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::clear_all, [&](context* ctx){
    controller.ledstrip.clear();
    controller.ledstrip.show();

    controller.leaf_master.send_slave_message(GENCALLADR, {
      (uint8_t)slave_commands::slave_set_all_led,
      3,
      { 0, 0, 0 }
    });
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::set_random, [&](context* ctx){
    // TODO: implement
  });

  controller.c_command_handler.add_handler((uint8_t)controller_commands::version, [&](context* ctx){
    uint8_t version = VERSION;
    spi_slave.write_data(&version, 1);
  });

  while (true) {
    controller.c_command_handler.handel_command(spi_slave.read_command());
  }
}

int main() {
  stdio_init_all();
  set_alive_led();

  controller.device_discovery();
  controller.topology_discovery();
  mapping.generate_mapping();

  multicore_launch_core1(spi_core);

  PRINT(controller.graph.to_string().c_str());
  PRINT("\n");

  PRINT(controller.graph.node_to_coords().c_str());
  PRINT("\n");

  // I think i²c will run on this core and spi on the other core
  while (true) {
    tight_loop_contents();
  }
}

