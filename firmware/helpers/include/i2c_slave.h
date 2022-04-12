#pragma once

#include <queue>
#include <array>
#include <stdint.h>

#include <helper_funcs_var.h>

#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include <i2c_slave_lib.h>

namespace Lannooleaf {

  class I2c_slave {
    
    public:
      static void initialize(uint8_t slave_address, i2c_inst_t* i2c, uint sda, uint scl);

      static uint8_t pop(void) {
        uint8_t value = Get().read_fifo.front();
        Get().read_fifo.pop();
        return value;
      }

      static u_int8_t address(void) {
        return Get()._address;
      }

      static void address(uint8_t address) {
        printf("Reinitializong slave\n");
        i2c_slave_deinit(Get().i2c);
        i2c_slave_init(Get().i2c, address, i2c_irq_callback);
        Get()._address = address;
      }

      static void push(uint8_t byte) {
        Get().write_fifo.push(byte);
      }

      static bool empty(void) {
        return Get().read_fifo.empty();
      }

    private:
      static I2c_slave& Get(void) {
        static I2c_slave instance;
        return instance;
      };

    private:
      static void i2c_irq_callback(i2c_inst_t* i2c, i2c_slave_event_t event);
    
    private:
      std::queue<uint8_t> read_fifo;
      std::queue<uint8_t> write_fifo;
    
    private:
      i2c_inst_t* i2c;
      uint8_t _address;

    private:
      I2c_slave() {}
      ~I2c_slave(){}

    private:
      I2c_slave(I2c_slave const&)      = delete;
      void operator=(I2c_slave const&) = delete;

  };

}
