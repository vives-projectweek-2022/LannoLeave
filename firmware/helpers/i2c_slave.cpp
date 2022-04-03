#include <i2c_slave.h>

namespace Lannooleaf {

  I2c_slave& I2c_slave::Get(void) {
    static I2c_slave instance;
    return instance;
  }

  void I2c_slave::initialize(uint8_t slave_address, i2c_inst_t* i2c, uint sda, uint scl) {
    static bool initialized = false;

    if (!initialized) {
      initialized = true;
      this->i2c = i2c;
      _address = slave_address;

      gpio_init(sda);
      gpio_set_function(sda, GPIO_FUNC_I2C);
      gpio_pull_up(sda);

      gpio_init(scl);
      gpio_set_function(scl, GPIO_FUNC_I2C);
      gpio_pull_up(scl);

      i2c_init(i2c, 100000);
      i2c_slave_init(i2c, slave_address, i2c_irq_callback);
    }
  }

  void I2c_slave::reassign_address(uint8_t slave_address) {
    i2c_slave_deinit(i2c);
    i2c_slave_init(i2c, slave_address, i2c_irq_callback);
    Get()._address = slave_address;
  }

  void I2c_slave::i2c_irq_callback(i2c_inst_t* i2c, i2c_slave_event_t event) {
    switch (event) {
      case I2C_SLAVE_RECEIVE: { 
        std::array<uint8_t, 8> buffer;

        i2c_read_raw_blocking(i2c, buffer.data(), 8);
        
        I2c_slave::Get().read_fifo.push(buffer);
        break;
      }

      case I2C_SLAVE_REQUEST: {
        const std::array<uint8_t, 8> packet = I2c_slave::Get().write_fifo.front(); 
        
        i2c_write_raw_blocking(i2c, packet.data(), 8);
        
        I2c_slave::Get().write_fifo.pop();
        break;
      }

      case I2C_SLAVE_FINISH:
        break;

      case I2C_GEN_CALL:
        break;
    }
  }

}
