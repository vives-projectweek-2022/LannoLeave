#pragma once
#include "mbed.h"

namespace LannooLeaf {

  enum Commands {
    SET_SEL_PIN,
    ALIVE_CHECK,
    SET_COLOR,
    SET_BRIGHTNESS
  };

  struct Packet {
    char command;
    char optional = 0x00;
    char optional_1 = 0x00;
    char optional_2 = 0x00;
  };
  

};