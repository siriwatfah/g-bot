/* 
 * Library for management of the serial comunications of PIC16f877A
 * Author: Adilson Pavan
 */

#ifndef _SERIAL_H
#define	_SERIAL_H

#include <xc.h>
#include <stdio.h>

void serialBegin(void);
void serialWrite(const char *str);
char serialRead();
int serialAvailable();

void putch(unsigned char c);

#endif	/* _SERIAL_H */

