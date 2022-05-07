import math

def findSmallest(coord_led_array):
  smallest_x = None
  smallest_y = None

  for pair in coord_led_array:
    x = pair[0]['x']
    y = pair[0]['y']

    if (smallest_x is None or x < smallest_x):
      smallest_x = x

    if (smallest_y is None or y < smallest_y):
      smallest_y = y

  return (smallest_x, smallest_y)

def findBiggest(coord_led_array):
  biggest_x = None
  biggest_y = None

  for pair in coord_led_array:
    x = pair[0]['x']
    y = pair[0]['y']

    if (biggest_x is None or x > biggest_x):
      biggest_x = x

    if (biggest_y is None or y > biggest_y):
      biggest_y = y

  return (biggest_x, biggest_y)

def makeAllPositive(coord_led_array, increase_coor):
  x = abs(increase_coor[0])
  y = abs(increase_coor[1])

  for pair in coord_led_array:
    pair[0]['x'] += x
    pair[0]['y'] += y

def scalePixelsToImage(coord_led_array, image):
  makeAllPositive(coord_led_array, findSmallest(coord_led_array))
  biggest = findBiggest(coord_led_array)
  x_scale = image.width / biggest[0] -  0.1
  y_scale = image.height / biggest[1] - 0.1

  for pair in coord_led_array:
    pair[0]['x'] = math.floor(x_scale * pair[0]['x'])
    pair[0]['y'] = math.floor(y_scale * pair[0]['y'])

def scaleTo17x17(coord_led_array):
  makeAllPositive(coord_led_array, findSmallest(coord_led_array))
  biggest = findBiggest(coord_led_array)
  x_scale = 13 / biggest[0] -  0.1
  y_scale = 13 / biggest[1] - 0.1

  for pair in coord_led_array:
    pair[0]['x'] = math.floor(x_scale * pair[0]['x'])
    pair[0]['y'] = math.floor(y_scale * pair[0]['y'])

def flip_x_axis(coord_led_array):
  for pair in coord_led_array:
    pair[0]['y'] = - pair[0]['y']

def prepare_coordinates(coord_led_array, image):
  flip_x_axis(coord_led_array)
  makeAllPositive(coord_led_array, 
                  findSmallest(coord_led_array))
  scalePixelsToImage(coord_led_array, image)
