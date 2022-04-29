#include <i2c_slave.h>

namespace Lannooleaf {

  void I2c_slave::initialize(uint8_t slave_address, i2c_inst_t* i2c, uint sda, uint scl) {
    static bool initialized = false;

    if (!initialized) {
      queue_init(&Get().read_fifo, 1, 200);
      queue_init(&Get().write_fifo, 1, 200);

      initialized = true;
      Get().i2c = i2c;
      Get()._address = slave_address;

      gpio_init(sda);
      gpio_set_function(sda, GPIO_FUNC_I2C);
      gpio_pull_up(sda);

      gpio_init(scl);
      gpio_set_function(scl, GPIO_FUNC_I2C);
      gpio_pull_up(scl);

      i2c_init(i2c, BAUDRATE);
      i2c_slave_init(i2c, slave_address, i2c_irq_callback);
    }
  }

  void __not_in_flash_func(I2c_slave::i2c_irq_callback)(i2c_inst_t* i2c, i2c_slave_event_t event) {
    switch (event) {
      case I2C_SLAVE_RECEIVE: { 
        while (i2c_get_read_available(i2c)) {
          uint8_t byte;
          byte = i2c_get_hw(i2c)->data_cmd;
          queue_add_blocking(&Get().read_fifo, &byte);
        }
        break;
      }

      case I2C_SLAVE_REQUEST: {
        uint8_t byte;
        queue_remove_blocking(&Get().write_fifo, &byte);
        i2c_write_raw_blocking(i2c, &byte, 1);
        break;
      }

      case I2C_SLAVE_FINISH:
        break;

      case I2C_GEN_CALL:
        break;
    }
  }

}
