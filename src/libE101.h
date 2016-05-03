
#ifndef LIB_E_101_H_
#define LIB_E_101_H_

#include <stdio.h>
#include <time.h>

// libe101 functions
extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
extern "C" int Sleep( int sec , int usec );
extern "C" int set_motor(int motor , int speed );


// open-gate.cpp functions

/// \brief Opens the gate
/// This assumes that the libe101 library is initialized.
void openGate();

// movement.cpp functions

/// \brief Moves the robot forward.
/// This assumes that the libe101 library is initialized.
void moveForward();

#endif // LIB_E_101_H_
