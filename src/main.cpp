
#include "libE101.h"

#include <signal.h>

void handle_signal(int signal) {
	if (signal == SIGINT) {
		set_motor(1, 0);
		set_motor(2, 0);
	}
}

int main(int argc, char* argv[]) {
	init(1);
	
	// set a signal handler to stop the motors when we exit the program with ^C
	// code from https://gist.github.com/aspyct/3462238
	sigaction sa;
	sa.sa_handler = &handle_signal;
	sa.sa_flags = SA_RESTART;
	
	sigfillset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	
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
