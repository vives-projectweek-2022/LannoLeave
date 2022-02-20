# Protocol

## Devices

### Controller
#### Pins
- Power
- GND
- Common data line or I²C
- 5 Select pins (active high / DigitalInOut pin)

#### Data
- Mapping of tile adresses


### Tile
Only sends response when one of the select pins is set to high.
Initial select pin is DigitalIn.

#### Pins
- Power
- GND
- Common data line or I²C
- 5 Select pins (active high / DigitalInOut pin)

#### Data
- LongId: 16 bit (Unique random)
- ShortId: 8 bit (Assigned by controller) 0xFF for broadcast

## Basic Payload

(Not for I²C)

| Byte |        Type        |
|------|--------------------|
|  1   |       Command      |
|  2   |Short ID to address |

## Commands
- Call for unconfigured tiles (Broadcast)
  - Ask if any tile has not been configures (Has no short id)

- Assign short id by long id (Broadcast)
  - Comand to assing a short id using long id

- Ask tiles to set select pins
  - Ask a tile to set select pin to high or low
  - Uses short id

- Read long id if selected
  - Ask for long id if no short id has been assigned

- Check if tile is still present
  - Using short id to ask if still prensent
  - Update table

Once all tiles have been configures all select pins can be set to high controller can now communicate using short id

- Set collor
- Set brightness
- ...

## Topology discovering

1. Loop over each slect pin pull one at a time high
2. Call for unconfigured tiles
3. Response ? no: go to next pin and go back to step 2 - yes: Read long id of tile
4. Assign short id to tile, store on which pin you got response and from which pin it came

When all pins have been looped, no loop over the neighbors neighbors and so on while keeping track of tiles using a tree datastructure.



## Topology

<img src="./img/Topo.png">
