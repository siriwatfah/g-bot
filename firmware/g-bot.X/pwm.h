
#ifndef _PWM_H
#define	_PWM_H

#include <xc.h>
#include <stdio.h>
#include "xtal.h"

#define TMR2PRESCALE 4

long freq;

int PWM_Max_Duty();
void PWM1_Init(long fre);
void PWM2_Init(long fre);
void PWM1_Duty(unsigned int duty);
void PWM2_Duty(unsigned int duty);
void PWM1_Start();
void PWM1_Stop();
void PWM2_Start();
void PWM2_Stop();

#endif	/* _PWM_H */

