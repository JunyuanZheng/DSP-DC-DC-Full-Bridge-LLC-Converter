#include "DSP28x_Project.h"
#include "CLLC_Function.h"
#include "CLLC_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC�������
void AdcInit()//ADC��ʼ������
{
    EALLOW;
    #if (CPU_FRQ_150MHZ)   // Default - 150 MHz SYSCLKOUT
    #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
    #endif
    #if (CPU_FRQ_100MHZ)
    #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
    #endif
    EDIS;
    InitAdc();
    EALLOW;
    SysCtrlRegs.HISPCP.all=ADC_MODCLK;
    EDIS;
    AdcRegs.ADCTRL3.bit.ADCCLKPS=0x1;
    AdcRegs.ADCTRL1.bit.ACQ_PS=0xf;
    AdcRegs.ADCTRL1.bit.SEQ_CASC=1;
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1=1;//ʹ��ePWM_SOCA�����ź�
}
void AdcReInit(void)
{
//�����ⲿ����
	extern Uint16 mode;
//ADC���³�ʼ��
	switch(mode)
	{
	case 0:
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //������ֵΪ��ֵ+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x3; //��ѹ���ѹ��3ͨ��
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x2; //��ѹ�������2ͨ��
		break;
	case 1:
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //������ֵΪ��ֵ+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x3; //��ѹ���ѹ��3ͨ��
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x2; //��ѹ�������2ͨ��
		break;
	case 2:
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=0; //������ֵΪ��ֵ+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x3; //������2ͨ��
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC��ȡ���
void ReadAdc(unsigned int *p)//ADC��ȡ����
{
	extern Uint16 mode;

	while(AdcRegs.ADCST.bit.SEQ1_BSY==1);
	switch(mode)
	{
	case 0:
		*p=AdcRegs.ADCRESULT0>>4;
		*(p+1)=AdcRegs.ADCRESULT1>>4;
		break;
	case 1:
		*p=AdcRegs.ADCRESULT0>>4;
		*(p+1)=AdcRegs.ADCRESULT1>>4;
		break;
	case 2:
		*p=AdcRegs.ADCRESULT0>>4;
		break;
	}
	AdcRegs.ADCTRL2.bit.RST_SEQ1=1;
	AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;
}

