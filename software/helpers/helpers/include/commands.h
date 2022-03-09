#pragma once

namespace LannoLeaf {

  typedef enum commands {

    slave_set_i2c_address = 0x01,
    slave_ping,
    slave_reset_mem_counter,
    slave_set_sel_pin,
    slave_get_sel_pin,
    slave_reset,
    slave_is_neighbor,
    slave_neighbor_size,
    slave_set_led,
    slave_set_all_led,
    slave_set_brightness

  } commands;

  typedef enum bl_commands {

    set_led,
    set_all_led,
    set_brightness

  } bl_commands;

  struct packet {
    uint8_t i2c_address;
    uint8_t led_num;
    uint8_t command;
    uint8_t data[4];
  };

}
