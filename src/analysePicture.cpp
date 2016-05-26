#include "libE101.h"

void analysePicture(pictureAnalysisData& data) {
    take_picture();
	
	auto now = std::chrono::system_clock::now();
	
	auto dur = now - data.lastPictureTime;
	data.dt = std::chrono::duration_cast<std::chrono::duration<float>>(dur).count();
	
	data.lastPictureTime = now;
	
	for (int i = 0; i < data.rows.size(); i++) {
		// for decting the end of the line maze, if the number of red pixels is much larger than the number
		// of blue pixels, then we are at the line maze end.
		int redCount = 0;
		int blueCount = 0;
		
		data.rows[i].isRowEmpty = true;
		data.rows[i].pixelCount = 0;
		
		//Proportional (P)
		float sum = 0;
		for(int j = 0; j<320; j++){
			int red = get_pixel(j, data.rows[i].rowNumber, 0);
			if (red > 127) {
				redCount++;
			}
			int blue = get_pixel(j, data.rows[i].rowNumber, 1);
			if (blue > 127) {
				blueCount++;
			}
			
			int whiteness = get_pixel(j, data.rows[i].rowNumber, 3);
			if(whiteness > 127){
				sum += (j - 160);
				data.rows[i].isRowEmpty = false;
				data.rows[i].pixelCount++;
			}
		}
		
		if (redCount > blueCount + 20) {
			// we are on the end
			data.rows[i].isLineMazeEnd = true;
		}
		
		float error = sum / 1000.0f;
		
		float proportional_signal = error*data.kp;

		//Integral(I)
		data.rows[i].total_error += error;
		float integral_signal = data.rows[i].total_error*data.ki * data.dt;
		
		//Deriviate (D)
		float error_diff = error - data.rows[i].last_error;
		float derivative_signal = (error_diff/data.dt)*data.kd;
		data.rows[i].last_error = error;
		
		data.rows[i].signal = integral_signal + derivative_signal + proportional_signal;
	}
	
	//printf("Error: %f, integral: %f, derivative: %f, signal: %f\n",
	//	error, integral_signal, derivative_signal, totalSignal);
}
