#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "CLLC_Function.h"
#include "CLLC_Parameter.h"
//mode说明
//0为开环，观测各种测量量
//1为升压轻载
//2为升压PI控制
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本函数测试变量
float32 frequency=150;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//主函数
void main(void)
{
//声明外部函数
	extern Uint16 Flag_PWMEnable; //控制脉冲波
	extern Uint16 mode;
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
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM1中断
	EDIS;
	IER|=M_INT3; //使能CPU的INT3中断，ePWM1使用INT3
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //ePWM1
//各种模块初始化
    InitParameter();
    AdcInit();//ADC初始化
    InitEPwm1();//EPwm1初始化
    InitEPwm2();//EPwm2初始化
    InitEPwm1Gpio();//EPwm1 GPIO初始化
    InitEPwm2Gpio();//EPwm2 GPIO初始化
//初始化完成
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //重启TBCLK
    EDIS;
    EINT;
    ERTM;
//死循环
    while(1)
    {
    	switch(mode)
    	{
    	case 0:
    		if(Flag_PWMEnable==1)//GpioDataRegs.GPCDAT.bit.GPIO69
    		{
    			EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
    		    EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
    		}
    		else
    		{
    		    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    		    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    		}
    		break;
    	default:
    		if(GpioDataRegs.GPCDAT.bit.GPIO69==1)//
    		{
    		    EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
    		    EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
    		}
    		else
    		{
    		    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    		    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    		}
    		break;
    	}
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM1中断，运算
interrupt void epwm1_isr(void)
{
//全局变量声明
//CLLC_Parameter.c定义
	extern Uint16 mode;
	extern Uint16 AdcResults[3];
	extern float32 HighVol,LowCur;
	extern float32 Freq;
//本函数静态变量
	static Uint16 EPwm_TIMER_TBPRD,mode_1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//运算
//重新初始化ADC
	if(mode_1!=mode)
		AdcReInit();
//读取ADC结果
	ReadAdc(AdcResults);
	AdcResCon();
//运算
	switch(mode)
	{
	case 0:
		Freq=frequency*1000;
		break;
	case 1:
		if((2.4*HighVol)>380)
		{
			EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
			EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
		}
		else
		{
    		EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
    		EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
		}
		break;
	case 2:
		Freq=PIDHighVol_Cal(HighVol,1/Freq); //设定值为380V，读书值*4.8倍，最高频率200K，最低频率90K
		break;
	}
//控制PWM频率
	EPwm_TIMER_TBPRD=150*1000000/(2*Freq);
	EPwm1Regs.TBPRD=EPwm_TIMER_TBPRD;
	EPwm1Regs.CMPA.half.CMPA=EPwm_TIMER_TBPRD/2;
	EPwm2Regs.TBPRD=EPwm_TIMER_TBPRD;
	EPwm2Regs.CMPA.half.CMPA=EPwm_TIMER_TBPRD/2;
	mode_1=mode;
//清除中断
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
