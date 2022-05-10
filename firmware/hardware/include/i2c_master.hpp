#pragma once

#include <i2c.hpp>
#include <addressed_communicator.hpp>

namespace Lannooleaf {

  class I2CMaster : public baseclasses::I2C, public baseclasses::AddressedCommunicator {

    public:
      I2CMaster (i2c_inst_t* i2c, uint sda, uint scl, uint frequency) 
      : I2C(i2c, sda, scl, frequency) {}
      
      ~I2CMaster() {}

    public:
      virtual void send_data(uint8_t slave_address, const uint8_t* src, size_t len);

      virtual void receive_data(uint8_t slave_address, uint8_t* dest, size_t len);

  };

}
