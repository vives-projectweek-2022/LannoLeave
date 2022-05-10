#include <buffered_communicator.hpp>

namespace Lannooleaf::baseclasses  {

  BufferedCommunicator::BufferedCommunicator() {
    queue_init(&read_fifo, 1, 100);
    queue_init(&write_fifo, 1, 100);
  }

  uint8_t BufferedCommunicator::read_byte(void) {
    uint8_t value;
    queue_remove_blocking(&read_fifo, &value);
    return value;
  }

  void BufferedCommunicator::write_byte(uint8_t byte) {
    queue_add_blocking(&write_fifo, &byte);
  }

  bool BufferedCommunicator::readable(void) {
    return !queue_is_empty(&read_fifo);
  }

  void BufferedCommunicator::reset(void) {
    queue_free(&read_fifo);
    queue_free(&write_fifo);
  }

}
