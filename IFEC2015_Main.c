#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "IFEC2015_DCDC_Function.h"
#include "IFEC2015_DCDC_Parameter.h"
#include <math.h>
//DCDCMode说明                         DCDCSubMode说明
//-2 初始状态
//-1 软起动                                      0.1 BurstMode
// 0  升压闭环控制                     0.2 升压PI控制
// 1  降压闭环控制
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//主函数
void main(void)
{
	extern int16 DCDCMode,DCDCMode_1;
//主程序
	InitSysCtrl();
	DINT;
    InitPieCtrl();
    IER=0x0000;
    IFR=0x0000;
    InitPieVectTable();
    DELAY_US(50000L); //延时50ms，等待其他模块完成初始化
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0; //配置ePWM前关闭TBCLK
    EDIS;
//配置中断
    EALLOW;
	PieVectTable.EPWM3_INT=&epwm3_isr; //ePWM3中断
	PieVectTable.EPWM4_INT=&epwm4_isr; //ePWM3中断
	EDIS;
	IER|=M_INT3; //使能CPU的INT3中断，ePWM3,ePWM4使用INT3
	PieCtrlRegs.PIEIER3.bit.INTx3=1; //ePWM3
	PieCtrlRegs.PIEIER3.bit.INTx4=1; //ePWM4
//各种模块初始化
    ParameterInitial();
    AdcInitial(); //ADC初始化
    GPIOInitial(); //GPIO初始化
    EPwm3Initial(); //EPwm1初始化
    EPwm4Initial(); //EPwm1初始化
    InitEPwm3Gpio(); //EPwm3GPIO初始化
    InitEPwm4Gpio(); //EPwm4GPIO初始化
//初始化完成
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //重启TBCLK
    EDIS;
    EINT;
    ERTM;
//死循环
    while(1)
    {
//    	VoltageProtection(); //电压保护
    	if(DCDCMode_1!=DCDCMode)
    	{
    		switch(DCDCMode)
    		{
    		case -1:
    			EPwm3Regs.ETSEL.bit.INTEN=1;
    			EPwm3Regs.ETSEL.bit.SOCAEN=1;
    			EPwm4Regs.ETSEL.bit.INTEN=0;
    			EPwm4Regs.ETSEL.bit.SOCAEN=0;
    			EPwm3Regs.AQCSFRC.all=PWMS_FRC_DISABLE;
    			EPwm3Regs.DBCTL.all=0xb;
    			EPwm4Regs.AQCSFRC.all=PWMS_ALBL;
    			EPwm4Regs.DBCTL.all=0x0;
    			break;
    		case 1:
    		    EPwm3Regs.ETSEL.bit.INTEN=0;
    		    EPwm3Regs.ETSEL.bit.SOCAEN=0;
    		    EPwm4Regs.ETSEL.bit.INTEN=1;
    		    EPwm4Regs.ETSEL.bit.SOCAEN=1;
    			EPwm3Regs.AQCSFRC.all=PWMS_ALBL;
			    EPwm3Regs.DBCTL.all=0x0;
			    EPwm4Regs.AQCSFRC.all=PWMS_FRC_DISABLE;
			    EPwm4Regs.DBCTL.all=0xb;
    		}
    	}
    	DCDCMode_1=DCDCMode;
    }
}
