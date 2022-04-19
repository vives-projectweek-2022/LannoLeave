#pragma once

#include <array>
#include <stdio.h>
#include <stdint.h>

#include <commands.h>
#include <helper_funcs_var.h>

#include <pico/stdlib.h>
#include <hardware/i2c.h>

namespace Lannooleaf {

  class I2CMaster {

    public:
      I2CMaster(i2c_inst_t* i2c, uint8_t sda_pin, uint scl_pin);
      ~I2CMaster();

    public:
      /** \brief Send data to i2c slave*/
      void send_data(uint8_t slave_address, const uint8_t* src, size_t len);
      /** \brief Receive data from i2c slave*/
      void get_data(uint8_t slave_address, uint8_t* dest, size_t len);

    private:
      i2c_inst_t* i2c;
  };

}
