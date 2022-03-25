#include "include/helper_funcs_var.h"
#include <graph.h>

namespace LannoLeaf {

  bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78 || addr == 0x08;
  }

  side sel_pin_to_side(select_pins pin) {
    switch (pin) {
    case select_pins::A:
      return side::a;

    case select_pins::B:
      return side::b;

    case select_pins::C:
      return side::c;

    case select_pins::D:
      return side::d;

    case select_pins::E:
      return side::e;

    case select_pins::F:
      return side::f;

    default:
      return side::invalid;
    }
  }

  side sel_pin_state_to_side(uint8_t state) {
    switch (state) {
    case 0b00000001:
      return side::a;

    case 0b00000010:
      return side::b;

    case 0b00000100:
      return side::c;

    case 0b00001000:
      return side::d;

    case 0b00010000:
      return side::e;

    case 0b00100000:
      return side::f;

    default:
      return side::invalid;
    }
  }

  char side_to_char(side side) {
    switch (side){
    case side::a:
      return 'a';

    case side::b:
      return 'b';
    
    case side::c:
      return 'c';

    case side::d:
      return 'd';

    case side::e:
      return 'e';

    case side::f:
      return 'f';

    default:
      return 'X';
    }
  }

  coordinate side_to_addition_coordinate(side side) {
    switch (side){
    case side::a:
      return coordinate(0, 2);

    case side::b:
      return coordinate(1, 1);
    
    case side::c:
      return coordinate(1, -1);

    case side::d:
      return coordinate(0, -2);

    case side::e:
      return coordinate(-1, -1);

    case side::f:
      return coordinate(-1, 1);

    default:
      return coordinate(0, 0);
    }
  }

}
