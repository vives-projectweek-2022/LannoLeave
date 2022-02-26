#include "controller.h"

#define SLEEP sleep_ms(20);

namespace LannoLeaf {

  Controller::Controller(uint8_t address, i2c_inst_t * i2c) : Leaf(address, i2c) { }
  
  Controller::~Controller() { }

  void Controller::initialize(void) {
    gpio_init(A);
    gpio_init(B);
    gpio_init(C);
    gpio_init(D);
    gpio_init(E);

    gpio_set_dir(A, GPIO_OUT);
    gpio_set_dir(B, GPIO_OUT);
    gpio_set_dir(C, GPIO_OUT);
    gpio_set_dir(D, GPIO_OUT);
    gpio_set_dir(E, GPIO_OUT);

    // Setting SDA pin
    gpio_init(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);

    // Setting SCL pin
    gpio_init(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    i2c_init(i2c_default, 100 * 1000);
  }

  void Controller::update(void) {
    
  }

  void Controller::search_topology(void) {
    //! Searching for controller neighbors
    for (uint8_t i = A; i <= E; i++) {
      printf("Searching for neighbor on pin: %i\r\n", i);
      gpio_put(i, true);
      assign_new_address();
      SLEEP
      gpio_put(i, false);
    }

    graph.print();
  }

  void Controller::scan_i2c_devices(void) {
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
      if (addr % 16 == 0) { printf("%02x ", addr); }
      int ret;
      uint8_t rxdata;
      
      if (reserved_addr(addr) || addr == 0x00) {
        ret = PICO_ERROR_GENERIC;
      } else {
        ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);
      }
      
      printf(ret < 0 ? "." : "@");
      printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
  }

  void Controller::assign_new_address(void) {
    uint8_t next_address = get_next_available_address();

    printf("Next available add: 0x%02x\r\n", next_address);

    printf("Sending message\r\n");
    send_slave_message(0x00, { 
        slave_set_i2c_address,
        1,
        { next_address }
      });


    printf("Sending ping\r\n");
    send_slave_message(next_address, {
      slave_ping,
      1,
      {2}
    });

    printf("Checking for response\r\n");
    get_slave_data(next_address, 2);

    if (memory[0] == 0xA5 && memory[1] == 0x5A) {
      memory[0] = 0;
      memory[1] = 0;
      graph.add_node(next_address);
      printf("Received response 😁😁😁!!\r\n");
    } else {
      printf("No response\r\n");
    }
    printf("\r\n");
  }

  void Controller::send_slave_message(uint8_t slave_address, message msg) {
    // printf("\r\nSending command to: 0x%02x\r\n", slave_address);
    // printf("cmd: 0x%02x\r\n", msg.command);
    // printf("length: 0x%02x\r\n", msg.lenght);
    // for (int i = 0; i < 5; i++) printf("data%i: 0x%02x\r\n", i, msg.data[i]);

    message data = msg;
    uint8_t cmd_ln[2] = { data.command, data.lenght};
    i2c_write_blocking(i2c, slave_address, cmd_ln, 2, false);
    i2c_write_blocking(i2c, slave_address, data.data, data.lenght, false);

    SLEEP
  }

  void Controller::get_slave_data(uint8_t slave_address, size_t lenght) {
    uint8_t temp;
    for (size_t i = 0; i < lenght; i++) {
      i2c_read_blocking(i2c, slave_address, &temp, 1, false);
      memory[i] = temp;
    }

    SLEEP

    send_slave_message(slave_address, {
      slave_reset_mem_counter,
      0
    });

    SLEEP
  }

  uint8_t Controller::get_next_available_address(void) {
    for (uint8_t addr = 0; addr < (1 << 7); ++addr) {
      if (graph.map.find(addr) == graph.map.end()) {
        if (!reserved_addr(addr)) return addr;
      }
    }
    return (uint8_t)0x08;
  }

}
