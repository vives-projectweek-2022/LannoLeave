#include "mbed.h"

// Comment to compile as tile
#define CONTROLLER

#ifdef CONTROLLER
#include "controller.h"

int main() {
  LannooLeaf::Controller controller;

  printf("Starting controller\n");

  // int address = controller.scan_i2c_devices();
  // ThisThread::sleep_for(1s);
  // printf("This should be 0: %i \n", address);

  // for (int i = 0; i < 11; i++) {
  //   for (int j = 0; j < 11; j++) {
  //     printf(controller.tiles[i][j].to_string().c_str());
  //     printf("  ");
  //   }
  //   printf("\n");
  // }
  // printf("\n");

  while (true) {
    controller.update();
  };
  
};
#endif

#ifndef CONTROLLER
#include "tile.h"

int main() {
  LannooLeaf::Tile tile;

  while (true) {
    tile.update();
  };
  
};
#endif