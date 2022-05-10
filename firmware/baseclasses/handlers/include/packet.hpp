#pragma once

#include <stdint.h>
#include <vector>

namespace Lannooleaf {

  struct Packet {
    uint8_t command;
    uint8_t checksum;
    uint8_t lenght;
    std::vector<uint8_t> data;
  };

}
