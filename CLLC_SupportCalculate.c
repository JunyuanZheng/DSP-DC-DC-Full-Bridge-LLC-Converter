#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "CLLC_Function.h"
#include "math.h"
#include "CLLC_Parameter.h"

float32 LowCurMean;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC结果转换
void AdcResCon(void)
{
//生命外部变量
	extern Uint16 AdcResults[3];
	extern float32 HighVol,HighCur,LowVol;
//转换函数
	HighVol=0.99*(6.0*(AdcResults[0]-9)/4095.0-3.0)/50*0.4*120*1000;
	HighCur=(6.0*(AdcResults[1]-12)/4095.0-3.0)/26*200;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//模式转换控制
void ModeSwitch(void)
{
	extern Uint16 mode;
	extern float32 HighVol;

	if(HighVol>380)
		mode=1;

/*	if(GpioDataRegs.GPCDAT.bit.GPIO69==0)
	{
		EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
		EPwm1Regs.DBCTL.all=0x0;
		mode=1;
	}
	else
	{
		EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
		EPwm1Regs.DBCTL.all=0xb;
		mode=2;
	}
	*/
}
