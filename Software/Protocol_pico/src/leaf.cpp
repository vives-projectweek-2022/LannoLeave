#include "leaf.h"

namespace LannoLeaf {
  
  void Leaf::update(void) {
    // TODO: I2C slave
  }

  static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {

    // master writing data
    case I2C_SLAVE_RECEIVE:
      printf("%x\r\n", i2c_read_byte(i2c));
      break;

    // Master requesting data
    case I2C_SLAVE_REQUEST:
        
      break;

    case I2C_SLAVE_FINISH:
      break;

    default:
      break;
    }
}

  void Leaf::init(void) {
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

  bool Leaf::reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
  }

  Leaf::Leaf(uint8_t address) {
    this->address = address;
    init();
  }
  
  Leaf::~Leaf() {
  
  }

} 
