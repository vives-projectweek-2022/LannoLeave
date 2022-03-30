#include <pixel_mapping.h>

namespace Lannooleaf {

  PixelMapping::PixelMapping(Graph* graph) { 
    this->graph = graph;
  }

  PixelMapping::~PixelMapping() { }

  void PixelMapping::generate_mapping(void) {
    std::vector<coordinate> internal_pixels = {
      coordinate(-1, 0),
      coordinate(-2, 0),
      coordinate(-1, -1),
      coordinate(0, -1),
      coordinate(1, 0),
      coordinate(2, 0),
      coordinate(1, 1),
      coordinate(0, 1)
    };

    std::map<std::pair<int16_t, int16_t>, Pixel> temp_map;

    int min_x = 0; 
    int min_y = 0;
    int max_x = 0;
    int max_y = 0;

    // Adding all leafs to temp_map with correct coordinates
    std::map<uint8_t, Node*>::iterator itr;
    for (itr = graph->map.begin(); itr != graph->map.end(); itr++) {
      int x = itr -> second -> pos.x;
      int y = itr -> second -> pos.y;

      int i = 0;
      for (coordinate cor : internal_pixels) {
        int new_x = x + cor.x;
        int new_y = y + cor.y;

        temp_map[std::make_pair(new_x, new_y)] = {
          itr -> second -> i2c_address,
          std::make_pair((2 * i), (2 * i) + 1)
        };

        if (new_x > max_x) max_x = new_x;
        else if (new_x < min_x) min_x = new_x;
        
        if (new_y > max_y) max_y = new_y;
        else if (new_y < min_y) min_y = new_y;

        i++;
      }
    }

    x_size = max_x + std::abs(min_x) + 1;
    y_size = max_y + std::abs(min_y) + 1;

    // Creating Pixel matrix with correct size
    Pixel pixel_matrix[y_size][x_size];

    std::map<std::pair<int16_t, int16_t>, Pixel>::iterator itr2;
    for (itr2 = temp_map.begin(); itr2 != temp_map.end(); itr2++) {
      int y_pos = itr2->first.second + std::abs(min_y);
      int x_pos = itr2->first.first + std::abs(min_x);
      pixel_matrix[y_pos][x_pos] = itr2->second;
    }

    // Adding pixels from coordinate to index to pixel_mapping map
    uint total = 0;
    for (int y = 0; y < y_size; y++) {
      for (int x = 0; x < x_size; x++) {
        PRINT(pixel_matrix[y][x].i2c_address != UNCONFIGUREDADDRESS ? "X " : "0 ");
        // Save space by not adding dead pixels
        if (pixel_matrix[y][x].i2c_address != UNCONFIGUREDADDRESS) pixel_mapping[total] = pixel_matrix[y][x];
        total++;
      }
      PRINT("\n");
    } 
  }

  Pixel PixelMapping::get_pixel(uint number) {
    std::map<uint, Pixel>::iterator itr = pixel_mapping.find(number);

    if (itr != pixel_mapping.end()) return pixel_mapping[number];
    return Pixel();
  }

}
