#include <stdio.h>
#include <stdint.h>

#include <pico/stdlib.h>

#include <commands.h>
#include <controller.h>
#include <helper_funcs_var.h>

#include <PicoLed.hpp>

using namespace LannoLeaf;

Controller controller(i2c0);

void set_alive_led(void) {
  #ifdef DEBUG
  const uint led_pin = 25;
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, true);
  #endif
}

void add_packet_handlers(void) {

  controller.add_packet_handel(set_led, [&](packet pkt){
    if (pkt.i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
      controller.ledstrip.setPixelColor(pkt.led_num, PicoLed::RGBW(pkt.data[0], pkt.data[1], pkt.data[2], pkt.data[3]));
      controller.ledstrip.show();
    } else {
      controller.leaf_master.send_slave_message(pkt.i2c_address, {
        slave_set_led,
        5,
        { pkt.led_num, pkt.data[0], pkt.data[1], pkt.data[2], pkt.data[3] }
      });
    }
  });

  controller.add_packet_handel(set_all_led, [&](packet pkt){
    if (pkt.i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
      controller.ledstrip.fill(PicoLed::RGBW(pkt.data[0], pkt.data[1], pkt.data[2], pkt.data[3]));
      controller.ledstrip.show();
    } else {
      controller.leaf_master.send_slave_message(pkt.i2c_address, {
        slave_set_all_led,
        4,
        { pkt.data[0], pkt.data[1], pkt.data[2], pkt.data[3] }
      });
    }
  });

  controller.add_packet_handel(set_brightness, [&](packet pkt){
    if (pkt.i2c_address == I2C_CONTOLLER_PLACEHOLDER_ADDRESS) {
      controller.ledstrip.setBrightness(pkt.data[0]);
    } else {
      printf("Sending message to 0x%02x\r\n", pkt.i2c_address);
      controller.leaf_master.send_slave_message(pkt.i2c_address, {
        slave_set_brightness,
        1,
        { pkt.data[0] }
      });
    }
  });

}

int main() {
  stdio_init_all();
  set_alive_led();

  sleep_ms(500);

  controller.device_discovery();
  controller.topology_discovery();

  add_packet_handlers();

  controller.ledstrip.fillRainbow(0, 255 / LED_LENGTH);
  controller.ledstrip.show();

  while (true) {
    if (uart_is_readable(uart0)) {
      uint8_t buffer[7];
      uart_read_blocking(uart0, buffer, 1);
      
      // byte 1: AA start reading
      // byte 2: Lenght of packet
      // byte 3: I2c_address
      // byte 4: Led number
      // byte 5: Command
      // byte 6 - 9: Data (RGBW, Brightness X X X) 

      if (buffer[0] == 0xAA) {
        uart_read_blocking(uart0, buffer, 1);

        uart_read_blocking(uart0, buffer, buffer[0]);
        
        controller.handel_packet({
          buffer[0],
          buffer[1],
          buffer[2],
          { buffer[3], buffer[4], buffer[5], buffer[6] }
        });
      }

      if (buffer[0] == 0x61) {
        uart_write_blocking(uart0, (const uint8_t *) controller.graph.to_string().c_str(), controller.graph.to_string().size());
      }
    }
  }
}
