/* 
 * Library for management of the serial comunications of PIC16f877A
 * Author: pavan
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

