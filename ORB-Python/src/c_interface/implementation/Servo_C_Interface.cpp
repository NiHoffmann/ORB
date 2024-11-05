#include "stdint.h"
#include "AppTask.h"

extern "C" {

	void setModelServo(uint16_t port, uint16_t speed, uint16_t angle){
		AppTask::setModelServo(nullptr, port, speed, angle);
	}

}
