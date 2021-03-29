#!/bin/bash

if [ ! -d "build" ]; then
  mkdir build
fi


#Builds
g++ -c -Wall src/Socket.cpp -o build/Socket.o
g++ -c -Wall src/Server.cpp -o build/Server.o
g++ -c -Wall src/Client.cpp -o build/Client.o
g++ -c -Wall src/Player.cpp -o build/Player.o
g++ -c -Wall src/Card.cpp -o build/Card.o

if [ ! -d "exe" ]; then
  mkdir exe
fi

#Exe
g++ -Wall build/Socket.o build/Card.o build/Player.o build/Server.o -o exe/Server.exe
g++ -Wall build/Socket.o build/Client.o -o exe/Client.exe
