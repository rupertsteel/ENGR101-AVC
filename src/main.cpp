
#include "libE101.h"

int main(int argc, char* argv[]) {
	init(1);
	
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
