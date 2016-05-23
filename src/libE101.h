
#ifndef LIB_E_101_H_
#define LIB_E_101_H_

#include <stdio.h>
#include <time.h>

#include <vector>
#include <chrono>

// libe101 functions
extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
extern "C" int Sleep( int sec , int usec );
extern "C" int set_motor(int motor , int speed );
extern "C" int display_picture(int delay_sec,int delay_usec);
extern "C" int read_analog(int ch_adc);

extern "C" int take_picture();
extern "C" char get_pixel(int row, int col, int color);

// robot dimensions
// all dimensions are in metres
#define ROBOT_WHEEL_RADIUS 0.0295f
#define ROBOT_WHEEL_WIDTH 0.007f
#define ROBOT_WHEEL_TO_CENTER 0.0455f


// open-gate.cpp functions

/// \brief Opens the gate
/// This assumes that the libe101 library is initialized.
void openGate();

// movement.cpp functions

/// \brief Moves the robot forward.
/// This assumes that the libe101 library is initialized.
void moveForward();

// Turns the robot in a circle of a specific radius. 
void turnLeftRadius(float radius);
void turnRightRadius(float radius);

// analysePicture.cpp functions and structs

struct pictureAnalysisRow {
	int rowNumber;
	float total_error;
	float last_error;
	float signal;
	bool isRowEmpty;
	int pixelCount;
	bool isLineMazeEnd;
};

struct pictureAnalysisData {
    std::vector<pictureAnalysisRow> rows;
	float kp;
	float ki;
	float kd;
	std::chrono::system_clock::time_point lastPictureTime;
};

struct movementInfo {
	float maxSpeed; // from 0 to 1, sets the max speed.
	float leftWheelSpeed; // can be any value, left and right are normalized so that one is 1 or -1, we then multiply by the max speed
	float rightWheelSpeed;
};

void setMotors(movementInfo& info);

void analysePicture(pictureAnalysisData& data);

#endif // LIB_E_101_H_
