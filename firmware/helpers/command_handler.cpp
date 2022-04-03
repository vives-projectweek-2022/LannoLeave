#include <command_handler.h>

namespace Lannooleaf {

  CommandHandler::CommandHandler() {  }

  CommandHandler::~CommandHandler() { }

  void CommandHandler::add_handler(uint8_t cmd, std::function<void(void)> handler) {
    std::unordered_map<uint8_t, std::function<void(void)>>::iterator itr = handler_map.find(cmd);

    if (itr == handler_map.end()) handler_map[cmd] = handler;
  }

  void CommandHandler::handel_command(uint8_t cmd) {
    std::unordered_map<uint8_t, std::function<void(void)>>::iterator itr = handler_map.find(cmd);

    if (itr != handler_map.end()) handler_map[cmd]();
  }

}
