#include <leaf.h>

namespace Lannooleaf {

  Leaf::Leaf(uint8_t address, i2c_inst_t * i2c, uint sda_pin, uint scl_pin):
  l_command_handler(&l_context) {
    _address = address;
    this -> i2c = i2c;
    
    for (select_pins pin : all_select_pins) {
      gpio_init((uint)pin);
      gpio_set_dir((uint)pin, GPIO_IN);
    }

    gpio_init(sda_pin);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);

    gpio_init(scl_pin);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(scl_pin);

    i2c_init(i2c, BAUDRATE);
  }

  Leaf::~Leaf() { }

  void Leaf::update(void) { 
    update_sel_status();
    if (l_read_mem.writen) {
      printf("0x%02x\n", l_read_mem.command);
      l_command_handler.handel_command(l_read_mem.command);
    } 
  } 

  void Leaf::slave_init(void) {
    i2c_slave_init(i2c, _address, i2c_slave_handler);
    _slave_initialized = true;
  }

  void Leaf::address(uint8_t address) {
    _address = address;
    if (_slave_initialized) i2c_slave_deinit(i2c);
    i2c_slave_init(i2c, _address, i2c_slave_handler);
    _configured = true;
  }

  void Leaf::reset(void) {
    _configured = false;
    _slave_initialized = false;
    _address = UNCONFIGUREDADDRESS;
    i2c_slave_deinit(i2c);
  }

  void Leaf::update_sel_status(void) {
    _sel_pin_status = 0x00;
    uint8_t i = 0;

    for (select_pins pin : all_select_pins) {
      if (!gpio_is_dir_out((uint)i) && gpio_get((uint)pin)) _sel_pin_status |= 1 << i;
      i++;
    }
  }

  static void i2c_slave_handler(i2c_inst_t * i2c, i2c_slave_event_t event) {
    switch (event) {
    case I2C_SLAVE_RECEIVE: { 
      uint8_t receive_buffer[2];

      i2c_read_raw_blocking(i2c, receive_buffer, 2);
      i2c_read_raw_blocking(i2c, l_read_mem.memory, receive_buffer[1]);
      
      l_read_mem.command = receive_buffer[0];
      l_read_mem.writen = true;
      break;
    }

    case I2C_SLAVE_REQUEST: {
        while (!i2c_get_write_available(i2c)) tight_loop_contents();
        i2c_write_byte(i2c, l_write_mem.memory[l_write_mem.memory_address]);
        l_write_mem.memory_address++;
      break;
    }

    case I2C_SLAVE_FINISH:
      break;

    case I2C_GEN_CALL:
      break;
    }
  }

}
