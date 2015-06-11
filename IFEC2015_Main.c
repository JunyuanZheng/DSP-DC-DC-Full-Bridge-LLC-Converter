#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "IFEC2015_DCDC_Function.h"
#include "IFEC2015_DCDC_Parameter.h"
#include <math.h>
//DCDCMode˵��                         DCDCSubMode˵��
//-2 ��ʼ״̬
//-1 ����                                      0.1 BurstMode
// 0  ��ѹ�ջ�����                     0.2 ��ѹPI����
// 1  ��ѹ�ջ�����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������
void main(void)
{
	extern int16 DCDCMode,DCDCMode_1;
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
	PieVectTable.EPWM3_INT=&epwm3_isr; //ePWM3�ж�
	PieVectTable.EPWM4_INT=&epwm4_isr; //ePWM3�ж�
	EDIS;
	IER|=M_INT3; //ʹ��CPU��INT3�жϣ�ePWM3,ePWM4ʹ��INT3
	PieCtrlRegs.PIEIER3.bit.INTx3=1; //ePWM3
	PieCtrlRegs.PIEIER3.bit.INTx4=1; //ePWM4
//����ģ���ʼ��
    ParameterInitial();
    AdcInitial(); //ADC��ʼ��
    GPIOInitial(); //GPIO��ʼ��
    EPwm3Initial(); //EPwm1��ʼ��
    EPwm4Initial(); //EPwm1��ʼ��
    InitEPwm3Gpio(); //EPwm3GPIO��ʼ��
    InitEPwm4Gpio(); //EPwm4GPIO��ʼ��
//��ʼ�����
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //����TBCLK
    EDIS;
    EINT;
    ERTM;
//��ѭ��
    while(1)
    {
//    	VoltageProtection(); //��ѹ����
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
