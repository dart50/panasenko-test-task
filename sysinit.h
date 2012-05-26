#ifndef __sysinit_h
#define __sysinit_h

#define RED_LED _LATA9
#define GREEN_LED _LATA10
#define ON 0
#define OFF 1
#define BUTTON _RC0
#define MOTOR _RB0
#define STOP 0
#define START 1
#define HighSpeed 30000
#define LowSpeed 10000
#define Lenght 5000

extern void IOInit(void);
extern void UARTInit(unsigned int);
extern void ADCInit(void);
extern void PWMInit(void);
extern void InitSysTimer(int);

#define Pr256 (3<<4)				/*timer prescaler*/
#define SysClk 16000				// kHz

//extern void UARTInit(u_int);




#endif
