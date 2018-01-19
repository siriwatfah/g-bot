
#ifndef _SERVO_H
#define	_SERVO_H

#include <xc.h>
#include "xtal.h"

#define PULSE   PORTDbits.RD2         // define pin pulse

#define MIN_PULSE_WIDTH      1000     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2000     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds

void __set_full(int deg);
void __set_pulse(int deg);

void servoRotate(int deg);

#endif	/* _SERVO_H */

