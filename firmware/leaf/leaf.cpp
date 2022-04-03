#include <leaf.h>

namespace Lannooleaf {

  Leaf::Leaf() {}

  Leaf::~Leaf() { }

  void Leaf::update(void) { 
    update_sel_status();
  } 

  void Leaf::update_sel_status(void) {
    _sel_pin_status = 0x00;
    uint8_t i = 0;

    for (select_pins pin : all_select_pins) {
      if (!gpio_is_dir_out((uint)pin) && gpio_get((uint)pin)) _sel_pin_status |= 1 << i;
      i++;
    }
  }

}
