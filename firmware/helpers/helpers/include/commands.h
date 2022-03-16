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

  } commands;

  typedef enum bl_commands {

    send_adj_list,

    set_leaf_led,

    set_leaf_all,
    set_unit_all,
    set_all_all,

    clear_leaf,
    clear_unit,
    clear_all,

    set_random

  } bl_commands;

  struct packet {
    
  };

}
