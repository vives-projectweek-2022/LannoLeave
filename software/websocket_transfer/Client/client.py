import os
import json
import math
import hashlib
import requests
import argparse

import numpy as np
from matplotlib import pyplot as plt

from time import sleep
from PIL import Image
from websocket import create_connection

# Data preparation
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

def flip_x_axis(coord_led_array):
  for pair in coord_led_array:
    pair[0]['y'] = - pair[0]['y']

def prepare_coordinates(coord_led_array, image):
  flip_x_axis(coord_led_array)
  makeAllPositive(coord_led_array, 
                  findSmallest(coord_led_array))
  scalePixelsToImage(coord_led_array, image)

def dist(x1,y1,x2,y2):
  d = np.sqrt((x2-x1)**2+(y2-y1)**2)
  return d

parser = argparse.ArgumentParser()
parser.add_argument('-im', '--image', type=str, required=True, help="Path to image source")
parser.add_argument('-i', '--ip', type=str, required=True, help="Ip address of websocket server")
parser.add_argument('-p', '--port', type=str, default=3000, help="Port of websocket server default is 3000")

args = parser.parse_args()

image = Image.open(args.image)

req = requests.get(f"http://{args.ip}:{args.port}/coordinates")
json_data = req.json()

shape_hash = hashlib.md5(json.dumps(json_data).encode("utf-8"))

print(shape_hash.hexdigest())

prepare_coordinates(json_data, image)

x, y = [], []
coords = []

for i, pair in enumerate(json_data):
  x.append(pair[0]['x'])
  y.append(pair[0]['y'])
  coords.append((pair[0]['x'], pair[0]['y']))

def intrepolate(image):
  vdist = np.vectorize(dist)

  im = np.array(image)

  nx, ny = im.shape[1], im.shape[0]
  xx = np.arange(0, nx, 1)
  yy = np.arange(0, ny, 1)

  newx = x
  newy = y

  vnewx = np.transpose(np.tile(newx,[len(xx),len(yy),1]),(2,1,0))
  vnewy = np.transpose(np.tile(newy,[len(xx),len(yy),1]),(2,1,0))
  vxx = np.tile(xx,[len(newx),len(yy),1])
  vyy = np.transpose(np.tile(yy,[len(newx),len(xx),1]),(0,2,1))
  distances = vdist(vnewx,vnewy,vxx,vyy)

  mind = np.argmin(distances, 0)

  imr = im[:,:,0]
  img = im[:,:,1]
  imb = im[:,:,2]

  newc = np.zeros((len(newx),3))
  for i in range(newc.shape[0]):
    newc[i,0] = np.mean(imr[np.where(mind==i)])
    newc[i,1] = np.mean(img[np.where(mind==i)])
    newc[i,2] = np.mean(imb[np.where(mind==i)])

  return newc

fullGif = []

base = os.path.basename(args.image)
base = os.path.splitext(base)[0]

if not os.path.exists(f"./saves/{shape_hash.hexdigest()}/{base}.npy"):
  for frame in range(image.n_frames):
    print(f"Proccessing frame {frame} of {image.n_frames}")
    image.seek(frame)
    fullGif.append(intrepolate(image.convert("RGB")))

  print("Saveing proccessed images")

  if not os.path.exists("./saves"):
    os.mkdir("saves/")

  if not os.path.exists(f"./saves/{shape_hash.hexdigest()}"):
    os.mkdir(f"./saves/{shape_hash.hexdigest()}")

  with open(f"./saves/{shape_hash.hexdigest()}/{base}.npy", 'wb') as f:
    np.save(f, np.array(fullGif))

else:
  print("Preproccessed images found")
  with open(f"./saves/{shape_hash.hexdigest()}/{base}.npy", 'rb') as f:
    a = np.load(f)
    fullGif = a.tolist()

# WebSocketConnection with interpolation
while True:
  for frame in fullGif:
    ws = create_connection(f"ws://{args.ip}:{args.port}")
    m_data = {}
    i = 0
    for coordinate, unit in json_data:
      if (not m_data.get(unit['address'])):
        m_data[unit['address']] = []

      color = [int(frame[i][0]), int(frame[i][1]), int(frame[i][2])]
      m_data.get(unit['address']).append(color)
      i+=1

    ws.send(json.dumps(m_data))
    ws.close()
    sleep(0.033)
