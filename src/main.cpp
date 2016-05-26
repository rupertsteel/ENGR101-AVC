
#include "libE101.h"

#include <csignal>
#include <cstring>
#include <cstdlib>

void handle_signal(int signal) {
	if (signal == SIGINT) {
		set_motor(1, 0);
		set_motor(2, 0);
		
		std::exit(0);
	}
}

int main(int argc, char* argv[]) {
	init(0);
	
	// set a signal handler to stop the motors when we exit the program with ^C
	std::signal(SIGINT, handle_signal);
	
	float maxSpeed = 0.5f;
	
	pictureAnalysisData data;
	
	data.kp = 0.5f;
	data.ki = 0.0f;
	data.kd = 0.0f;
	
	data.rows.resize(3);
	data.rows[0].rowNumber = 60;
	data.rows[1].rowNumber = 120;
	data.rows[2].rowNumber = 180;
	
	bool shouldOpenGate = false;
	
	// movement test
	if (argc == 2 && strcmp(argv[1], "m") == 0) {
		// movement test
		printf("Left wheel forward\n");
		set_motor(1, 25);
		getchar();
		
		printf("Left wheel backwards\n");
		set_motor(1, -25);
		getchar();
		
		set_motor(1, 0);
		
		printf("Right wheel forward\n");
		set_motor(2, 25);
		getchar();
		
		printf("Right wheel backwards\n");
		set_motor(2, -25);
		getchar();
		
		set_motor(2, 0);
		return 0;
	} else if (argc == 2 && strcmp(argv[1], "c") == 0) {
		take_picture();
		display_picture(10, 0);
	} else if (argc >= 2 && strcmp(argv[1], "r") == 0) {
		// rotate 180 test
		int time = 100;
		if (argc == 3) {
			time = std::strtol(argv[2], NULL);
		}
		
		rotate180();
		
		return 0;
	} else {
		int argvBase = 1;
		if (argc >= 2 && strcmp(argv[1], "g") == 0) {
			argvBase = 2;
			shouldOpenGate = true;
		}
		
		// provide custom values for the pid
		if (argc >= argvBase + 1) {
			maxSpeed = std::strtod(argv[argvBase], NULL);
		}
		if (argc >= argvBase + 2) {
			data.kp = std::strtod(argv[argvBase + 1], NULL);
		}
		if (argc >= argvBase + 3) {
			data.ki = std::strtod(argv[argvBase + 2], NULL);
		}
		if (argc >= argvBase + 4) {
			data.kd = std::strtod(argv[argvBase + 3], NULL);
		}
	}
	
	//openGate(); // the gate server isn't up yet, so this will block
	//moveForward();
	
	if (shouldOpenGate) {
		openGate();
	}
	
	float turnResponse = 0.01;
	bool wasReversingLastTurn = false;
	
	float leftWheelTotalMovement = 0;
	float rightWheelTotalMovement = 0;
	
	// line maze code
	while (!data.rows[1].isLineMazeEnd) {
		if (wasReversingLastTurn) {
			leftWheelTotalMovement = 0;
			rightWheelTotalMovement = 0;
			
			for (int i = 0; i < data.rows.size(); i++) {
				data.rows[i].total_error = 0;
				data.rows[i].last_error = 0;
			}
		}
		
		analysePicture(data);
		
		movementInfo movement;
		movement.maxSpeed = maxSpeed;
		movement.leftWheelSpeed = 1;
		movement.rightWheelSpeed = 1;
		
		float signal = data.rows[1].signal;
		
		// we start reversing when we see less than 10 pixels, we stop reversing when we see 20 or more pixels.
		if ((!wasReversingLastTurn && data.rows[1].pixelCount <= 10)
			|| (wasReversingLastTurn && data.rows[1].pixelCount <= 20)) {
			// check if we need to turn around.	
			
			if (leftWheelTotalMovement > 0) {
				printf("L: %f, R: %f\n", leftWheelTotalMovement, rightWheelTotalMovement);
			}
			
			wasReversingLastTurn = true;
			movement.leftWheelSpeed = -1;
			movement.rightWheelSpeed = -1;
		} else {
			wasReversingLastTurn = false;
			if (signal < 0) {
				movement.rightWheelSpeed += -signal * turnResponse;
			} else {
				movement.leftWheelSpeed += signal * turnResponse;
			}
		}
		
		setMotors(movement);
		
		leftWheelTotalMovement += movement.leftWheelSpeed * data.dt;
		rightWheelTotalMovement += movement.rightWheelSpeed * data.dt;
	}
	
	
	// walled maze code
	
	return 0;
}
