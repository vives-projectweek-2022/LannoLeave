# Controller

## Packets to send over SPI

### Get_matrix_size
| Command | Extra | Returns |
|---------|-------|---------|
| 0x01    | None  | 2 bytes |

**Get the size of the pixel matrix**

### Set leaf led
| Command | Extra      | Returns |
|---------|------------|---------|
| 0x02    | Index, RGB | None    |

**Set one individual led**

### Set all
| Command | Extra | Returns |
|---------|-------|---------|
| 0x03    | RGB   | None    |

**Set all leds**

### Clear leaf led
| Command | Extra | Returns |
|---------|-------|---------|
| 0x04    | Index | None    |

**Clear one led**

### Clear all
| Command | Extra | Returns |
|---------|-------|---------|
| 0x05    | None  | None    |

**Clears all leds**

### Set Random
| Command | Extra | Returns |
|---------|-------|---------|
| 0x06    | None  | None    |

**Gives random value to all leds**
