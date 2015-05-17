#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "CLLC_Function.h"
#include "CLLC_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//定时器初始化
void TimerInit(void)
{
	InitCpuTimers();
#if(CPU_FRQ_150MHZ)
	ConfigCpuTimer(&CpuTimer0,150,1000000);
#endif
#if(CPU_FRQ_100MHZ)
	ConfigCpuTimer(&CpuTimer0,100,1000000);
#endif
	CpuTimer0Regs.TCR.all=0x4001;
}



