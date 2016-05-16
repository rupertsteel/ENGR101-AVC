#include "libE101.h"

float analysePicture(pictureAnalysisData& data) {
    take_picture();
    
    //Proportional (P)
    float sum = 0;
    for(int i = 0; i<320; i++){
        int whiteness = get_pixel(i, 120, 3);
        if(whiteness > 127){
            sum += (i - 160);
        }
    }
    
    float error = sum / 1000.0f;
    
    float proportional_signal = error*data.kp;

    //Integral(I)
    data.total_error += error;
    float integral_signal = data.total_error*data.ki;
    
    //Deriviate (D)
    float error_period = 1.0f/90.0f;
    float error_diff = error - data.last_error;
    float derivative_signal = (error_diff/error_period)*data.kd;
    data.last_error = error;
	
	float totalSignal = integral_signal + derivative_signal + proportional_signal;
	
	printf("Error: %f, integral: %f, derivative: %f, signal: %f\n",
		error, integral_signal, derivative_signal, totalSignal);
	
	return totalSignal;
}
