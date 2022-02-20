#include "tile.h"

namespace LannooLeaf {

  Tile::Tile(void) {
    select[0] = new DigitalInOut(D15);
    select[1] = new DigitalInOut(D14);
    select[2] = new DigitalInOut(D13);
    select[3] = new DigitalInOut(D12);
  }
  
  Tile::~Tile() {
    for (int i = 0; i < 4; i++) delete select[i];
  }

  void Tile::update(void) {
    
  }

}