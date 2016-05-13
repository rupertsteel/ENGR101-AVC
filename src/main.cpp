
#include "libE101.h"

#include <csignal>
#include <cstring>

void handle_signal(int signal) {
	if (signal == SIGINT) {
		set_motor(1, 0);
		set_motor(2, 0);
		
		std::exit();
	}
}

int main(int argc, char* argv[]) {
	init(1);
	
	// set a signal handler to stop the motors when we exit the program with ^C
	std::signal(SIGINT, handle_signal);
	
	// movement test
	if (argc == 2 && strcmp(argv[1], "m") == 0) {
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
	}
	
	if (argc == 2 && strcmp(argv[1], "g") == 0) {
		openGate();
	}
	
	//openGate(); // the gate server isn't up yet, so this will block
	//moveForward();
	
	pictureAnalysisData data;
	data.last_error = 0.0f;
	data.total_error = 0.0f;
	
	float turnResponse = 1;
	
	while (true) {
		float signal = analysePicture(data);
		
		if (signal < -0.1f) {
			turnLeftRadius(turnResponse / signal);
		} else if (signal > 0.1f) {
			turnRightRadius(turnResponse / signal);
		} else {
			moveForward();
		}
		
		Sleep(0, 11111);
	}
	
	return 0;
}
