#include "spi_slave.h"

static uint8_t dummy_data[5] = { 'D', 'U', 'M', 'M', 'Y' };
static uint8_t trash[5];
static uint8_t next = 0x00;

namespace Lannooleaf {

  Spi_slave::Spi_slave(uint mosi, uint miso, uint clk, uint cs) {

    spi_init(spi0, 1000 * 4000);
    spi_set_slave(spi0, true);

    gpio_set_function(mosi, GPIO_FUNC_SPI);
    gpio_set_function(miso, GPIO_FUNC_SPI);
    gpio_set_function(clk, GPIO_FUNC_SPI);
    gpio_set_function(cs, GPIO_FUNC_SPI);

    spi_set_format(spi0,
    8,
    SPI_CPOL_1,
    SPI_CPHA_1,
    SPI_MSB_FIRST);
  }

  uint8_t Spi_slave::read_command(void) {
    uint8_t cmd = 0x00;
    read_data(&cmd, 1);
    return cmd;
  }

  void Spi_slave::read_data(uint8_t *data_buffer, size_t len) {
    // !DO NOT PRINTF NO TIME!
    if (spi_is_readable(spi0)) spi_read_blocking(spi0, next, data_buffer, len);
  }

  void Spi_slave::write_data(uint8_t *data_buffer, size_t len) {
    // !DO NOT PRINTF NO TIME!
    spi_write_read_blocking(spi0, data_buffer, trash, len);
  }

}
