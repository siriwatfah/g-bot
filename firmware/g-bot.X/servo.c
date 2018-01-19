
#include "servo.h"

void __set_full(int deg) {
    switch (deg) {
        case 0:
            __delay_us(18450);
            break;
        case 90:
            __delay_us(17550);
            break;
        case 180:
            __delay_us(19300);
            break;
        default:
            __delay_us(18000);
            break;
    }
}

void __set_pulse(int deg) {
    switch (deg) {
        case 0:
            __delay_us(1550);
            break;
        case 90:
            __delay_us(2450);
            break;
        case 180:
            __delay_us(700);
            break;
        default:
            __delay_us(2000);
            break;
    }
}

void servoRotate(int deg) {    
    unsigned int i;
    for (i = 0; i < 50; i++) {
        PULSE = 1;
        __set_pulse(deg);
        PULSE = 0;
        __set_full(deg);
    }
}

/*
 
    float aux;   
    int pulse, full, minPulse, maxPulse, refreshInterval;
    
    minPulse = 450;
    maxPulse = 2450;
    refreshInterval = 20000;
    
    aux = (maxPulse - minPulse) / 180.0;
    pulse = minPulse + (deg * aux);
    full = refreshInterval - pulse;
 * 
 * 
 */
