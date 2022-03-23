#include "include/helper_funcs_var.h"

namespace LannoLeaf {

  bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78 || addr == 0x08;
  }

  side sel_pin_to_side(select_pins pin) {
    switch (pin) {
    case A:
      return a;

    case B:
      return b;

    case C:
      return c;

    case D:
      return d;

    case E:
      return e;

    default:
      return invalid;
    }
  }

  side sel_pin_state_to_side(uint8_t state) {
    switch (state) {
    case 0x01:
      return a;

    case 0x02:
      return b;

    case 0x04:
      return c;

    case 0x08:
      return d;

    case 0x10:
      return e;

    default:
      return invalid;
    }
  }

  char side_to_char(side side) {
    switch (side){
    case a:
      return 'a';

    case b:
      return 'b';
    
    case c:
      return 'c';

    case d:
      return 'd';

    case e:
      return 'e';

    default:
      return 'X';
    }
  }

}
