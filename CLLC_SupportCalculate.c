#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "CLLC_Function.h"
#include "CLLC_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC���ת��
void AdcResCon(void)
{
//�����ⲿ����
	extern Uint16 mode;
	extern Uint16 AdcResults[3];
	extern float32 HighVol,LowCur,LowVol;
//ת������
	switch(mode)
	{
	case 0:
		HighVol=(6.0*(AdcResults[0]-2)/4095.0-3.0)/50*0.4*68*1000;
		LowCur=(6.0*(AdcResults[1]-2)/4095.0-3.0)/200*1000;
		break;
	case 1:
		HighVol=(6.0*(AdcResults[0]-2)/4095.0-3.0)/50*0.4*68*1000;
		LowCur=(6.0*(AdcResults[1]-2)/4095.0-3.0)/200*1000;
		break;
	case 2:
		HighVol=(6.0*(AdcResults[0]-2)/4095.0-3.0)/50*0.4*68*1000;
		break;
	}
}
