#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "src/leaf.h"

#define ADDRESS 0x01

LannoLeaf::Leaf leaf((uint8_t) ADDRESS);

int main() {
  stdio_init_all();

  while (true) {
    leaf.update();
  }
}
