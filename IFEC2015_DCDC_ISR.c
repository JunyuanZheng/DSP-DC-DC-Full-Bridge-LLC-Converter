#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include <math.h>
#include "IFEC2015_DCDC_Function.h"
#include "IFEC2015_DCDC_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM3中断，升压运算
interrupt void epwm3_isr(void)
{
	extern int16 DCDCMode,DCDCMode_1;
	extern float32 DCDCSubMode;
	extern float32 BurstModeDutyCycle;

	extern float32 DCBusVoltage,BatteryVoltage,BatteryCurrent;
	extern Uint16 AdcResults[3];

	extern float32 GUIComposer_SwitchingFrequency;

//本函数静态变量
	static Uint16 EPwm3_TIMER_TBPRD;
	static float32 SwitchingFrequency;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//运算
//读取ADC结果
	ReadAdc(AdcResults);
	AdcResultConvert();
//软起动控制
	if(DCDCMode==-1)
		SoftStartModeControl(); //有点问题
//运算
	switch(DCDCMode)
	{
	case -1:
		SwitchingFrequency=200000;
		break;
	case 0:
		if(DCDCSubMode==0.1) //BurstMode
		{
			static Uint16 i;
			if(i==0)
			{
				SwitchingFrequency=PIDBatteryToDCBus(DCBusVoltage,0.001); //最高频率200K，最低频率60K
				i=ceil(SwitchingFrequency/1000);
			}
			if(i<floor(BurstModeDutyCycle*SwitchingFrequency/1000))
			{
				EPwm3Regs.AQCSFRC.all=PWMS_FRC_DISABLE;
				EPwm3Regs.DBCTL.all=0xb;
     		}
			else
			{
				EPwm3Regs.AQCSFRC.all=PWMS_ALBL;
				EPwm3Regs.DBCTL.all=0x0;
			}
			i--;
		}
		else
			SwitchingFrequency=PIDBatteryToDCBus(DCBusVoltage,3/SwitchingFrequency); //升压最高频率200K，最低频率60K
		break;
	}
	GUIComposer_SwitchingFrequency=SwitchingFrequency/1000;
//控制PWM频率
	EPwm3_TIMER_TBPRD=150*1000000/(2*SwitchingFrequency);
	EPwm3Regs.TBPRD=EPwm3_TIMER_TBPRD;
	EPwm3Regs.CMPA.half.CMPA=EPwm3_TIMER_TBPRD/2;
//清除中断
	EPwm3Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM4中断，升压运算
interrupt void epwm4_isr(void)
{
	EPwm4Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
