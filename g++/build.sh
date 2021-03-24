#Builds
g++ -c -Wall src/Socket.cpp -o build/Socket.o
g++ -c -Wall src/Server.cpp -o build/Server.o
g++ -c -Wall src/Client.cpp -o build/Client.o

#Exe
g++ -Wall build/Socket.o build/Server.o -o exe/Server
g++ -Wall build/Socket.o build/Client.o -o exe/Client
