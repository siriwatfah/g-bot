/*
 * File:   serial.c
 * Author: pavan
 *
 * Created on September 26, 2017, 9:57 PM
 */

#include "serial.h"

void serialBegin(void) {
    RCSTAbits.SPEN = 1;     // serial port enable
    TXSTAbits.TXEN = 1;     // trasmit enable
    RCSTAbits.CREN = 1;     // receive enable
    TXSTAbits.SYNC = 0;     // usart asynchronous mode
    TXSTAbits.BRGH = 1;     // high speed baud rate
    SPBRG = 129;            // baudrate in 9600
    
    TXSTAbits.TX9 = 0;      // 8-bit transmission
    RCSTAbits.RX9 = 0;      // 8-bit reception
    
    TRISC = 0x80;
}

void serialWrite(const char *str) {
    while (*str != '\0') {
        putch(*str);
        str++;
    }
}

char serialRead() {    
    while (!RCIF);
    return RCREG;    
}

int serialAvailable(){
    
    // check for over run error
    
    if (OERR) { 
        CREN = 0;
        CREN = 1; //Reset CREN
    }
    
    return PIR1bits.RCIF;
}

void serialClean() {
    PIR1bits.RCIF = 0;
}

void putch(unsigned char c) {
    while (!TRMT);
    TXREG = c;
}

