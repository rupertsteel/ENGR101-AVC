
#include "libE101.h"

int main(int argc, char* argv[]) {
	init(1);
	
	//openGate(); // the gate server isn't up yet, so this will block
	moveForward();
	
	return 0;
}
