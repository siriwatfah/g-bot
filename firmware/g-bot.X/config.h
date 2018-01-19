
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

#define TRIG   PORTDbits.RD0    // trigger for HC-SR04
#define ECHO   PORTDbits.RD1    // echo for HC-SR04

#define ALERT   PORTDbits.RD3   // led for collision indicator
#define LIGHT_L PORTDbits.RD4   // led for frontal light left
#define LIGHT_R PORTDbits.RD5   // led for frontal light right

#define S_CENTER  0             // angle 0 in servomotor is center 
#define S_LEFT    90            // angle 90 in servomotor is left
#define S_RIGHT   180           // angle 180 in servomotor is right

#endif	/* _CONFIG_H */