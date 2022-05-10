/**
 * @file command_handler.cpp
 * @author Joey De Smet (Joey@de-smet.org)
 * @brief Source file to command_handler.h
 * @version 0.1
 * @date 2022-05-04
 * 
 * @copyright Copyright 2022 Joey De Smet
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *         http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "packet_handler.hpp"

namespace Lannooleaf {

  PacketHandler::PacketHandler() {  }

  PacketHandler::~PacketHandler() { }

  void PacketHandler::add_handler(uint8_t cmd, std::function<void(Packet* pkt)> handler) {
    std::unordered_map<uint8_t, std::function<void(Packet* pkt)>>::iterator itr = handler_map.find(cmd);

    if (itr == handler_map.end()) handler_map[cmd] = handler;
  }

  void PacketHandler::handel_packet(Packet* pkt) {
    std::unordered_map<uint8_t, std::function<void(Packet* pkt)>>::iterator itr = handler_map.find(pkt->command);

    if (itr != handler_map.end()) {
      if (pkt->data.size() != 0 && checksum(pkt->data.data(), pkt->data.size()) != pkt->checksum) 
        throw std::runtime_error("Packet Drop: checksum failed!!\n");

      handler_map[pkt->command](pkt);
    } 
    else 
      throw std::runtime_error("Packet Drop: Did not find requested command!!\n");
    
  }

  unsigned char PacketHandler::checksum(unsigned char *data, unsigned char lenght) {
    unsigned char sum = 0;

    do {
      sum += *data++;
    } while (--lenght);

    return sum & 0xff;
  }

}
