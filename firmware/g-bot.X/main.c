/*
 * G-Bot is a simple robot based on the PIC16F877A microcontroller.
 * Author: Adilson Pavan
 * 
 * Controller: PIC16F887A
 * Complier: MPLABX + XC8
 * Created on December 9, 2017, 10:09 PM
 */

#include <pic16f877a.h>
#include <pic.h>

#include "config.h"
#include "serial.h"
#include "servo.h"
#include "motor.h"

enum mode_opt {
    MOD_M = 1,      // manual mode
    MOD_H,          // hybrid mode
    MOD_A           // automatic mode
}mode = MOD_M;

unsigned int index = 0;
unsigned char buffer[10];

unsigned int distLeft = 0;
unsigned int distRight = 0;
unsigned int distCenter = 0;

void configPorts();
void configSerial();
void configUltrasonic();
void configInterrupts();

/**
 * Starts the initial state of motors and sensors.
 */
void setInitialState();

/**
 * Object detect with ultrasonic sensor
 */
unsigned int ultrasonic(void);

/**
 * Interrupt treatment of rs232.
 */
void interrupt serialHandler();

void denialAction();
void happinessAction();
void lookLeftAction();
void lookRightAction();

void main(void) {
    
    // initial settings     
    configMotor();    
    configPorts();
    configSerial();
    configUltrasonic();
    configInterrupts();
    
    setInitialState();
    
    while (1) {
        
        if (mode == MOD_A) {
        
            distLeft = 0;
            distRight = 0;
        
            distCenter = ultrasonic();        
        
            if (distCenter <= 15) {
                
                setDirection(STOP);
                ALERT = 1;

                servoRotate(S_LEFT);
                __delay_ms(10);
                distLeft = ultrasonic();

                servoRotate(S_RIGHT);
                __delay_ms(10);
                distRight = ultrasonic();
                
                servoRotate(S_CENTER);

                printf("left: %u\n", distLeft);
                printf("right: %u\n", distRight);
                printf("center: %u\n", distCenter);
                
                if (distLeft > distRight) {
                    setDirection(LEFT);
                } else {
                    setDirection(RIGHT);
                }

                __delay_ms(300);
                setDirection(FRONT);
                ALERT = 0;
            }
            
        }
       
    }
    
    return;
}

void configPorts() {
    TRISB = 0x00;
    TRISD = 0b00000010;
}

void configSerial() {
    serialBegin();
}

void configUltrasonic() {
    T1CON = 0b00100000;
}

void configInterrupts() {
    INTCONbits.GIE = 0x01;      // Enables all unmasked interrupts
    INTCONbits.PEIE = 0x01;     // Enables all unmasked peripheral interrupts
    PIR1bits.RCIF = 0x00;       // USART Receive Interrupt Flag bit (buffer is empty)
    PIE1bits.RCIE = 0x01;       // USART Receive Interrupt Enable bit
}

void setInitialState() {
    //start stopped
    setDirection(STOP);
    
    // set servo position
    servoRotate(S_CENTER);
    
    ALERT = 0x00;
    LIGHT_L = 0x00;
    LIGHT_R = 0x00;
}

unsigned int ultrasonic(void) {
    
    unsigned long duration = 0;
    unsigned int distance_cm = 0, TMR = 0;
    
    // clear Timer1
    TMR1H = 0x00;           
    TMR1L = 0x00;
    
    // trigger
    TRIG = 0x00;
    __delay_us(2);        
    TRIG = 0x01;
    __delay_us(10);
    TRIG = 0x00;
    
    // echo
    while(!ECHO);
    T1CONbits.TMR1ON = 0x01;            // ON Counter
    while(ECHO);
    T1CONbits.TMR1ON = 0x00;

    TMR = (unsigned int) TMR1H << 8;
    TMR = TMR + TMR1L;                  // Combine 2x counter byte into single integer

    duration = (TMR/10) * 8;            // Duration Formula = TMR * 0.2us(Clock speed) * 4 (Timer Prescale)

    distance_cm = duration / 58 ;       // Refer HC-SR04 Datasheet
    
    return distance_cm;
}

void interrupt serialHandler() {
    
    if (serialAvailable()) {
        
        buffer[index] = serialRead();
        
        if (buffer[index] == '&') {
            
            /* Mode options  */
            
            if (buffer[0] == '$' && buffer[1] == 'M' && buffer[2] == 'O' && buffer[3] == 'D' && buffer[4] == 'M') {
                mode = MOD_M;
            }
            
            if (buffer[0] == '$' && buffer[1] == 'M' && buffer[2] == 'O' && buffer[3] == 'D' && buffer[4] == 'A') {
                mode = MOD_A;
                setDirection(FRONT);
            }
            
            /* Engine controls */
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'S') {
                if (mode == MOD_M) setDirection(STOP);
            }
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'F') {
                if (mode == MOD_M) setDirection(FRONT);
            }
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'L') {
                if (mode == MOD_M) {
                    setDirection(LEFT);
                    __delay_ms(100);
                    
                    if (isMove) {
                        setDirection(FRONT);
                    } else {
                        setDirection(STOP);
                    }                    
                }
            }
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'R') {
                if (mode == MOD_M) {
                    setDirection(RIGHT);
                    __delay_ms(100);
                    
                    if (isMove) {
                        setDirection(FRONT);
                    } else {
                        setDirection(STOP);
                    }
                }
            }
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'B') {
                setDirection(BACK);
                __delay_ms(200);
                setDirection(STOP);
            }
            
            /* Actions and other commands */
            
            if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'C' && buffer[3] == 'T' && buffer[4] == 'D') {
                if (mode == MOD_M) {
                    denialAction();
                }
            }
            
            if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'C' && buffer[3] == 'T' && buffer[4] == 'H') {
                if (mode == MOD_M) {
                    happinessAction();
                }
            }
            
            if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'C' && buffer[3] == 'T' && buffer[4] == 'L' && buffer[5] == 'L') {
                if (mode == MOD_M) {
                    lookLeftAction();
                }
            }
            
            if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'C' && buffer[3] == 'T' && buffer[4] == 'L' && buffer[5] == 'R') {
                if (mode == MOD_M) {
                    lookRightAction();
                }
            }
            
            index = 0;
        } else {
            index++;
        }
        
        serialClean();        
    }
    
}

/* Mode options */



/* Actions and other commands */

void denialAction() {
    servoRotate(S_LEFT);
    servoRotate(S_RIGHT);
    servoRotate(S_CENTER);
}

void happinessAction() {
    setDirection(LEFT);
    denialAction();
    __delay_ms(100);
    setDirection(RIGHT);
    denialAction();
    __delay_ms(100);
    setDirection(STOP);
}

void lookLeftAction() {
    servoRotate(S_LEFT);
    __delay_ms(100);
    servoRotate(S_CENTER);
}

void lookRightAction() {
    servoRotate(S_RIGHT);
    __delay_ms(100);
    servoRotate(S_CENTER);
}