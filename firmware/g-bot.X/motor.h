 
#ifndef _MOTOR_H
#define	_MOTOR_H

#include <xc.h>
#include "pwm.h"

#define IN1    PORTBbits.RB0    // Input for engine right
#define IN2    PORTBbits.RB1    // Input for engine right
#define IN3    PORTBbits.RB2    // Input for engine left
#define IN4    PORTBbits.RB3    // Input for engine left

#define STOP    0               // flag stop for L293D
#define FRONT   1               // flag front for L293D
#define BACK    2               // flag bag for L293D
#define LEFT    3               // flag left for L293D
#define RIGHT   4               // flag right for L293D

int isMove = 0;

void configMotor();
/**
 * Set direction of the engines 
 * @param direction integer with direction of engine
 */
void setDirection(int direction);

void stopGo();
void goBack();
void goStraight();

void turnLeft();
void turnRight();

#endif	/* _MOTOR_H */

