#include <leaf.h>

namespace LannoLeaf {

  Leaf::Leaf(uint8_t address, i2c_inst_t * i2c) {
    _address = address;
    this -> i2c = i2c;
    initialize();
  }

  Leaf::~Leaf() { }

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

  void Leaf::add_command_handel(uint8_t command, std::function<void(context*, msg_buff*)> handler) {
    std::map<uint8_t, std::function<void(context*, msg_buff*)>>::iterator itr = handlers.find(command);

    if (itr == handlers.end()) {
      handlers[command] = handler;
    }
  }

  void Leaf::reset(void) {
    _configured = false;
    _slave_initialized = false;
    _address = UNCONFIGUREDADDRESS;
    i2c_slave_deinit(i2c);
  }

  void Leaf::initialize(void) {
    for (select_pins pin : all_select_pins) {
      gpio_init(pin);
      gpio_set_dir(pin, GPIO_IN);
    }

    gpio_init(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);

    gpio_init(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    i2c_init(i2c, BAUDRATE);
  }

  void Leaf::handle_data(void) {
    if (msg_buf.writen) {
      std::map<uint8_t, std::function<void(context*, msg_buff*)>>::iterator itr = handlers.find(msg_buf.command);

      if (itr != handlers.end()) handlers[msg_buf.command](&_context, &msg_buf);
      
      msg_buf.writen = false;
    }
  }

  void Leaf::update_sel_status(void) {
    _sel_pin_status = 0x00;

    for (uint8_t i = A, j = 0; i <= E; i++, j++) {
      if (!gpio_is_dir_out(i) && gpio_get(i)) _sel_pin_status |= 1 << j;
    }
  }

  static void i2c_slave_handler(i2c_inst_t * i2c, i2c_slave_event_t event) {
    switch (event) {
    case I2C_SLAVE_RECEIVE: {      
      uint8_t receive_buffer[2];

      i2c_read_raw_blocking(i2c, receive_buffer, 2);
      i2c_read_raw_blocking(i2c, msg_buf.buffer, receive_buffer[1]);
      
      msg_buf.command = receive_buffer[0];
      msg_buf.writen = true;
      break;
    }

    case I2C_SLAVE_REQUEST: {
      while (!i2c_get_write_available(i2c));

      i2c_write_byte(i2c, _context.mem[_context.mem_address]);
      
      _context.mem_address++;
      break;
    }

    case I2C_SLAVE_FINISH:
      break;

    case I2C_GEN_CALL:
      break;
    }
  }

}
