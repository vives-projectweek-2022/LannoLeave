#pragma once

#include <stdint.h>
#include <initializer_list>

#define UNCONFIGUREDADDRESS 0x08
#define GENCALLADR 0x00
#define BAUDRATE 100000

#ifdef DEBUG

#define PRINT(x) printf(x)
#define PRINT_A(x, a) printf(x, a)
#define SLEEP_TIME 100

#else

#define PRINT(x)
#define PRINT_A(x, a)
#define SLEEP_TIME 2

#endif

namespace LannoLeaf {

  typedef enum side {
    invalid, a, b, c, d, e,
  } side;

  typedef enum select_pins {
    A=16,
    B,
    C,
    D,
    E
  } select_pins;

  static std::initializer_list all_select_pins = { A, B, C, D, E };

  bool reserved_addr(uint8_t addr);

  side sel_pin_to_side(select_pins pin);

  side sel_pin_state_to_side(uint8_t state);

  char side_to_char(side side);

}
