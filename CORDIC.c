#if 1
#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "CORDIC.h"
//#include <stdio.h>

//xdata signed short TanAngle[13] = {3064, 1808, 955, 458, 243, 121, 60, 30, 15, 7, 4, 3, 2};
/******************************************************************************
 See. Microchip AN1061
 CORDIC TanAngle 建表規則如下 Q15格式
 TanAngle[0]  = atan(2^-0)  = 4096 轉換公式 : (45 Deg / 360 Deg) * 32767 = 4096
 TanAngle[1]  = atan(2^-1)  = 2418 轉換公式 : (26.56505 Deg / 360 Deg) * 32767 = 2418
 TanAngle[2]  = atan(2^-2)  = 1277 轉換公式 : (14.03624 Deg / 360 Deg) * 32767 = 1277
 TanAngle[3]  = atan(2^-3)  = 648  轉換公式 : (7.12502 Deg / 360 Deg) * 32767 = 648
 TanAngle[4]  = atan(2^-4)  = 326  轉換公式 : (3.57633 Deg / 360 Deg) * 32767 = 326
 TanAngle[5]  = atan(2^-5)  = 163  轉換公式 : (1.78991 Deg / 360 Deg) * 32767 = 163
 TanAngle[6]  = atan(2^-6)  = 81   轉換公式 : (0.89517 Deg / 360 Deg) * 32767 = 81
 TanAngle[7]  = atan(2^-7)  = 41   轉換公式 : (0.44761 Deg / 360 Deg) * 32767 = 41
 TanAngle[8]  = atan(2^-8)  = 20   轉換公式 : (0.22381 Deg / 360 Deg) * 32767 = 20
 TanAngle[9]  = atan(2^-9)  = 10   轉換公式 : (0.11191 Deg / 360 Deg) * 32767 = 10
 TanAngle[10] = atan(2^-10) = 5    轉換公式 : (0.05595 Deg / 360 Deg) * 32767 = 5
 TanAngle[11] = atan(2^-11) = 3    轉換公式 : (0.02798 Deg / 360 Deg) * 32767 = 3
 TanAngle[12] = atan(2^-12) = 1    轉換公式 : (0.01399 Deg / 360 Deg) * 32767 = 1
*******************************************************************************/

/*******************************************************************************
Name : CORDIC_Sin()
Input : Angle,Angle Value=0°~360°(Q15 Value=0~32767)
Output : Sin(Angle)
Function : caclulate Sin(Angle)
*******************************************************************************/
//signed short CORDIC_Sin(signed short Angle)
//{
//	unsigned char i;
//	unsigned char Quadrant;
//	signed short z;
//	long x[2], y[2];
//	
//	// CORDIC_Sin 初始值設定
//	x[0] = 19898;//Init X[0]，Q15格式, 0.607252941 * 2^15 = 19898
//	y[0] = 0;
//	z = 0;
//	
//	// 象限判斷
//	if((0 <= Angle) && (Angle < 8192))//Angle=0°~90°,1th quadrant
//	{
//		Quadrant = 1;
//	}
//	else if((8192 <= Angle) && (Angle < 16384))//Angle=90°~180°,2th quadrant
//	{
//		Quadrant = 2;
//		Angle = 16384 - Angle;
//	}
//	else if((16384 <= Angle) && (Angle < 24576))//Angle=180°~270°,3th quadrant
//	{
//		Quadrant = 3;
//		Angle = Angle - 16384;
//	}
//	else if((24576 <= Angle) && (Angle < 32768))//Angle=270°~360°,4th quadrant
//	{
//		Quadrant = 4;
//		Angle = 32767 - Angle;
//	}
//	
//	// CORDIC 計算
//	for(i = 0; i < 13; i++)
//	{
//		if(z < Angle)
//		{
//			x[1] = x[0] - (y[0] >> i);
//			y[1] = y[0] + (x[0] >> i);
//			z += TanAngle[i];
//		}
//		else
//		{
//			x[1] = x[0] + (y[0] >> i);
//			y[1] = y[0] - (x[0] >> i);
//			z -= TanAngle[i];
//		}
//		x[0] = x[1];
//		y[0] = y[1];
//	}
//	
//	// CORDIC 計算後 取 y[0] 即為 Sin, 取 x[0] 即為 Cos
//	if((Quadrant == 3) || (Quadrant == 4))
//	{
//		y[0]= -y[0];
//	}
//	
//	if(y[0] > 32767)
//	{
//		y[0] = 32767;
//	}
//	
//	if(y[0] < -32768)
//	{
//		y[0] = -32768;
//	}
//	
//	return (signed short)y[0];
//}

