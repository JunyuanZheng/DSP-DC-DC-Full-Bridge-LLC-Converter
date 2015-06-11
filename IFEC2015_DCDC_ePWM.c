#include "DSP28x_Project.h"
#include "IFEC2015_DCDC_Function.h"
#include "IFEC2015_DCDC_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EPwm3,EPwm4，EPwm5 初始化，其中EPwm1A触发中断
void EPwm3Initial(void)
{
    EPwm3Regs.TBPRD=375;
    EPwm3Regs.TBPHS.half.TBPHS=0x0000;
    EPwm3Regs.TBCTR=0x0000;
    EPwm3Regs.CMPA.half.CMPA= EPwm3Regs.TBPRD/2;
    EPwm3Regs.CMPB=0;
    EPwm3Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm3Regs.TBCTL.bit.PHSEN=TB_DISABLE;
    EPwm3Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm3Regs.TBCTL.bit.CLKDIV=TB_DIV1;
    EPwm3Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;
    EPwm3Regs.AQCTLA.bit.CAU=AQ_CLEAR;
    EPwm3Regs.AQCTLA.bit.CAD=AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAU=AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAD=AQ_CLEAR;

    EPwm3Regs.ETSEL.bit.INTSEL=ET_CTR_PRD;
    EPwm3Regs.ETSEL.bit.INTEN=0;
    EPwm3Regs.ETPS.bit.INTPRD=ET_1ST;
    EPwm3Regs.ETCLR.bit.INT=1;

    EPwm3Regs.ETSEL.bit.SOCAEN=0;//设置ePWM_SOCA为ADC模块的启动信号，使能A组的启动信号SOCA
    EPwm3Regs.ETSEL.bit.SOCASEL=ET_CTR_ZERO;//选择SOCA信号产生的时刻为CTR=PRD
    EPwm3Regs.ETPS.bit.SOCAPRD=ET_1ST;//每次触发事件产生一个启动信号
    EPwm3Regs.ETCLR.bit.SOCA=1;//清除SOCA标志位

    EPwm3Regs.DBCTL.all=0xb;
    EPwm3Regs.DBRED=75;
    EPwm3Regs.DBFED=75;

	EPwm3Regs.AQCSFRC.all=PWMS_ALBL;
	EPwm3Regs.DBCTL.all=0x0;
}

void EPwm4Initial(void) //待定
{
    EPwm4Regs.TBPRD=375;
    EPwm4Regs.TBPHS.half.TBPHS=0x0000;
    EPwm4Regs.TBCTR=0x0000;
    EPwm4Regs.CMPA.half.CMPA= EPwm4Regs.TBPRD/2;
    EPwm4Regs.CMPB=0;
    EPwm4Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm4Regs.TBCTL.bit.PHSEN=TB_DISABLE;
    EPwm4Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm4Regs.TBCTL.bit.CLKDIV=TB_DIV1;
    EPwm4Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;
    EPwm4Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;
    EPwm4Regs.AQCTLA.bit.CAU=AQ_CLEAR;
    EPwm4Regs.AQCTLA.bit.CAD=AQ_SET;
    EPwm4Regs.AQCTLB.bit.CAU=AQ_SET;
    EPwm4Regs.AQCTLB.bit.CAD=AQ_CLEAR;

    EPwm4Regs.ETSEL.bit.INTSEL=ET_CTR_PRD;
   	EPwm4Regs.ETSEL.bit.INTEN=0;
   	EPwm4Regs.ETPS.bit.INTPRD=ET_1ST;
   	EPwm4Regs.ETCLR.bit.INT=1;

    EPwm3Regs.ETSEL.bit.SOCAEN=0;
    EPwm3Regs.ETSEL.bit.SOCASEL=ET_CTR_ZERO;
    EPwm3Regs.ETPS.bit.SOCAPRD=ET_1ST;
    EPwm3Regs.ETCLR.bit.SOCA=1;

    EPwm4Regs.DBCTL.all=0xb;
    EPwm4Regs.DBRED=75;
    EPwm4Regs.DBFED=75;

	EPwm4Regs.AQCSFRC.all=PWMS_ALBL;
	EPwm4Regs.DBCTL.all=0x0;
}

