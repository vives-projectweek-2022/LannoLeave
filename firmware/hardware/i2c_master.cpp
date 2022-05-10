#include <i2c_master.hpp>

namespace Lannooleaf {

  void I2CMaster::send_data(uint8_t slave_address, const uint8_t* src, size_t len) {
    i2c_write_blocking(i2c, slave_address, src, len, false);
  }

  void I2CMaster::receive_data(uint8_t slave_address, uint8_t* dest, size_t len) {
    i2c_read_blocking(i2c, slave_address, dest, len, false);
  }

}
