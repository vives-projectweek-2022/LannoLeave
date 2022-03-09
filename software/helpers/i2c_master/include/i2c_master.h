#pragma once

#include <stdint.h>
#include <hardware/i2c.h>
#include <helper_funcs_var.h>
#include <commands.h>

namespace LannoLeaf {

  struct message {
    uint8_t command;
    uint8_t lenght;
    uint8_t data[5];
  };

  class I2CMaster {

    public:
      I2CMaster();
      ~I2CMaster();

    public:
      /** \brief Send a message to slave device
       * \param slave_address 7-bit address of slave to send to
       * \param msg Message to send to slave message struct defined in i2c_master.h*/
      void send_slave_message(uint8_t slave_address, message msg);

      /** \brief Request data from slave device
       * \param slave_address 7-bit address of slave to request data from
       * \param lenght Amount of bytes to request 
       * \note All received bytes are stored in memory attribute*/
      void get_slave_data(uint8_t slave_address, size_t lenght);

      /** \brief Same as get_slave_data but does not send slave_mem_reset message*/
      void get_slave_data_no_mem_reset(uint8_t slave_address, size_t lenght);

    public:
      /** \brief Sets the i2c_instance for master to use
       * \param i2c i2c_instance */
      inline void set_i2c_inst(i2c_inst_t* i2c) { this -> i2c = i2c; }

      /** \brief Resets all readonly memory to 0x00 */
      inline void reset_mem(void) { for (int i = 0; i < 5; i++) _memory[i] = 0x00; }


    public:
      /** \brief Readonly memory of i2c_master*/
      const uint8_t* memory = _memory;

    private:
      i2c_inst_t* i2c;
      uint8_t _memory[10];
  };

}
