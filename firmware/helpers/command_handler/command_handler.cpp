#include <command_handler.h>

namespace Lannooleaf {

  CommandHandler::CommandHandler(context* contx) { this->contx = contx; }

  CommandHandler::~CommandHandler() { }

  void CommandHandler::add_handler(uint8_t cmd, std::function<void(context*)> handler) {
    std::map<uint8_t, std::function<void(context*)>>::iterator itr = handler_map.find(cmd);

    if (itr == handler_map.end()) handler_map[cmd] = handler;
  }

  void CommandHandler::handel_command(uint8_t cmd) {
    std::map<uint8_t, std::function<void(context*)>>::iterator itr = handler_map.find(cmd);

    if (itr != handler_map.end()) handler_map[cmd](contx);
  }

}
