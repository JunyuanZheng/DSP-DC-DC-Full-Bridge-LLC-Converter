#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include <math.h>
#include "CLLC_Function.h"
#include "CLLC_Parameter.h"
//mode说明
//0为开环，观测各种测量量
//1为升压轻载
//2为升压PI控制
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本函数测试变量
float32 P,P_1,Reference;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//主函数
void main(void)
{
//声明外部函数
	extern Uint16 Flag_PWMEnable,Flag_Start; //控制脉冲波
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
	PieVectTable.EPWM2_INT=&epwm2_isr; //ePWM2中断
	EDIS;
	IER|=M_INT3; //使能CPU的INT3中断，ePWM1使用INT3
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //ePWM1
	PieCtrlRegs.PIEIER3.bit.INTx2=1; //ePWM2
//各种模块初始化
    InitParameter();
    AdcInit();//ADC初始化
    InitEPwm1();//EPwm1初始化
    InitEPwm2();//EPwm2初始化
    InitEPwm3();//EPwm3初始化
    InitEPwm1Gpio();//EPwm1 GPIO初始化
    InitEPwm2Gpio();//EPwm2 GPIO初始化
    InitEPwm3Gpio();//Epwm3 GPIO初始化
//初始化完成
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //重启TBCLK
    EDIS;
    EINT;
    ERTM;
//死循环
    while(1)
    {
/*    	if(Flag_Start==0&&Flag_PWMEnable==1)//GpioDataRegs.GPCDAT.bit.GPIO69
    	{
    	    EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
    	    EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
    	}
    	else
    	{
    	    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    	    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    	}
*/   }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM1中断，运算
interrupt void epwm1_isr(void)
{
//全局变量声明
//CLLC_Parameter.c定义
	extern Uint16 mode;
	extern Uint16 AdcResults[3];
	extern float32 HighVol,HighCur;
	extern float32 Freq;
//本函数静态变量
	static Uint16 EPwm_TIMER_TBPRD;
	static float32 DutyCycle=0.05;
	static Uint16 i;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//运算
//读取ADC结果
	ReadAdc(AdcResults);
	AdcResCon();
	ModeSwitch();
//运算
	switch(mode)
	{
	case 0:
		Freq=200000;
		break;
	case 1:

		if(i==0)
		{
			Freq=PIDHighVolLightLoad_Cal(HighVol,0.002); //最高频率200K，最低频率80K
			i=Freq/500;
		}
		if(i<((1-DutyCycle)*(Freq/500)))
		{
			EPwm1Regs.AQCSFRC.all=PWMS_ALBL;
			EPwm1Regs.DBCTL.all=0x0;
		}
		else
		{
			EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE;
			EPwm1Regs.DBCTL.all=0xb;
		}
		i--;
		break;
	case 2:
		Freq=PIDHighVolNomal_Cal(HighVol,1/Freq); //最高频率200K，最低频率80K
		break;
	case 3:
		Freq=PIDTest_Cal(Reference,HighVol,1/Freq);
		break;
	}
//控制PWM频率
	EPwm_TIMER_TBPRD=150*1000000/(2*Freq);
	EPwm1Regs.TBPRD=EPwm_TIMER_TBPRD;
	EPwm1Regs.CMPA.half.CMPA=EPwm_TIMER_TBPRD/2;
//清除中断
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM2中断，运算
interrupt void epwm2_isr(void)
{
	static Uint16 t=1;
	static float32 vg,ig,dp;
	extern float32 HighVol;

	vg=220*sin(2*3.14159*t/200);
	ig=vg/100;
	P=vg*ig;

	dp=(P-P_1)/(0.0001);
	Reference=dp/(680*0.000001*380*(2*3.14159*100)*(2*3.14159*100));
	if(t<200)
		t++;
	else
		t=1;
	P_1=P;

	EPwm2Regs.CMPA.half.CMPA =(vg/(2*HighVol)+0.5)*7500; //如两个都为(e/100+0.5)*EPwm_TIMER_TBPRD则 A通道给1，4，B通道给2，3，双极型调制
	EPwm3Regs.CMPA.half.CMPA=(-vg/(2*HighVol)+0.5)*7500; //如两个为(+-e/100+0.5)*EPwm_TIMER_TBPRD则 A通道给1，2，B通道给3，4，但极性调制

	EPwm2Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
