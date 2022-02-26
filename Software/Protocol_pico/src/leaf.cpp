#include "leaf.h"

namespace LannoLeaf {

  Leaf::Leaf(uint8_t address, i2c_inst_t * i2c) { 
    _address = address; 
    this->i2c = i2c;
  }
  
  Leaf::~Leaf() { }

  void Leaf::initialize(void) {
    gpio_init(A);
    gpio_init(B);
    gpio_init(C);
    gpio_init(D);
    gpio_init(E);

    gpio_set_dir(A, GPIO_IN);
    gpio_set_dir(B, GPIO_IN);
    gpio_set_dir(C, GPIO_IN);
    gpio_set_dir(D, GPIO_IN);
    gpio_set_dir(E, GPIO_IN);

    gpio_init(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);

    gpio_init(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  
    i2c_init(i2c, 100 * 1000);
  }

  void Leaf::update(void) {
    update_sel_status();
    gpio_put(25, true);

    if (sel_pin_status) {
      if (!slave_initialized) {
        i2c_slave_init(i2c, _address, i2c_slave_handler);
        slave_initialized = true;
        printf("Slave initialized 🎉🎉!!\r\n");
      }
      gpio_put(25, false);
      handle_data();
    } 
  }

  bool Leaf::reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78 || addr == 0x08;
  }

  void Leaf::address(uint8_t address) {
    printf("Assigning address 0x%02x \r\n");
    _address = address;
    i2c_slave_deinit(i2c);
    i2c_slave_init(i2c, _address, i2c_slave_handler);
  }

  void Leaf::update_sel_status(void) {
    sel_pin_status = 0x00;

    for (uint8_t i = A, j = 0; i <= E; i++, j++) {
      if (!gpio_is_dir_out(i) && gpio_get(i)) sel_pin_status |= 1 << j;
    }
  }

  void Leaf::handle_data(void) {
    if (msg_buf.writen) {
      switch ((commands)msg_buf.command) {
        case slave_set_i2c_address:
          if (_address != 0x08) break;
          address(msg_buf.buffer[0]);
          break;

        case slave_ping:
          printf("Received ping\r\n");
          context.mem[0] = 0xA5;
          context.mem[1] = 0x5A;
          break;

        case slave_reset_mem_counter:
          context.mem_address = 0;
          break;

        case slave_set_sel_pin:
          printf("Setting sel pin %i to %s\r\n", msg_buf.buffer[0], msg_buf.buffer[1] ? "High" : "Low");
          if(gpio_is_dir_out(msg_buf.buffer[0]) && !msg_buf.buffer[1]) {
            gpio_put(msg_buf.buffer[0], false);
            gpio_set_dir(msg_buf.buffer[0], GPIO_IN);
          } else {
            gpio_set_dir(msg_buf.buffer[0], GPIO_OUT);
            gpio_put(msg_buf.buffer[0], true);
          }
          break;

        default:
          break;
      }

      msg_buf.writen = false;
    }
  }

  static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {
      case I2C_SLAVE_RECEIVE:
        uint8_t buf[2];
        i2c_read_raw_blocking(i2c, buf, 2);
        i2c_read_raw_blocking(i2c, msg_buf.buffer, buf[1]);
        msg_buf.command = (commands)buf[0];
        msg_buf.writen = true;
        break;

      case I2C_SLAVE_REQUEST:
        while (!i2c_get_write_available(i2c));
        i2c_write_byte(i2c, context.mem[context.mem_address]);
        context.mem_address++;
        break;

      case I2C_SLAVE_FINISH:
        break;

      case I2C_GEN_CALL:
        break;
    }
  }
} 
