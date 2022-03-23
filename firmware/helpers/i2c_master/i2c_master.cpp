#include <i2c_master.h>
#include <stdio.h>

namespace LannoLeaf {

  I2CMaster::I2CMaster() { }

  I2CMaster::~I2CMaster() { }

  void I2CMaster::send_slave_message(uint8_t slave_address, message msg) {
    message data = msg;
    uint8_t cmd_ln[2] = {
      data.command,
      data.lenght
    };

    i2c_write_blocking(i2c, slave_address, cmd_ln, 2, false);
    i2c_write_blocking(i2c, slave_address, data.data, data.lenght, false);

    sleep_ms(SLEEP_TIME);
  }

  void I2CMaster::get_slave_data(uint8_t slave_address, size_t lenght) {
    uint8_t temp;
    for (size_t i = 0; i < lenght; i++) {
      i2c_read_blocking(i2c, slave_address, & temp, 1, false);
      _memory[i] = temp;
    }

    sleep_ms(SLEEP_TIME);

    send_slave_message(slave_address, {
      slave_reset_mem_counter,
      0,
      { }
    });

    sleep_ms(SLEEP_TIME);
  }

  void I2CMaster::get_slave_data_no_mem_reset(uint8_t slave_address, size_t lenght) {
    uint8_t temp;
    for (size_t i = 0; i < lenght; i++) {
      i2c_read_blocking(i2c, slave_address, & temp, 1, false);
      _memory[i] = temp;
    }

    sleep_ms(SLEEP_TIME);
  }

}
