# Software

## API

The [API](./api) connect with the SPI interface and communicates with the frontend, artnet and has a socket to run python processed gifs.

## ArtNet

The [ArtNet](./artnet) api reicieves artnet broadcast messages ans sends them trough the websochet of the api to controll all the leds. The api mapps them depending on the coords.

I used QLC+ (Que Light Controller plus) This is a freeware ArtNet DMX controller. There's a demo show in this directory.

ArtNet is an open protocoll that uses broadcast. So the controller sends messages thru an interface and broadcasts them in the subnet. That way everyone in the subnet can reicieve them.

This protocol is meant to run on isolated networks and is made for network noobs. It is easy to use, but not secure.

## Frontend

The [Frontend](./frontend) from Jay is the webinterface and all of its files.
