
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
	} else if (argc == 2 && strcmp(argv[1], "c") == 0) {
		take_picture();
		display_picture(10, 0);
	} else {
		// provide custom values for the pid
		if (argc >= 2) {
			maxSpeed = std::strtod(argv[1], NULL);
		}
		if (argc >= 3) {
			data.kp = std::strtod(argv[2], NULL);
		}
		if (argc >= 4) {
			data.ki = std::strtod(argv[3], NULL);
		}
		if (argc >= 5) {
			data.kd = std::strtod(argv[4], NULL);
		}
	}
	
	//openGate(); // the gate server isn't up yet, so this will block
	//moveForward();
	
	float turnResponse = 0.01;
	bool wasReversingLastTurn = false;
	
	while (true) {
		if (wasReversingLastTurn) {
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
		
		Sleep(0, 11111);
	}
	
	return 0;
}
