#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "IFEC2015_DCDC_Function.h"
#include "IFEC2015_DCDC_Parameter.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GPIO��ʼ��
//˵����GPIO8��������
void GPIOInitial(void)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO8=0;
	GpioCtrlRegs.GPAPUD.bit.GPIO8=0;
	GpioCtrlRegs.GPADIR.bit.GPIO8=1;
	EDIS;
}


