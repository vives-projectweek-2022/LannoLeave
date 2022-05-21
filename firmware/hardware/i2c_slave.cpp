#include <i2c_slave.hpp>

namespace Lannooleaf {

  I2CSlave::I2CSlave(uint8_t slave_address, i2c_inst_t* i2c, uint sda, uint scl, uint frequency) 
  : I2C(i2c, sda, scl, frequency), _address(slave_address) {
        
    if (i2c == i2c0) {
      s0_read_fifo  = &this->read_fifo;
      s0_write_fifo = &this->write_fifo;

      i2c_slave_init(i2c, slave_address, i2c0_irq_callback);

    } else {
      // Todo initialze i2c1 slave
      throw std::runtime_error("i2c1 is not implemented yet");
    }
  }

  I2CSlave::~I2CSlave() {
    i2c_slave_deinit(i2c);
  }

  void I2CSlave::address(uint8_t address) {
    i2c_slave_deinit(i2c);
    i2c_slave_init(i2c, address, i2c0_irq_callback);
    _address = address;
  }

  void __not_in_flash_func(I2CSlave::i2c0_irq_callback)(i2c_inst_t* i2c, i2c_slave_event_t event) {
    switch (event) {
      [[likely]] case I2C_SLAVE_RECEIVE: { 
        while (i2c_get_read_available(i2c)) {
          uint8_t byte;
          byte = i2c_get_hw(i2c)->data_cmd;
          queue_try_add(I2CSlave::s0_read_fifo, &byte);
        }
        break;
      }

      case I2C_SLAVE_REQUEST: {
        uint8_t byte;
        queue_try_remove(I2CSlave::s0_write_fifo, &byte);
        i2c_write_raw_blocking(i2c, &byte, 1);
        break;
      }

      case I2C_SLAVE_FINISH:
        break;

      case I2C_GEN_CALL:
        break;
    }
  };

}
