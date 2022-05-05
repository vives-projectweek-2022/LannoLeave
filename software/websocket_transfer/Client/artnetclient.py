import os 
import json

import matplotlib.pyplot as plt
import preprocces as pp

json_data = json.load(open("./data/coordinates.json"))
print(json_data)

pp.scaleTo17x17(json_data)

x, y = [], []

for i, pair in enumerate(json_data):
  x.append(pair[0]['x'])
  y.append(pair[0]['y'])

plt.scatter(x, y)
plt.show()

print(json.dumps(json_data))
