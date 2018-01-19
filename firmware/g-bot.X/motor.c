
#include "motor.h"

void configMotor() {
    PWM1_Init(5000);
    PWM2_Init(5000);    
    PWM1_Start();
    PWM2_Start();
    
    isMove = 0;
}

void setDirection(int direction) {
    
    switch(direction) {
        case STOP:
            isMove = 0;
            stopGo();
            break;
        case FRONT:
            isMove = 1;
            goStraight();
            break;
        case BACK:            
            goBack();
            break;
        case LEFT:
            turnLeft();
            break;
        case RIGHT:
            turnRight();
            break;
    }    
    
}

void stopGo() {
    //right motor
    IN1 = 0x00;
    IN2 = 0x00;

    //left motor
    IN3 = 0x00;
    IN4 = 0x00;

    PWM1_Duty(0);
    PWM2_Duty(0);
}

void goStraight() {
    //right motor
    IN1 = 0x00;
    IN2 = 0x01;

    //left motor
    IN3 = 0x00;
    IN4 = 0x01;

    PWM1_Duty(880);
    PWM2_Duty(1000);
}

void goBack() {
    //right motor
    IN1 = 0x01;
    IN2 = 0x00;

    //left motor
    IN3 = 0x01;
    IN4 = 0x00;

    PWM1_Duty(1000);
    PWM2_Duty(1200);
}

void turnLeft() {
    //right motor
    IN1 = 0x00;
    IN2 = 0x01;

    //left motor
    IN3 = 0x00;
    IN4 = 0x00;

    PWM1_Duty(800);
    PWM2_Duty(0);
}

void turnRight() {
    //right motor
    IN1 = 0x00;
    IN2 = 0x00;

    //left motor
    IN3 = 0x00;
    IN4 = 0x01;

    PWM1_Duty(0);
    PWM2_Duty(1000);
}
