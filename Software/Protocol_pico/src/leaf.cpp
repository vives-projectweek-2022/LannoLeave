#include "leaf.h"

namespace LannoLeaf {
  
  void Leaf::update(void) {
    
  }

  void Leaf::init(void) {
    gpio_init((uint)SelectPin::UP);
    gpio_init((uint)SelectPin::DOWN);
    gpio_init((uint)SelectPin::LEFT);
    gpio_init((uint)SelectPin::RIGHT);

    gpio_set_dir((uint)SelectPin::UP, GPIO_IN);
    gpio_set_dir((uint)SelectPin::DOWN, GPIO_IN);
    gpio_set_dir((uint)SelectPin::LEFT, GPIO_IN);
    gpio_set_dir((uint)SelectPin::RIGHT, GPIO_IN);
  }

  Leaf::Leaf(uint8_t address) {
    this->address = address;
    init();
  }
  
  Leaf::~Leaf() {
  
  }

} 
