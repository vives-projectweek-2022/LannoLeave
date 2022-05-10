#pragma once

#include <cstddef>
#include <stdint.h>

namespace Lannooleaf::baseclasses {

  class AddressedCommunicator {

    public:
      AddressedCommunicator () {};
      ~AddressedCommunicator() {};

    public:
      virtual void send_data(uint8_t slave_address, const uint8_t* src, size_t len) = 0;

      virtual void receive_data(uint8_t slave_address, uint8_t* dest, size_t len) = 0;
  };

}
