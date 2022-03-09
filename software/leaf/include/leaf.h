#pragma once

#include <stdio.h>
#include <stdint.h>
#include <map>
#include <functional>

#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <i2c_slave.h>
#include <i2c_fifo.h>

#include <commands.h>
#include <helper_funcs_var.h>

namespace LannoLeaf {

  /** \brief Storage struct where master writes to*/
  struct msg_buff {
    bool writen = false;
    uint8_t command = 0x00;
    uint8_t buffer[5] = {0};
  };

  /** \brief  Storage stuct where master reads from*/
  struct context {
    uint8_t mem[11];
    uint8_t mem_address = 0;
  };

  static context _context;
  static msg_buff msg_buf;

  class Leaf {

    public:
      Leaf(uint8_t address, i2c_inst_t * i2c);
      ~Leaf();
  
    public:
      /** \brief Updates the select pin status and handels data */
      inline void update(void) { update_sel_status(); handle_data(); } 
      
    public:
      /** \brief Initializes slave and start listening on the i²c bus */
      void slave_init(void);

      /** \brief Assing a new i²c address to slave 
      * \param address The new i²c slave address  */
      void address(uint8_t address);

      /** \brief Adds a command handler for i²c slave
      * \param command Command wich slave needs to handel defined in command.h
      * \param handler Function to execute */
      void add_command_handel(uint8_t command, std::function<void(context*, msg_buff*)> handler);

      /** \brief Resets slave to default configuration*/
      void reset(void);

    public:
      /** \returns Slave i²c address */
      inline uint8_t& address(void) { return _address; }

      /** \returns Slave select pin status as uint8_t */
      inline uint8_t& sel_pin_status(void) { return _sel_pin_status; }

    public:
      /** \returns true if slave is initialized, fase is not*/
      inline bool& slave_initialized(void) { return _slave_initialized; }

      /** \returns true if slave has been configured with a new i²c address, fase if not */
      inline bool& configured(void) { return _configured; }

    private:
      void initialize(void);
      void handle_data(void);
      void update_sel_status(void);

    private:
      uint8_t _sel_pin_status = 0x00;

    private:
      bool _slave_initialized = false;
      bool _configured = false;

    private:
      i2c_inst_t * i2c;
      uint8_t _address;
      std::map<uint8_t, std::function<void(context*, msg_buff*)>> handlers;

  };

  static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event);

}
