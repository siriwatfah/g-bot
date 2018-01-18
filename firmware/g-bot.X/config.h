
#ifndef _CONFIG_H
#define	_CONFIG_H

#include <xc.h>
#include "xtal.h"

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#define IN1    PORTBbits.RB0    // Input for engine right
#define IN2    PORTBbits.RB1    // Input for engine right
#define IN3    PORTBbits.RB2    // Input for engine left
#define IN4    PORTBbits.RB3    // Input for engine left

#define TRIG   PORTDbits.RD0    // trigger for HC-SR04
#define ECHO   PORTDbits.RD1    // echo for HC-SR04

#define ALERT   PORTDbits.RD3   // led for collision indicator
#define LIGHT_L PORTDbits.RD4   // led for frontal light left
#define LIGHT_R PORTDbits.RD5   // led for frontal light right

#define STOP    0               // flag stop for L293D
#define FRONT   1               // flag front for L293D
#define BACK    2               // flag bag for L293D
#define LEFT    3               // flag left for L293D
#define RIGHT   4               // flag right for L293D

#define S_CENTER  0             // angle 0 in servomotor is center 
#define S_LEFT    90            // angle 90 in servomotor is left
#define S_RIGHT   180           // angle 180 in servomotor is right

#endif	/* _CONFIG_H */