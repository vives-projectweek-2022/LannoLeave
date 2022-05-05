/**
 * @file commands.h
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Enum definitions of commands used by i2c and spi
 * @version 0.1
 * @date 2022-05-04
 * 
 * @copyright Copyright 2022 Joey De Smet
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *         http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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
