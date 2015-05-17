#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "CLLC_Function.h"
#include "math.h"
#include "CLLC_Parameter.h"

float32 LowCurMean;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC���ת��
void AdcResCon(void)
{
//�����ⲿ����
	extern Uint16 AdcResults[3];
	extern float32 HighVol,HighCur,LowVol;
//ת������
	HighVol=0.99*(6.0*(AdcResults[0]-9)/4095.0-3.0)/50*0.4*120*1000;
	HighCur=(6.0*(AdcResults[1]-12)/4095.0-3.0)/26*200;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ģʽת������
void ModeSwitch(void)
{
	extern Uint16 mode;
	extern float32 HighVol;

	if(HighVol>380)
		mode=1;

/*	if(GpioDataRegs.GPCDAT.bit.GPIO69==0)
	{
		EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
		EPwm1Regs.DBCTL.all=0x0;
		mode=1;
	}
	else
	{
		EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
		EPwm1Regs.DBCTL.all=0xb;
		mode=2;
	}
	*/
}
