import Lannooleaf from '../src/Lannooleaf.js'
import Color from '../src/Color.js'

import Graph from '../src/graph.js';

const red = new Color(50, 0, 0);
const green = new Color(0, 50, 0);
const blue = new Color(0, 0, 50);
const off = new Color(0, 0, 0);

const controller = new Lannooleaf();
const graph = new Graph();

await controller.Init();

await controller.getAdjList(graph);

// Set all led to a random value
let units = Array.from(graph.map.keys());
for (var i = 0; i < units.length; i++) {
  let string = [];
  for (var led = 0; led < 16; led++) {
    string.push(new Color(Math.floor(Math.random() * 128), Math.floor(Math.random() * 128), Math.floor(Math.random() * 128)));
  }
  await controller.setLedString(graph.map.get(units[i]).address, string);
  await sleep(33);
}

await sleep(2000);
await controller.setAll(off);

for (var x = 0; x < 100; x++) {
  for (var i = 0; i < units.length; i++) {
    await controller.setAll(off);
    await sleep(1);
    for (var led = 0; led < 16; led++) {
      await controller.setLed(units[i], led, new Color(Math.floor(Math.random() * 128), Math.floor(Math.random() * 128), Math.floor(Math.random() * 128)));
      await sleep(33);
    }
  }
}

console.log("destroying");
controller.Destroy(error => {
  if (error) console.log(error);
});

async function sleep(ms) {
  return new Promise(resolve => {
    setTimeout(resolve, ms);
  });
}
