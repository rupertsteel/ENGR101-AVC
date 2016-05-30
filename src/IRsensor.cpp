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
		// turn left or right
	} else {
		data.wallInfront = false;
		data.turnLeft = false;
		data.turnRight = false;
		
		float error = right - left;
		
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

/*
float IRsensor() {
 //read IR sensors
 front = ReadAnalog (0);
 right= ReadAnalog (1); //made sure that the left and right IR sensors line up with the pins
 left = ReadAnalog (2);
float signal =0;
 //if  right reading is more than the left reading we are gonna turn right
 (right>left){
   
 }
 if(left>right){
}if
*/
