#pragma once

#include <queue>
#include <array>
#include <stdint.h>

#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include <i2c_slave_lib.h>

namespace Lannooleaf {

  struct Packet {
    uint8_t read_buffer[8];
  };

  class I2c_slave {

    private:
      I2c_slave(){};
      ~I2c_slave(){};

    public:
      I2c_slave(I2c_slave const&) = delete;
      void operator=(I2c_slave const&) = delete;

    public:
      static I2c_slave& Get(void);
      static uint8_t address(void) { return Get()._address; };

    public:
      std::queue<std::array<uint8_t, 8>> read_fifo;
      std::queue<std::array<uint8_t, 8>> write_fifo;

    public:
      void initialize(uint8_t slave_address, i2c_inst_t* i2c, uint sda, uint scl);
      void reassign_address(uint8_t slave_address);

    private:
      i2c_inst_t* i2c;
      u_int8_t _address;

    private:
      static void i2c_irq_callback(i2c_inst_t* i2c, i2c_slave_event_t event);

  };

}
