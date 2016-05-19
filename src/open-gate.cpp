
#include "libE101.h"

void openGate() {
	//connects to server with the ip address 192.168.1.2
    connect_to_server("130.195.6.196", 22);
    //sends a message to the connected server
    send_to_server("Hello server");
    //receives message from the connected server
    char message[24];
    receive_from_server(message);

    // send the password back to open the gate
    send_to_server(message);
}
