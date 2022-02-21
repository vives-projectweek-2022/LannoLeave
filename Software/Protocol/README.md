# Protocol

## Devices

### Controller
#### Pins
- Power
- GND
- I²C pins
- 4 Select pins (active high / DigitalOut pin)

#### Data
- Mapping of tile adresses
  - N*N Matirx

### Tile
Only sends response when one of the select pins is set to high.
Initial select pin is DigitalIn.

#### Pins
- Power
- GND
- I²C pins
- 4 Select pins (active high / DigitalInOut pin)

## Controller Commands
- Call for unconfigured tiles
  - Ask if any tile has not been configures (Address not in Matrix)

- Ask for info after discovering tile
  - Device response must contain incomming pin id, so that we know where to place his neigbors in the Matrix

- Ask tiles to set select pins
  - Ask a tile to set select pin to high or low

- Check if tile is still present
  - Send message to I²C address and check for response
  - Update table

- Set collor

- Set brightness

- ...

## Topology discovering

1. Loop over each slect pin, pull one at a time high
2. Search for I²C device
3. Response? no: go to next pin and go back to step 2 - yes: Update Matix using information from response

## Topology

<img src="./img/Topo.png">
