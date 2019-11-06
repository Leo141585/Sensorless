#ifndef _PI_SETUP_
#define _PI_SETUP_

//---------------------------------------------------------------------------//
// Head Files																																 //
//---------------------------------------------------------------------------//
//#include	"CTRL_SETTING.h"
#include "Motor.h"

//---------------------------------------------------------------------------//
// PID Controller																														 //
//---------------------------------------------------------------------------//
// Watt PI Parameters
#define		WattRef											((signed int)(32767 * 0.1))

#define		WattFreInHz									(50)
#define		WattFreSet									(1000 / WattFreInHz)
#define		WattRefMin									(0.0 * I_AMPLIFIER)
#define		WattRefMax									(10.0 * I_AMPLIFIER)
#define		WattOutMin									(0.0 * I_AMPLIFIER)
#define		WattOutMax									(10.0 * I_AMPLIFIER)

#define		WattKp											((signed int)(32767 * 0.25))
#define		WattNko											(1)
#define		WattKi											((signed int)(32767 * 0.1))
#define		WattKd											((signed int)(32767 * 0.0))
#define		WattKc											((signed int)(32767 * 0.999))
	
//-------------------------------------//
// Variables for Global
typedef struct 
{
		signed int	InRefMax;
		signed int	InRefMin;
		signed int	InRef;
		signed int	InMeas;
		signed int	Err;
		signed int	ErrOld;
		signed int	ErrDelta;
		signed int	SumErr;
		signed long	Sum;
		signed long	POut;
		signed long	IOut;
		signed long	DOut;
		signed long	U;
		signed int	Out;
		signed int	Excess;
		signed int	OutMax;
		signed int	OutMin;
		signed int	Kp;
		signed int	Nko;
		signed int	Ki;
		signed int	Kd;
		signed int	Kc;
		signed char	Cnt;
		signed int	IOutClc;
		signed int	SampingFre;
		signed int	SampingTime;
}xdata tPIParm;
extern xdata tPIParm	PIParm_Watt;	// Structure definition for Watt

//-------------------------------------//
// Subroutines for Global
extern void InitPI(tPIParm *pParm);
extern void CalcPI(tPIParm *pParm);
extern void InitPI_ALL(void);
//-------------------------------------//
#endif /* _PI_SETUP_ */
