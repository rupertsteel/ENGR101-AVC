
#include "libE101.h"

#include <algorithm>

void moveForward() {
	set_motor(1, 63);
	set_motor(2, 63);
}

void turnRightRadius(float radius) {
	float leftWheelRadius = radius + ROBOT_WHEEL_TO_CENTER;
	float rightWheelRadius = radius - ROBOT_WHEEL_TO_CENTER;
	
	float leftWheelDrive = 1;
	float rightWheelDrive = leftWheelDrive / leftWheelRadius * rightWheelRadius;
	
	// now normalize the values 
	float driveMax = std::max(abs(leftWheelDrive), abs(rightWheelDrive)) * 4;
	leftWheelDrive /= driveMax;
	rightWheelDrive /= driveMax;
	
	// and convert to int's for the set_motor function
	int leftWheelDriveInt = static_cast<int>(leftWheelDrive * 255);
	int rightWheelDriveInt = static_cast<int>(rightWheelDrive * 255);
	
	set_motor(1, leftWheelDriveInt);
	set_motor(2, rightWheelDriveInt);
}

void turnLeftRadius(float radius) {
	float leftWheelRadius = radius - ROBOT_WHEEL_TO_CENTER;
	float rightWheelRadius = radius + ROBOT_WHEEL_TO_CENTER;
	
	float rightWheelDrive = 1;
	float leftWheelDrive = rightWheelDrive / rightWheelRadius * leftWheelRadius;
	
	// now normalize the values 
	float driveMax = std::max(abs(leftWheelDrive), abs(rightWheelDrive)) * 4;
	leftWheelDrive /= driveMax;
	rightWheelDrive /= driveMax;
	
	// and convert to int's for the set_motor function
	int leftWheelDriveInt = static_cast<int>(leftWheelDrive * 255);
	int rightWheelDriveInt = static_cast<int>(rightWheelDrive * 255);
	
	set_motor(1, leftWheelDriveInt);
	set_motor(2, rightWheelDriveInt);
}

void setMotors(movementInfo& movement) {
	float driveMax = std::max(abs(movement.leftWheelSpeed), abs(movement.rightWheelSpeed));
	
	movement.leftWheelSpeed = movement.leftWheelSpeed / driveMax * movement.maxSpeed;
	movement.rightWheelSpeed = movement.rightWheelSpeed / driveMax * movement.maxSpeed;
	
	// and convert to int's for the set_motor function
	int leftWheelDriveInt = static_cast<int>(movement.leftWheelSpeed * 255);
	int rightWheelDriveInt = static_cast<int>(movement.rightWheelSpeed * 255);
	
	if (leftWheelDriveInt > 255 || leftWheelDriveInt < -255) {
		printf("Out of range: I: %d, F: %f\n", leftWheelDriveInt, movement.leftWheelSpeed);
	}
	
	set_motor(1, leftWheelDriveInt);
	set_motor(2, rightWheelDriveInt);
}
