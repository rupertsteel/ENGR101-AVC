#include "libE101.h"

void analyseIrSensor(irData& data) {
	// read IR sensors
	int front = read_analog(0);
	int right = read_analog(1); //made sure that the left and right IR sensors line up with the pins
	int left = read_analog(2);
	
	data.dt = 0.02f;
	
	if (front > 300) {
		data.signal = 0;
		data.wallInfront = true;
		
		data.turnLeft = (left > 200);
		data.turnRight = (right > 200);
		
		// turn left or right
	} else {
		data.wallInfront = false;
		data.turnLeft = false;
		data.turnRight = false;
		
		float error = left - right;
		
		float proportional_signal = error*data.kp;

		//Integral(I)
		data.total_error += error;
		float integral_signal = data.total_error*data.ki * data.dt;
		
		//Deriviate (D)
		float error_diff = error - data.last_error;
		float derivative_signal = (error_diff/data.dt)*data.kd;
		data.last_error = error;
		
		data.signal = integral_signal + derivative_signal + proportional_signal;
	}
}


