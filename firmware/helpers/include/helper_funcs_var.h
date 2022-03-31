#pragma once

#include <stdint.h>
#include <initializer_list>

#define I2C_CONTOLLER_PLACEHOLDER_ADDRESS 0xFF
#define UNCONFIGUREDADDRESS 0x08
#define GENCALLADR 0x00

#define BAUDRATE 100000

#define LED_PIN 4
#define LED_LENGTH 8

#ifdef DEBUG

#define PRINT(x) printf(x)
#define PRINT_A(x, a) printf(x, a)
#define SLEEP_TIME 20

#else

#define PRINT(x)
#define PRINT_A(x, a)
#define SLEEP_TIME 1

#endif

namespace Lannooleaf {

  enum class side {
    invalid, a, b, c, d, e, f
  };

  enum class select_pins {
    A=6,
    B=5,
    C=28,
    D=18,
    E=16,
    F=15
  };

  static std::initializer_list<select_pins> all_select_pins = { select_pins::A, select_pins::B, select_pins::C, select_pins::D, select_pins::E, select_pins::F };

  bool reserved_addr(uint8_t addr);

  side sel_pin_to_side(select_pins pin);

  side sel_pin_state_to_side(uint8_t state);

  char side_to_char(side side);

}
