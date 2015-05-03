#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "CLLC_Function.h"
#include "CLLC_Parameter.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//升压电压环PID计算
float32 PIDHighVol_Cal(float32 HighVol,float32 T)
{
	static float32 Kp=200,Ti=0.05,Td=0; //200，0.001 //200，0.0005
	static float32 a0,a1,a2;
	static float32 Ek=0,Ek_1=0,Ek_2=0;
	static float32 OutPut,OutPut_1=150000;
//计算误差
	Ek=-380+HighVol;
//计算参数
	a0=Kp*(1+T/Ti+Td/T);
	a1=Kp*(1+2*Td/T);
	a2=Kp*(Td/T);
//计算输出
	OutPut=OutPut_1+a0*Ek-a1*Ek_1+a2*Ek_2;
//限制最高最低
	if(OutPut>MAX_FREQ)
		OutPut=MAX_FREQ;
	if(OutPut<MIN_FREQ)
		OutPut=MIN_FREQ;
//传递
	OutPut_1=OutPut;
	Ek_1=Ek;
	Ek_2=Ek_1;
//输出结果
	return OutPut;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//降压电流PID计算
float32 PIDLowCur_Cal(float32 LowCur,float32 T)
{
	static float32 Kp=200,Ti=0.0005,Td=0;
	static float32 a0,a1,a2;
	static float32 Ek=0,Ek_1=0,Ek_2=0;
	static float32 OutPut,OutPut_1=150000;
//计算误差
	Ek=-0.5+LowCur;
//计算参数
	a0=Kp*(1+T/Ti+Td/T);
	a1=Kp*(1+2*Td/T);
	a2=Kp*(Td/T);
//计算输出
	OutPut=OutPut_1+a0*Ek-a1*Ek_1+a2*Ek_2;
//限制最高最低
	if(OutPut>MAX_FREQ)
		OutPut=MAX_FREQ;
	if(OutPut<MIN_FREQ)
		OutPut=MIN_FREQ;
//传递
	OutPut_1=OutPut;
	Ek_1=Ek;
	Ek_2=Ek_1;
//输出结果
	return OutPut;
}


