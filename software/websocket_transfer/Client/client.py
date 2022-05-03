import os
import json
import hashlib
import requests
import argparse

import numpy as np
from matplotlib import pyplot as plt

from time import sleep
from PIL import Image
from websocket import create_connection

import preprocces as pp
from interpolation import intrepolate

# Parsing arguments
parser = argparse.ArgumentParser()
parser.add_argument('-im', '--image', type=str, required=True, help="Path to image source")
parser.add_argument('-i', '--ip', type=str, required=True, help="Ip address of websocket server")
parser.add_argument('-p', '--port', type=str, default=3000, help="Port of websocket server default is 3000")
parser.add_argument('--interpolate', help="Tells script to use interpolation")

args = parser.parse_args()

image = Image.open(args.image)

# Getting coordinates from api
req = requests.get(f"http://{args.ip}:{args.port}/coordinates")
json_data = req.json()

shape_hash = hashlib.md5(json.dumps(json_data).encode("utf-8"))

pp.prepare_coordinates(json_data, image)

x, y = [], []

for i, pair in enumerate(json_data):
  x.append(pair[0]['x'])
  y.append(pair[0]['y'])

fullGif = []

base = os.path.basename(args.image)
base = os.path.splitext(base)[0]

if not os.path.exists(f"./saves/{shape_hash.hexdigest()}/{base}.npy"):
  for frame in range(image.n_frames):
    print(f"Proccessing frame {frame} of {image.n_frames}")
    image.seek(frame)
    fullGif.append(intrepolate(image.convert("RGB"), x, y))

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
    run = ws.recv()
    
    if (not bool(run)): break

    ws.close()
    sleep(0.033)
