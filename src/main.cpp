
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
	
	pictureAnalysisData data;
	data.last_error = 0.0f;
	data.total_error = 0.0f;
	
	data.kp = 0.5f;
	data.ki = 0.0f;
	data.kd = 0.0f;
	
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
	} else if (argc == 2 && strcmp(argv[1], "g") == 0) {
		openGate();
	} else {
		// provide custom values for the pid
		if (argc >= 2) {
			data.kp = std::strtod(argv[1], NULL);
		}
		if (argc >= 3) {
			data.ki = std::strtod(argv[2], NULL);
		}
		if (argc >= 4) {
			data.kd = std::strtod(argv[3], NULL);
		}
	}
	
	//openGate(); // the gate server isn't up yet, so this will block
	//moveForward();
	
	float turnResponse = 0.01;
	
	while (true) {
		float signal = analysePicture(data);
		
		movementInfo movement;
		movement.maxSpeed = 0.15f;
		movement.leftWheelSpeed = 1;
		movement.rightWheelSpeed = 1;
		
		
		if (signal < 0) {
			movement.rightWheelSpeed += -signal * turnResponse;
		} else {
			movement.leftWheelSpeed += signal * turnResponse;
		}		
		
		setMotors(movement);
		
		Sleep(0, 11111);
	}
	
	return 0;
}
