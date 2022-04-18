#pragma once

namespace Lannooleaf {

  /** \brief Enum class of i2c slave commands*/
  enum class slave_commands {

    set_i2c_address = 0x01,
    ping,
    
    set_sel_pin,
    get_sel_pin,
    
    is_neighbor,
    get_neigbor_size,
    get_neighbor_information,
    
    set_led,
    set_all_led,

    set_led_string,

    discovery_done,

    reset
  };

  /** \brief Enum class of i2c master commands*/
  enum class controller_commands {

    hello_message = 0x01,

    get_adj_list_size,
    get_adj_list,

    set_leaf_led,
    set_all,

    set_led_string,

    clear_leaf_led,
    clear_all,

    set_random,

  };

}
