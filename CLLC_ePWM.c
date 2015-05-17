#include "DSP28x_Project.h"
#include "CLLC_Function.h"
#include "CLLC_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EPwm1,EPwm2初始化，其中EPwm1A触发中断
void InitEPwm1(void)
{
	extern float32 Freq;
    EPwm1Regs.TBPRD=1/(2*Freq/150000);
    EPwm1Regs.TBPHS.half.TBPHS=0x0000;
    EPwm1Regs.TBCTR=0x0000;
    EPwm1Regs.CMPA.half.CMPA= EPwm1Regs.TBPRD/2;
    EPwm1Regs.CMPB=0;
    EPwm1Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm1Regs.TBCTL.bit.PHSEN=TB_DISABLE;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm1Regs.TBCTL.bit.CLKDIV=TB_DIV1;
    EPwm1Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;
    EPwm1Regs.AQCTLA.bit.CAU=AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.CAD=AQ_SET;
    EPwm1Regs.AQCTLB.bit.CAU=AQ_SET;
    EPwm1Regs.AQCTLB.bit.CAD=AQ_CLEAR;

    EPwm1Regs.ETSEL.bit.INTSEL=ET_CTR_PRD;
    EPwm1Regs.ETSEL.bit.INTEN=1;
    EPwm1Regs.ETPS.bit.INTPRD=ET_1ST;
    EPwm1Regs.ETCLR.bit.INT=1;

    EPwm1Regs.ETSEL.bit.SOCAEN=1;//设置ePWM_SOCA为ADC模块的启动信号，使能A组的启动信号SOCA
    EPwm1Regs.ETSEL.bit.SOCASEL=ET_CTR_ZERO;//选择SOCA信号产生的时刻为CTR=PRD
    EPwm1Regs.ETPS.bit.SOCAPRD=ET_1ST;//每次触发事件产生一个启动信号
    EPwm1Regs.ETCLR.bit.SOCA=1;//清除SOCA标志位

    EPwm1Regs.DBCTL.all=0xb;
    EPwm1Regs.DBRED=15;
    EPwm1Regs.DBFED=15;
}
void InitEPwm2(void)
{
	extern float32 Freq;
    EPwm2Regs.TBPRD=7500;
    EPwm2Regs.TBPHS.half.TBPHS=0x0000;
    EPwm2Regs.TBCTR=0x0000;
    EPwm2Regs.CMPA.half.CMPA=0;
    EPwm2Regs.CMPB=0;
    EPwm2Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm2Regs.TBCTL.bit.PHSEN=TB_DISABLE;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV=TB_DIV1;
    EPwm2Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;

    EPwm2Regs.AQCTLA.bit.CAU=AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD=AQ_SET;
    EPwm2Regs.AQCTLB.bit.CAU=AQ_SET;
    EPwm2Regs.AQCTLB.bit.CAD=AQ_CLEAR;

    EPwm2Regs.ETSEL.bit.INTSEL=ET_CTR_PRD;
    EPwm2Regs.ETSEL.bit.INTEN=1;
    EPwm2Regs.ETPS.bit.INTPRD=ET_1ST;
    EPwm2Regs.ETCLR.bit.INT=1;

    EPwm2Regs.DBCTL.all=0xb;
    EPwm2Regs.DBRED=50;
    EPwm2Regs.DBFED=50;
}

void InitEPwm3(void)
{
	extern float32 Freq;
    EPwm3Regs.TBPRD=7500;
    EPwm3Regs.TBPHS.half.TBPHS=0x0000;
    EPwm3Regs.TBCTR=0x0000;
    EPwm3Regs.CMPA.half.CMPA=0;
    EPwm3Regs.CMPB=0;
    EPwm3Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm3Regs.TBCTL.bit.PHSEN=TB_DISABLE;
    EPwm3Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm3Regs.TBCTL.bit.CLKDIV=TB_DIV1;
    EPwm3Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;

    EPwm3Regs.AQCTLA.bit.CAU=AQ_CLEAR; //待测试
    EPwm3Regs.AQCTLA.bit.CAD=AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAU=AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAD=AQ_CLEAR;

    EPwm3Regs.DBCTL.all=0xb;
    EPwm3Regs.DBRED=50;
    EPwm3Regs.DBFED=50;
}
