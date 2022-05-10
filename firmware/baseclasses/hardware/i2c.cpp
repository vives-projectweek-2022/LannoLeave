#include <i2c.hpp>

namespace Lannooleaf::baseclasses {

  I2C::I2C(i2c_inst_t* i2c, uint sda, uint scl, uint frequency) 
  : i2c(i2c), sda(sda), scl(scl) {
    assert(i2c == i2c0 || i2c == i2c1);

    static bool i2c0_init = false;
    static bool i2c1_init = false;

    if (i2c == i2c0 && i2c0_init || i2c == i2c1 && i2c1_init) {
      throw std::runtime_error("ERROR: tried to create a new instance of already in use i2c hardware");
    }

    //? Gpio could be other class to keep track of used pins

    gpio_init(sda);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_pull_up(sda);

    gpio_init(scl);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(scl);

    if (i2c == i2c0) {
      i2c0_init = true;
      i2c_init(i2c0, frequency);
    } else {
      i2c1_init = true;
      i2c_init(i2c1, frequency);
    }
  }

  I2C::~I2C() {
    gpio_init(sda);
    gpio_init(scl);
  
    i2c_deinit(i2c);
  }

}
