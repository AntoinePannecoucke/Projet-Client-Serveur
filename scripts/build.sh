#!/bin/bash

if [ ! -d "build" ]; then
  mkdir build
fi


#Builds
g++ -c -Wall src/Socket.cpp -o build/Socket.o
g++ -c -Wall src/Server.cpp -o build/Server.o
g++ -c -Wall src/Client.cpp -o build/Client.o

if [ ! -d "exe" ]; then
  mkdir exe
fi

#Exe
g++ -Wall build/Socket.o build/Server.o -o exe/Server.exe
g++ -Wall build/Socket.o build/Client.o -o exe/Client.exe
