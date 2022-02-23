#include "controller.h"

namespace LannoLeaf {

  Controller::Controller() {
    init();
  }
  
  Controller::~Controller() {
  }

  void Controller::init(void) {
    gpio_init((uint)SelectPin::ONE);
    gpio_init((uint)SelectPin::TWO);
    gpio_init((uint)SelectPin::THREE);
    gpio_init((uint)SelectPin::FOUR);
    gpio_init((uint)SelectPin::FIVE);

    gpio_set_dir((uint)SelectPin::ONE, GPIO_OUT);
    gpio_set_dir((uint)SelectPin::TWO, GPIO_OUT);
    gpio_set_dir((uint)SelectPin::THREE, GPIO_OUT);
    gpio_set_dir((uint)SelectPin::FOUR, GPIO_OUT);
    gpio_set_dir((uint)SelectPin::FIVE, GPIO_OUT);

    gpio_init(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);

    gpio_init(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    i2c_init(i2c_default, 100 * 1000);
  }

  void Controller::scan_i2c_devices(void) {
    uint8_t dummy_data = 0xA6;
    int ret;

    for (uint8_t add = 0; add < (1 << 7); ++add) {
      if (!reserved_addr(add)) {
        ret = i2c_write_blocking(i2c0, add, &dummy_data, 1, false);
        if (ret > 0) {
          connected_devices.push_back(add);
        }
      }
    }
  }

  bool Controller::reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
  }

  void Controller::send_slave_message(uint8_t slave_address, commands cmd) {
    uint8_t data = cmd;
    i2c_write_blocking(i2c_default, slave_address, &data, 1, false);

    // size_t response_leght = 1;

    // // TODO: Set the right response lenght
    // switch (cmd) {
    //   case slave_detect_message:
    //     response_leght = 1;
    //     break;
    
    //   case slave_set_sel_pin:
    //     response_leght = 1;
    //     break;

    //   case slave_set_color:
    //     response_leght = 1;
    //     break;

    // }

    // i2c_read_blocking(i2c_default, slave_address, response_buffer, 1 + response_leght, false);
  }

}
