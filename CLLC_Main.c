#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "CLLC_Function.h"
#include "CLLC_Parameter.h"
//mode˵��
//0Ϊ�������۲���ֲ�����
//1Ϊ��ѹ����
//2Ϊ��ѹPI����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���������Ա���
float32 frequency=150;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������
void main(void)
{
//�����ⲿ����
	extern Uint16 Flag_PWMEnable; //�������岨
	extern Uint16 mode;
//������
	InitSysCtrl();
	DINT;
    InitPieCtrl();
    IER=0x0000;
    IFR=0x0000;
    InitPieVectTable();
    DELAY_US(50000L); //��ʱ50ms���ȴ�����ģ����ɳ�ʼ��

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0; //����ePWMǰ�ر�TBCLK
    EDIS;
//�����ж�
    EALLOW;
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM1�ж�
	EDIS;
	IER|=M_INT3; //ʹ��CPU��INT3�жϣ�ePWM1ʹ��INT3
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //ePWM1
//����ģ���ʼ��
    InitParameter();
    AdcInit();//ADC��ʼ��
    InitEPwm1();//EPwm1��ʼ��
    InitEPwm2();//EPwm2��ʼ��
    InitEPwm1Gpio();//EPwm1 GPIO��ʼ��
    InitEPwm2Gpio();//EPwm2 GPIO��ʼ��
//��ʼ�����
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //����TBCLK
    EDIS;
    EINT;
    ERTM;
//��ѭ��
    while(1)
    {
    	switch(mode)
    	{
    	case 0:
    		if(Flag_PWMEnable==1)//GpioDataRegs.GPCDAT.bit.GPIO69
    		{
    			EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
    		    EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
    		}
    		else
    		{
    		    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    		    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    		}
    		break;
    	default:
    		if(GpioDataRegs.GPCDAT.bit.GPIO69==1)//
    		{
    		    EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
    		    EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
    		}
    		else
    		{
    		    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    		    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    		}
    		break;
    	}
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM1�жϣ�����
interrupt void epwm1_isr(void)
{
//ȫ�ֱ�������
//CLLC_Parameter.c����
	extern Uint16 mode;
	extern Uint16 AdcResults[3];
	extern float32 HighVol,LowCur;
	extern float32 Freq;
//��������̬����
	static Uint16 EPwm_TIMER_TBPRD,mode_1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����
//���³�ʼ��ADC
	if(mode_1!=mode)
		AdcReInit();
//��ȡADC���
	ReadAdc(AdcResults);
	AdcResCon();
//����
	switch(mode)
	{
	case 0:
		Freq=frequency*1000;
		break;
	case 1:
		if((2.4*HighVol)>380)
		{
			EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
			EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
		}
		else
		{
    		EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
    		EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
		}
		break;
	case 2:
		Freq=PIDHighVol_Cal(HighVol,1/Freq); //�趨ֵΪ380V������ֵ*4.8�������Ƶ��200K�����Ƶ��90K
		break;
	}
//����PWMƵ��
	EPwm_TIMER_TBPRD=150*1000000/(2*Freq);
	EPwm1Regs.TBPRD=EPwm_TIMER_TBPRD;
	EPwm1Regs.CMPA.half.CMPA=EPwm_TIMER_TBPRD/2;
	EPwm2Regs.TBPRD=EPwm_TIMER_TBPRD;
	EPwm2Regs.CMPA.half.CMPA=EPwm_TIMER_TBPRD/2;
	mode_1=mode;
//����ж�
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
