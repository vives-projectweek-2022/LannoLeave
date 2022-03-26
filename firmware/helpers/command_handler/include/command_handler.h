#pragma once

#include <map>
#include <stdint.h>
#include <functional>

namespace LannoLeaf {

  struct read_memory {
    bool writen = false;
    uint8_t command = 0;
    uint8_t memory[16] = {0};
  };
  
  struct write_memory {
    uint8_t memory[16];
    uint8_t memory_address = 0;
  };
  
  struct context {
    read_memory* read_mem;
    write_memory* write_mem;    
  };

  class CommandHandler {

    public:
      CommandHandler(context* contx);
      ~CommandHandler();

    public:
      void add_handler(uint8_t cmd, std::function<void(context*)> handler);
      void handel_command(uint8_t cmd);

    private:
      std::map<uint8_t, std::function<void(context*)>> handler_map;
      context* contx;

  };

}
