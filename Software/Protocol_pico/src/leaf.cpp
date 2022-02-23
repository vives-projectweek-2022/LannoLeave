#include "leaf.h"

namespace LannoLeaf {
  
  Leaf::Leaf(uint8_t address) { this->address = address; }
  
  Leaf::~Leaf() { }

  void Leaf::initialize(void) {
    gpio_init((uint)SelectPin::ONE);
    gpio_init((uint)SelectPin::TWO);
    gpio_init((uint)SelectPin::THREE);
    gpio_init((uint)SelectPin::FOUR);
    gpio_init((uint)SelectPin::FIVE);

    gpio_set_dir((uint)SelectPin::ONE, GPIO_IN);
    gpio_set_dir((uint)SelectPin::TWO, GPIO_IN);
    gpio_set_dir((uint)SelectPin::THREE, GPIO_IN);
    gpio_set_dir((uint)SelectPin::FOUR, GPIO_IN);
    gpio_set_dir((uint)SelectPin::FIVE, GPIO_IN);

    gpio_init(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);

    gpio_init(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  
    i2c_init(i2c0, 100 * 1000);

    i2c_slave_init(i2c0, address, &i2c_slave_handler);
  }

  void Leaf::update(void) {
    // TODO: add code
  }

  bool Leaf::reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
  }

  static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {
      // Master writing data
      case I2C_SLAVE_RECEIVE:
        // TODO: Handle command received from master
        break;

      // Master requesting data
      case I2C_SLAVE_REQUEST:
        // TODO: Handle sending data to master
        break;

      // Master Stop / Restart signal
      case I2C_SLAVE_FINISH:
        // TODO: Add code
        break;
    }
  }
} 
