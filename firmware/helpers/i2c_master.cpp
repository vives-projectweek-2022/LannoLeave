#include <i2c_master.h>

namespace Lannooleaf {

  I2CMaster::I2CMaster(i2c_inst_t* i2c, uint8_t sda_pin, uint scl_pin) { 
    this->i2c = i2c; 

    gpio_init(sda_pin);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);

    gpio_init(scl_pin);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(scl_pin);

    i2c_init(i2c, BAUDRATE);
  };

  I2CMaster::~I2CMaster() { }

  void I2CMaster::send_data(uint8_t slave_address, uint8_t data[8]) {
    i2c_write_blocking(i2c, slave_address, data, 8, false);
    sleep_ms(SLEEP_TIME);
  }

  std::array<uint8_t, 8> I2CMaster::get_data(uint8_t slave_address) {
    std::array<uint8_t, 8> data;
    i2c_read_blocking(i2c, slave_address, data.data(), data.size(), false);
    sleep_ms(SLEEP_TIME);
    return data;
  }

}
