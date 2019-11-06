//---------------------------------------------------------------------------//
// Head Files																																 //
//---------------------------------------------------------------------------//
#include "ET1202_SFR.h"
#include "Motor.h"
#include "PI_CTRL.h"
#if (POWER_CONTROL == 1)
//---------------------------------------------------------------------------//
// Global Variable Parameters																								 //
//---------------------------------------------------------------------------// 
xdata tPIParm PIParm_Watt;	// Structure definition for Watt

//---------------------------------------------------------------------------//
// Subroutine																																 //
//---------------------------------------------------------------------------//
void InitPI(tPIParm *pParm);
void CalcPI(tPIParm *pParm);
void InitPI_ALL(void);

//---------------------------------------------------------------------------//
// Initial PI																																 //
//---------------------------------------------------------------------------// 
void InitPI_ALL(void)
{
		PIParm_Watt.InRefMin		= WattRefMin;
		PIParm_Watt.InRefMax		= WattRefMax;
		PIParm_Watt.OutMax			= WattOutMax;
		PIParm_Watt.OutMin			= WattOutMin;
		PIParm_Watt.Kp					= WattKp;
		PIParm_Watt.Nko					= WattNko;
		PIParm_Watt.Ki					= WattKi;
		PIParm_Watt.Kd					= WattKd;
		PIParm_Watt.Kc					= WattKc;
		PIParm_Watt.SampingFre	= WattFreInHz;
		PIParm_Watt.SampingTime	= (32768 / PIParm_Watt.SampingFre);
}

void InitPI(tPIParm *pParm)
{
		pParm->Sum				= 0;
//		pParm->SumErr			= 0;
		pParm->ErrOld			= 0;
		pParm->POut				= 0;
		pParm->IOut				= 0;
		pParm->DOut				= 0;
		pParm->Out				= 0;
}


//---------------------------------------------------------------------------//
// PI Calculate																															 //
//---------------------------------------------------------------------------// 
void CalcPI(tPIParm *pParm)
{
		if(pParm->InRef >= pParm->InRefMax) pParm->InRef = pParm->InRefMax;
		if(pParm->InRef <= pParm->InRefMin) pParm->InRef = pParm->InRefMin;
	
		pParm->Err         = pParm->InRef - pParm->InMeas;
		pParm->ErrDelta    = 0;//(signed long)(pParm->Err - pParm->ErrOld) * pParm->SampingFre;
		
		pParm->POut        = (pParm->Kp * (signed long)pParm->Err) >> 15;
		pParm->POut        = pParm->POut << pParm->Nko;
		if(pParm->POut >= 32767) pParm->POut = 32767;
		pParm->IOut        = (pParm->Sum) >> 15;
		pParm->DOut        = 0;//(pParm->Kd * pParm->ErrDelta) >> 15;
		pParm->U           = (pParm->POut + pParm->IOut + pParm->DOut);

		if(pParm->U      > pParm->OutMax)     pParm->Out  = pParm->OutMax;
		else if(pParm->U < pParm->OutMin)     pParm->Out  = pParm->OutMin;
		else                                  pParm->Out  = pParm->U;
		
		pParm->Excess      = pParm->U - pParm->Out;
		pParm->Sum         = pParm->Sum + (pParm->Ki * (signed long)pParm->Err) - (pParm->Kc * (signed long)pParm->Excess);
		pParm->ErrOld      = pParm->Err;
//	if(pParm->qInRef == 0 && pParm->qInMeas == 0)	pParm->qSum = pParm->qSumErr = 0;
}
#endif
//---------------------------------------------------------------------------//