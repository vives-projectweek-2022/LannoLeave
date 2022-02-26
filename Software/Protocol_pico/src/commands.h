#pragma once

namespace LannoLeaf {

  typedef enum commands {

    slave_set_i2c_address = 0x01,
    slave_ping,
    slave_reset_mem_counter,
    slave_set_sel_pin,
    slave_set_color

  } commands;

  static struct {
    bool writen = false;
    uint8_t command = 0x00;
    uint8_t buffer[5] = {0};
  } msg_buf;

  static struct {
    uint8_t mem[8];
    uint8_t mem_address;
  } context;

  struct message {
    commands command;
    uint8_t lenght;
    uint8_t data[5];
  };

}
