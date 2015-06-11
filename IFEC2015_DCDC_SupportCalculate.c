#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "IFEC2015_DCDC_Function.h"
#include "IFEC2015_DCDC_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC结果转换
void AdcResultConvert(void) //需要校准
{
//生命外部变量
	extern Uint16 AdcResults[3];
	extern float32 DCBusVoltage,BatteryVoltage,BatteryCurrent;
//转换函数
	DCBusVoltage=(6.0*(AdcResults[0]-9)/4095.0-3.0)/49.8*0.4*120*1000;
	BatteryVoltage=(6.0*(AdcResults[1]-16)/4095.0-3.0)/99.8*0.4*49.9*1000;
	BatteryCurrent=(6.0*(AdcResults[2]-6)/4095.0-3.0)/25*1000;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//软起动模式转换
void SoftStartModeControl()
{
	extern int16 DCDCMode;
	extern float32 DCDCSubMode;
	extern float32 BurstModeDutyCycle;

	extern float32 DCBusVoltage;

	if(DCBusVoltage<360)
	{
		DCDCMode=-1; //后面加一句控制端口
		GpioDataRegs.GPADAT.bit.GPIO8=0;
	}
	else
	{
		DCDCMode=0;
		GpioDataRegs.GPADAT.bit.GPIO8=1;
		DCDCSubMode=0.1;
		BurstModeDutyCycle=0.05;
	}
}

