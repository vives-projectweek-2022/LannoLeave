#pragma once

namespace Lannooleaf {

  enum class slave_commands {

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

  };

  enum class controller_commands {

    get_matrix_size = 0x01,

    set_leaf_led,
    set_all,

    clear_leaf_led,
    clear_all,

    set_random,

    version

  };

}
