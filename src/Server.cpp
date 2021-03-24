 #include "Socket.h"

int main(){
    stdsock::ConnectionSocket connection;
    connection.openConnection();
    while(true){
        stdsock::TransportSocket transport;
        transport.setSock(connection.accept());
        transport.send("hello world");
    }
    return 0;
}