
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef _PWM_H
#define	_PWM_H

#include <xc.h>
#include <stdio.h>
#include "xtal.h"

#define TMR2PRESCALE 4

long freq;

int PWM_Max_Duty();
PWM1_Init(long fre);
PWM2_Init(long fre);
PWM1_Duty(unsigned int duty);
PWM2_Duty(unsigned int duty);
PWM1_Start();
PWM1_Stop();
PWM2_Start();
PWM2_Stop();

#endif	/* _PWM_H */

