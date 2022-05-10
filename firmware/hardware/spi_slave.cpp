#include "spi_slave.hpp"

namespace Lannooleaf {

  SPISlave::SPISlave(spi_inst_t* spi, uint mosi, uint miso, uint clk, uint cs, uint frequency)
  : SPI(spi, mosi, miso, clk, cs, frequency) {

    if (spi == spi0) {
      s0_read_fifo  = &this->read_fifo;
      s0_write_fifo = &this->write_fifo;

      spi_set_slave(spi0, true);

      hw_set_bits(&spi_get_hw(spi0)->imsc, SPI_SSPIMSC_RXIM_BITS | SPI_SSPIMSC_TXIM_BITS);
      irq_set_exclusive_handler(SPI0_IRQ, spi0_irq_handel);
      irq_set_enabled(SPI0_IRQ, true);

    } else {
      // Create slave for spi1
      throw std::runtime_error("Slave functionality for spi1 has not been implemented yet");
    }

  }

  void __not_in_flash_func(SPISlave::spi0_irq_handel)(void) {
    uint32_t status = spi_get_hw(spi0)->mis;
    
    // Tx fifo half empty or less
    while (status & SPI_SSPMIS_TXMIS_BITS) {
      // Adding 0xff if writefifo is empty

      uint8_t value = 0xff;
      queue_try_remove(s0_write_fifo, &value);
      spi_get_hw(spi0)->dr = value;
      
      status = spi_get_hw(spi0)->mis;
    }

    while (status & SPI_SSPMIS_RXMIS_BITS) {
      while (spi_get_hw(spi0)->sr & 0x04) {  
        uint16_t value = spi_get_hw(spi0)->dr;
        queue_try_add(s0_read_fifo, &value);
      }

      status = spi_get_hw(spi0)->mis;
    }
  }

}
