
#include "libE101.h"

void moveForward() {
	set_motor(1, 1, 255);
	set_motor(2, 2, 255);
	Sleep(0, 500000);
	
	set_motor(1, 1, 0);
	set_motor(2, 1, 0);
}
