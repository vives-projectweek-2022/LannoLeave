#pragma once

namespace LannoLeaf {

  typedef enum commands {

    slave_set_i2c_address = 0x01,
    slave_ping,
    slave_reset_mem_counter,
    slave_set_sel_pin,
    slave_get_sel_pin,
    slave_set_color,
    slave_reset,
    slave_is_neighbor,
    slave_neighbor_size

  } commands;

}
