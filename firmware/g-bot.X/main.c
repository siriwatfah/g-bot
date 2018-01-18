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
#include "pwm.h"

enum mode_opt {
    MODM = 1,      // manual mode
    MODH,          // hybrid mode
    MODA           // automatic mode
}mode = MODM;

unsigned int index = 0;
unsigned char buffer[10];

unsigned int distLeft = 0;
unsigned int distRight = 0;
unsigned int distCenter = 0;

void configPWM();
void configPorts();
void configSerial();
void configUltrasonic();
void configInterrupts();

/**
 * Starts the initial state of motors and sensors.
 */
void setInitialState();

/**
 * Set direction of the engines 
 * @param direction integer with direction of engine
 */
void setDirection(int direction);

/**
 * Object detect with ultrasonic sensor
 */
unsigned int ultrasonic(void);

/**
 * Interrupt treatment of rs232.
 */
void interrupt serialHandler();


void main(void) {
    
    // initial settings     
    configPWM();    
    configPorts();
    configSerial();
    configUltrasonic();
    configInterrupts();
    
    setInitialState();
    
    while (1) {
        
        distLeft = 0;
        distRight = 0;
        
        distCenter = ultrasonic();
        printf("%u\n", distCenter);
        
        if (distCenter <= 15) {
            ALERT = 1;
            setDirection(STOP);

            if (mode == MODA) {
                servoRotate(S_LEFT);
                distLeft = ultrasonic();

                servoRotate(S_RIGHT);
                distRight = ultrasonic();
                servoRotate(S_CENTER);

                setDirection(BACK);
                __delay_ms(100);
                
                if (distLeft > distRight) {
                    setDirection(LEFT);
                } else {
                    setDirection(RIGHT);
                }

                __delay_ms(300);
                setDirection(FRONT);
                ALERT = 0;

            } else {
                __delay_ms(100);
                ALERT = 0;
            }
        }
       
    }
    
    return;
}

void configPWM() {
    PWM1_Init(5000);
    PWM2_Init(5000);    
    PWM1_Start();
    PWM2_Start();
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

void setDirection(int direction) {
    
    if (direction == STOP) {
        
        //right motor
        IN1 = 0x00;
        IN2 = 0x00;
        
        //left motor
        IN3 = 0x00;
        IN4 = 0x00;
        
        PWM1_Duty(0);
        PWM2_Duty(0);
        
    } else if (direction == FRONT) {
        
        //right motor
        IN1 = 0x00;
        IN2 = 0x01;
        
        //left motor
        IN3 = 0x00;
        IN4 = 0x01;
        
        PWM1_Duty(880);
        PWM2_Duty(1000);
        
    } else if (direction == BACK) {
        
        //right motor
        IN1 = 0x01;
        IN2 = 0x00;
        
        //left motor
        IN3 = 0x01;
        IN4 = 0x00;
        
        PWM1_Duty(1000);
        PWM2_Duty(1200);
        
    } else if (direction == LEFT) {
        
        //right motor
        IN1 = 0x00;
        IN2 = 0x01;
        
        //left motor
        IN3 = 0x00;
        IN4 = 0x00;
        
        PWM1_Duty(800);
        PWM2_Duty(0);
        
    } else if (direction == RIGHT) {
        
        //right motor
        IN1 = 0x00;
        IN2 = 0x00;
        
        //left motor
        IN3 = 0x00;
        IN4 = 0x01;
        
        PWM1_Duty(0);
        PWM2_Duty(1000);
    }
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
            
            if (buffer[0] == '$' && buffer[1] == 'M' && buffer[2] == 'O' && buffer[3] == 'D' && buffer[4] == 'M') {
                mode = MODM;
            }
            
            if (buffer[0] == '$' && buffer[1] == 'M' && buffer[2] == 'O' && buffer[3] == 'D' && buffer[4] == 'A') {
                mode = MODA;
                setDirection(FRONT);
            }
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'S') {
                if (mode == MODM) setDirection(STOP);
            }
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'F') {
                if (mode == MODM) setDirection(FRONT);
            }
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'L') {
                if (mode == MODM) {
                    setDirection(LEFT);
                    __delay_ms(100);
                    setDirection(FRONT);
                }
            }
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'R') {
                if (mode == MODM) {
                    setDirection(RIGHT);
                    __delay_ms(100);
                    setDirection(FRONT);
                }
            }
            
            if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'I' && buffer[3] == 'R' && buffer[4] == 'B') {
                setDirection(BACK);                
            }
            
            index = 0;
        } else {
            index++;
        }
        
        serialClean();        
    }
    
}