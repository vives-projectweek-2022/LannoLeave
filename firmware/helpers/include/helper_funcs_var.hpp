/**
 * @file helper_funcs_var.h
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief # Defines and helper functions used in Lannooleaf project
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

#include <initializer_list>

#include <stdint.h>
#include <pico/stdlib.h>

#include <PicoLed.hpp>

#define I2C_CONTOLLER_PLACEHOLDER_ADDRESS 0xFF
#define UNCONFIGUREDADDRESS 0x08
#define GENCALLADR 0x00

#define BAUDRATE 1000000

#define LED_PIN 4u
#define LED_LENGTH 16u

#define VERSION 3

#define SDA 8u
#define SCL 9u
#define MOSI 0u
#define MISO 3u
#define CLK 2u
#define CS 1u

#ifdef DEBUG

#define PRINT(x) printf(x)
#define PRINT_A(x, a) printf(x, a)
#define SLEEP_TIME 10

#else

#define PRINT(x)
#define PRINT_A(x, a)
#define SLEEP_TIME 1

#endif

namespace Lannooleaf {

  /**
   * @brief A stuct to hold RGB value
   */
  struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  };

  /**
   * @brief enum of sides
   */
  enum class side {
    invalid, a, b, c, d, e, f
  };

  /**
   * @brief enum of select_pins ri pi pico GPIO pins
   */
  enum class select_pins {
    A=6,
    B=5,
    C=28,
    D=18,
    E=16,
    F=15
  };

  /**
   * @brief initializer_list of all select_pins to be able to use range based for loop
   */
  static std::initializer_list<select_pins> all_select_pins = { select_pins::A, select_pins::B, select_pins::C, select_pins::D, select_pins::E, select_pins::F };

  /**
   * @brief Set the alive led of ri pi pico
   */
  void set_alive_led(void);

  [[noreturn]] void error_blink(void);

  /**
   * @brief Plays a discovery animation
   * 
   * @param ledstrip 
   * pointer to PicoLedController to use
   * @param color 
   * Color to display animation
   */
  void discover_animation(PicoLed::PicoLedController* ledstrip, Color color);

  /**
   * @brief Check is address is a reserved address
   * 
   * @param addr 
   * address to check
   * @return true 
   * address is a reserved address
   * @return false 
   * address can be used
   */
  bool reserved_addr(uint8_t addr);

  /**
   * @brief Conversion from select_pins to side
   * 
   * @param pin 
   * select_pins to convert
   * @return side 
   * side representation of select_pins
   */
  side sel_pin_to_side(select_pins pin);

  /**
   * @brief Conversion from uint8_t to side
   * 
   * @param state 
   * uint8_t to convert
   * @return side 
   * side representation of uint8_t
   */
  side sel_pin_state_to_side(uint8_t state);

  /**
   * @brief Conversion from side to char
   * 
   * @param side 
   * side to convert
   * @return char 
   * char representation
   */
  char side_to_char(side side);

}
