#ifndef __BUTTON_H
#define __BUTTON_H

#include "msp430f1611.h"
#include "stdint.h"

typedef void(*btnCallBackFunc)(void);

btnCallBackFunc btnCb;

void btn_setCallbacks(btnCallBackFunc func)
{
	btnCb = func;
	P2DIR &= ~0x80;
	P2OUT |= 0x80;
	P2IES |= 0x80;
	P2IE |= 0x80;
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void) {
	P2IFG &= ~0x80;
	btnCb();
}

#endif