#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include <math.h>
#include "CLLC_Function.h"
#include "CLLC_Parameter.h"
//mode˵��
//0Ϊ�������۲���ֲ�����
//1Ϊ��ѹ����
//2Ϊ��ѹPI����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���������Ա���
float32 P,P_1,Reference;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������
void main(void)
{
//�����ⲿ����
	extern Uint16 Flag_PWMEnable,Flag_Start; //�������岨
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
	PieVectTable.EPWM2_INT=&epwm2_isr; //ePWM2�ж�
	EDIS;
	IER|=M_INT3; //ʹ��CPU��INT3�жϣ�ePWM1ʹ��INT3
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //ePWM1
	PieCtrlRegs.PIEIER3.bit.INTx2=1; //ePWM2
//����ģ���ʼ��
    InitParameter();
    AdcInit();//ADC��ʼ��
    InitEPwm1();//EPwm1��ʼ��
    InitEPwm2();//EPwm2��ʼ��
    InitEPwm3();//EPwm3��ʼ��
    InitEPwm1Gpio();//EPwm1 GPIO��ʼ��
    InitEPwm2Gpio();//EPwm2 GPIO��ʼ��
    InitEPwm3Gpio();//Epwm3 GPIO��ʼ��
//��ʼ�����
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //����TBCLK
    EDIS;
    EINT;
    ERTM;
//��ѭ��
    while(1)
    {
/*    	if(Flag_Start==0&&Flag_PWMEnable==1)//GpioDataRegs.GPCDAT.bit.GPIO69
    	{
    	    EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
    	    EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
    	}
    	else
    	{
    	    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    	    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    	}
*/   }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM1�жϣ�����
interrupt void epwm1_isr(void)
{
//ȫ�ֱ�������
//CLLC_Parameter.c����
	extern Uint16 mode;
	extern Uint16 AdcResults[3];
	extern float32 HighVol,HighCur;
	extern float32 Freq;
//��������̬����
	static Uint16 EPwm_TIMER_TBPRD;
	static float32 DutyCycle=0.05;
	static Uint16 i;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����
//��ȡADC���
	ReadAdc(AdcResults);
	AdcResCon();
	ModeSwitch();
//����
	switch(mode)
	{
	case 0:
		Freq=200000;
		break;
	case 1:

		if(i==0)
		{
			Freq=PIDHighVolLightLoad_Cal(HighVol,0.002); //���Ƶ��200K�����Ƶ��80K
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
		Freq=PIDHighVolNomal_Cal(HighVol,1/Freq); //���Ƶ��200K�����Ƶ��80K
		break;
	case 3:
		Freq=PIDTest_Cal(Reference,HighVol,1/Freq);
		break;
	}
//����PWMƵ��
	EPwm_TIMER_TBPRD=150*1000000/(2*Freq);
	EPwm1Regs.TBPRD=EPwm_TIMER_TBPRD;
	EPwm1Regs.CMPA.half.CMPA=EPwm_TIMER_TBPRD/2;
//����ж�
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM2�жϣ�����
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

	EPwm2Regs.CMPA.half.CMPA =(vg/(2*HighVol)+0.5)*7500; //��������Ϊ(e/100+0.5)*EPwm_TIMER_TBPRD�� Aͨ����1��4��Bͨ����2��3��˫���͵���
	EPwm3Regs.CMPA.half.CMPA=(-vg/(2*HighVol)+0.5)*7500; //������Ϊ(+-e/100+0.5)*EPwm_TIMER_TBPRD�� Aͨ����1��2��Bͨ����3��4�������Ե���

	EPwm2Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
