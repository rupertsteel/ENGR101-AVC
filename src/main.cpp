
#include "libE101.h"

#include <csignal>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <thread>

void handle_signal(int signal) {
	if (signal == SIGINT) {
		set_motor(1, 0);
		set_motor(2, 0);
		
		std::exit(0);
	}
}

void savePictureWithNumber(int& pictureNumber) {
	char buffer[6];
	sprintf(buffer, "%05u", pictureNumber);
	
	save_picture(buffer);
	pictureNumber++;
}

int main(int argc, char* argv[]) {
	init(0);
	
	// set a signal handler to stop the motors when we exit the program with ^C
	std::signal(SIGINT, handle_signal);
	
	float maxSpeed = 0.2f;
	
	pictureAnalysisData data;
	irData walledMazeData;
	
	data.kp = 40.0f;
	data.ki = 1.0f;
	data.kd = 1.0f;
	
	data.rows.resize(3);
	data.rows[0].rowNumber = 60;
	data.rows[1].rowNumber = 120;
	data.rows[2].rowNumber = 180;
	
	walledMazeData.kp = 1.0f;
	walledMazeData.ki = 0.1f;
	walledMazeData.kd = 0.1f;
	
	bool shouldOpenGate = false;
	bool shouldSharpTurn = false;
	
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
	} else if (argc == 2 && strcmp(argv[1], "c") == 0) {
		take_picture();
		display_picture(10, 0);
	} else if (argc >= 2 && strcmp(argv[1], "i") == 0) {
		for (int i = 0; i < 10; i++) {
			int value = read_analog(0);
			printf("%d\n", value);
		}
		
	} else if (argc >= 2 && strcmp(argv[1], "r") == 0) {		
		turnToLineClockwise();
		
		return 0;
	} else {
		int argvBase = 1;
		if (argc >= 2 && islower(argv[1][0])) {
			argvBase = 2;
			
			for (int i = 0; argv[1][i] != 0; i++) {
				if (argv[1][i] == 'g') {
					shouldOpenGate = true;
				} else if (argv[1][i] == 't') {
					shouldSharpTurn = true;
				}
			}
		}
		
		// provide custom values for the pid
		if (argc >= argvBase + 1) {
			maxSpeed = std::strtod(argv[argvBase], NULL);
		}
		if (argc >= argvBase + 2) {
			data.kp = std::strtod(argv[argvBase + 1], NULL);
		}
		if (argc >= argvBase + 3) {
			data.ki = std::strtod(argv[argvBase + 2], NULL);
		}
		if (argc >= argvBase + 4) {
			data.kd = std::strtod(argv[argvBase + 3], NULL);
		}
		
		// walled maze pid
		if (argc >= argvBase + 5) {
			walledMazeData.kp = std::strtod(argv[argvBase + 4], NULL);
		}
		if (argc >= argvBase + 6) {
			walledMazeData.ki = std::strtod(argv[argvBase + 5], NULL);
		}
		if (argc >= argvBase + 7) {
			walledMazeData.kd = std::strtod(argv[argvBase + 6], NULL);
		}
	}
	
	if (shouldOpenGate) {
		openGate();
	}
	
	float turnResponse = 0.01;
	
	float leftWheelTotalMovement = 0;
	float rightWheelTotalMovement = 0;
	
	int pictureCount = 0;
	
	bool reversing = false;
	
	std::vector<float> lastMovementDistances;
	
	// line maze code
	while (!data.rows[1].isLineMazeEnd) {
		if (reversing) {
			leftWheelTotalMovement = 0;
			rightWheelTotalMovement = 0;
			
			for (int i = 0; i < data.rows.size(); i++) {
				data.rows[i].total_error = 0;
				data.rows[i].last_error = 0;
			}
		}
		
		analysePicture(data);
		
		if (!data.isThereLineToFront && shouldSharpTurn) {
			if (data.isThereLineToLeft && data.isThereLineToRight) {
				// do a random chance
				
				savePictureWithNumber(pictureCount);
				
				if (rand() % 2 == 0) {
					turnToLineClockwise();
				} else {
					turnToLineAntiClockwise();
				}
				
				continue;
			} else if (data.isThereLineToLeft) {
				savePictureWithNumber(pictureCount);
				turnToLineAntiClockwise();
				continue;
			} else if (data.isThereLineToRight) {
				savePictureWithNumber(pictureCount);
				turnToLineClockwise();
				continue;
			}
		}
		
		movementInfo movement;
		movement.maxSpeed = maxSpeed;
		movement.leftWheelSpeed = 1;
		movement.rightWheelSpeed = 1;
		
		float signal = data.rows[1].signal;
		
		bool shouldStartReversing = data.rows[1].pixelCount <= 10;
		bool shouldStopReversing = data.rows[1].pixelCount > 20;
		
		if (shouldStartReversing && !reversing) {
			// code for starting reversing
			reversing = true;
			
			/*if (leftWheelTotalMovement > 0) {
				printf("L: %f, R: %f\n", leftWheelTotalMovement, rightWheelTotalMovement);
			}*/
			
			savePictureWithNumber(pictureCount);
			
			lastMovementDistances.push_back((leftWheelTotalMovement + rightWheelTotalMovement) / 2.0f);
			leftWheelTotalMovement = 0;
			rightWheelTotalMovement = 0;
			if (lastMovementDistances.size() > 7) {
				lastMovementDistances.erase(lastMovementDistances.begin());
			}
			
			if (lastMovementDistances.size() == 7 &&
			  std::all_of(lastMovementDistances.begin(), lastMovementDistances.end(), [](float f){ return f < 0.09f; })) {
				// we will turn around
				lastMovementDistances.clear();
				reversing = false;
				
				turnToLineClockwise();
				
				continue;
			}
		}
		if (shouldStopReversing && reversing) {
			// code for when we are stopping reversing
			reversing = false;
			leftWheelTotalMovement = 0;
			rightWheelTotalMovement = 0;
		}
		
		if (reversing) {
			movement.leftWheelSpeed = -1;
			movement.rightWheelSpeed = -1;
		} else {
			if (signal < 0) {
				movement.rightWheelSpeed += -signal * turnResponse;
			} else {
				movement.leftWheelSpeed += signal * turnResponse;
			}
		}
		
		setMotors(movement);
		
		leftWheelTotalMovement += movement.leftWheelSpeed * data.dt;
		rightWheelTotalMovement += movement.rightWheelSpeed * data.dt;
	}
	
	while (true) {
		movementInfo movement;
		movement.maxSpeed = maxSpeed;
		movement.leftWheelSpeed = 1;
		movement.rightWheelSpeed = 1;
		
		analyseIrSensor(walledMazeData);
		
		if (walledMazeData.wallInfront) {
			if (walledMazeData.turnRight) {
				set_motor(1, 51);
				set_motor(2, -51);
	
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				
				continue;
			}
			if (walledMazeData.turnLeft) {
				set_motor(1, -51);
				set_motor(2, 51);
	
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				
				continue;
			}
		}
		
		if (walledMazeData.signal < 0) {
			movement.rightWheelSpeed += -walledMazeData.signal * 0.001f;
		} else {
			movement.leftWheelSpeed += walledMazeData.signal * 0.001f;
		}
		
		Sleep(0, 20000);
		
		setMotors(movement);
	}
	// walled maze code
	
	return 0;
}