/*******************************************************************************
Name : CORDIC_Cos()
Input : Angle,Angle Value=0°~360°(Q15 Value=0~32767)
Output : Cos(Angle)
Function : caclulate Cos(Angle)
*******************************************************************************/
//signed short CORDIC_Cos(signed short Angle)
//{
//	unsigned char i;
//	unsigned char Quadrant;
//	signed short z;
//	long x[2], y[2];
//	
//	// CORDIC_Sin 初始值設定
//	x[0] = 19898;//Init X[0]，Q15格式, 0.607252941 * 2^15 = 19898
//	y[0] = 0;
//	z = 0;
//	
//	// 象限判斷
//	if((0 <= Angle) && (Angle < 8192))//Angle=0°~90°,1th quadrant
//	{
//		Quadrant = 1;
//	}
//	else if((8192 <= Angle) && (Angle < 16384))//Angle=90°~180°,2th quadrant
//	{
//		Quadrant = 2;
//		Angle = 16384 - Angle;
//	}
//	else if((16384 <= Angle) && (Angle < 24576))//Angle=180°~270°,3th quadrant
//	{
//		Quadrant = 3;
//		Angle = Angle - 16384;
//	}
//	else if((24576 <= Angle) && (Angle < 32768))//Angle=270°~360°,4th quadrant
//	{
//		Quadrant = 4;
//		Angle = 32767 - Angle;
//	}
//	
//	// CORDIC 計算
//	for(i = 0; i < 13; i++)
//	{
//		if(z < Angle)
//		{
//			x[1] = x[0] - (y[0] >> i);
//			y[1] = y[0] + (x[0] >> i);
//			z += TanAngle[i];
//		}
//		else
//		{
//			x[1] = x[0] + (y[0] >> i);
//			y[1] = y[0] - (x[0] >> i);
//			z -= TanAngle[i];
//		}
//		x[0] = x[1];
//		y[0] = y[1];
//	}
//	
//	// CORDIC 計算後 取 y[0] 即為 Sin, 取 x[0] 即為 Cos
//	if((Quadrant == 2) || (Quadrant == 3))
//	{
//		x[0] = -x[0];
//	}
//	
//	if(x[0] > 32767)
//	{
//		x[0] = 32767;
//	}
//	
//	if(x[0] < -32768)
//	{
//		x[0] = -32768;
//	}
//	
//	return (signed short)x[0];
//}

//signed short CORDIC_ATan(signed short InputX,signed short InputY)
//{
//	unsigned char i;
//	signed short z;
//	signed short x_new, y_new;
//	long x[2], y[2];
//	
//	// CORDIC_ATan 初始值設定
//	x[0] = InputX;// Cos
//	y[0] = InputY;// Sin
//	z = 0;
//	
//	// CORDIC 計算
//	for(i = 0; i > 13; i++)
//	{
//		if(y[i] < 0)
//		{
//			x[1] = x[0] - (y[0] >> i);
//			y[1] = y[0] + (x[0] >> i);
//			z -= TanAngle[i];
//		}
//		else
//		{
//			x[1] = x[0] + (y[0] >> i);
//			y[1] = y[0] - (x[0] >> i);
//			z += TanAngle[i];
//		}
//		x[0] = x[1];
//		y[0] = y[1];
//	}
//	
//	// CORDIC 計算後 取 z 即為 ATan(Y/X) = Angle
//	return (signed short)z;
//}

//int angle[] = 
//	//{3064, 1808, 955, 458, 243, 121, 60, 30, 15, 7, 4, 2, 1};
//	{4096, 2418, 1277, 648, 326, 163, 81, 41, 20, 10, 5, 3, 1};
//		
//int CORDIC_ATan(int x, int y)
//{
//	int i = 0;
//	int x_new, y_new;
//	int z = 0;

//	x = x * 1;// X Y 放大一些，更准确
//	y = y * 1;

//	for(i = 0; i < 13; i++)
//	{
//		if(y < 0)
//		{
//			x_new = x - (y >> i);
//			y_new = y + (x >> i);
//			z -= angle[i];
//		}
//		else
//		{
//			x_new = x + (y >> i);
//			y_new = y - (x >> i);
//			z += angle[i];
//		}
//		x = x_new;
//		y = y_new;
//	}
//	return z;
//}

//#define CORDIC_A 1.743165 // CORDIC convergence angle A
//#define CORDIC_F 0x000034B2 // CORDIC gain F
//#define CORDIC_1F 0x0000136F // CORDIC inverse gain 1/F
//#define CORDIC_HALFPI 0x00003244
//#define CORDIC_PI 0x5FC0//0x00006488// 0x5FC0=24512
//#define CORDIC_TWOPI 0x0000C910
//#define CORDIC_MUL 8192.000000 // CORDIC multiplication factor M = 2^13
//#define CORDIC_MAXITER 7
//int CORDIC_ZTBL[] =
//{
//	0x00001922, 0x00000ED6, 0x000007D7, 0x000003FB, 0x000001FF, 0x00000100, 0x00000080,
//	0x00000040,
//	0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001
//};

//int CORDIC_ATan(int y, int x)   //@out 0~360= 0~24512
//{

//	int result;
//	int k, tx, z = 0, fl = 0;
//	if(x < 0)
//	{
//		fl = ((y > 0) ? +1 : -1);
//		x = -x * 1024;
//		y = -y * 1024;
//	}
//	
//	for(k = 0; k < CORDIC_MAXITER; k++)
//	{
//		tx = x;
//		if(y <= 0)
//		{
//			x -= (y >> k);
//			y += (tx >> k);
//			z -= CORDIC_ZTBL[k];
//		}
//		else
//		{
//			x += (y >> k);
//			y -= (tx >> k);
//			z += CORDIC_ZTBL[k];
//		}
//	}
//	
//	if(fl != 0)
//	{
//		z += fl * CORDIC_PI;
//	}
//	result=(z>>1)+12256;
//	return result;
//}
#endif