#include "libE101.h"

void analysePicture(pictureAnalysisData& data) {
    take_picture();
	
	for (int i = 0; i < data.rows.size(); i++) {
		data.rows[i].isRowEmpty = true;
		data.rows[i].pixelCount = 0;
		
		//Proportional (P)
		float sum = 0;
		for(int j = 0; j<320; j++){
			int whiteness = get_pixel(j, data.rows[i].rowNumber, 3);
			if(whiteness > 127){
				sum += (j - 160);
				data.rows[i].isRowEmpty = false;
				data.rows[i].pixelCount++;
			}
		}
		
		float error = sum / 1000.0f;
		
		float proportional_signal = error*data.kp;

		//Integral(I)
		data.rows[i].total_error += error;
		float integral_signal = data.rows[i].total_error*data.ki;
		
		//Deriviate (D)
		float error_period = 1.0f/90.0f;
		float error_diff = error - data.rows[i].last_error;
		float derivative_signal = (error_diff/error_period)*data.kd;
		data.rows[i].last_error = error;
		
		data.rows[i].signal = integral_signal + derivative_signal + proportional_signal;
	}
	
	//printf("Error: %f, integral: %f, derivative: %f, signal: %f\n",
	//	error, integral_signal, derivative_signal, totalSignal);
}
