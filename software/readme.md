# raspberry pi with docker

Raspi Arno IP-Addr: `172.16.240.30`

API port  : `:3000`

## installation:

- install raspi x64

install packages

- curl -fsSL https://get.docker.com -o get-docker.sh
- sudo sh get-docker.sh
- ‍sudo usermod -aG docker ${USER}
- sudo apt-get install libffi-dev libssl-dev
- sudo apt install python3-dev -y
- sudo apt-get install -y python3 python3-pip
- ‍sudo pip3 install docker-compose
- sudo systemctl enable docker
- sudo apt install git -y

then

- git cline the repo
- cd into software/lannoo_leave
- docker compose up

done :)

## sources

https://github.com/noble/noble
