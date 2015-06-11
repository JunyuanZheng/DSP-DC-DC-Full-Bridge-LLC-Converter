#include "DSP28x_Project.h"
#include "IFEC2015_DCDC_Function.h"
#include "IFEC2015_DCDC_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC�������
void AdcInitial()//ADC��ʼ������
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

    AdcRegs.ADCMAXCONV.bit.MAX_CONV1=2; //������ֵΪ��ֵ+1
    AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x3; //ֱ��ĸ�ߵ�ѹ
    AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x2; //��ص�ѹ
    AdcRegs.ADCCHSELSEQ1.bit.CONV02=0x1; //��ص���

    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1=1;//ʹ��ePWM_SOCA�����ź�
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC��ȡ���
void ReadAdc(unsigned int *p)//ADC��ȡ����
{
	while(AdcRegs.ADCST.bit.SEQ1_BSY==1);
	*p=AdcRegs.ADCRESULT0>>4; //ֱ��ĸ�ߵ�ѹ
	*(p+1)=AdcRegs.ADCRESULT1>>4; //��ص�ѹ
	*(p+2)=AdcRegs.ADCRESULT2>>4; //��ص���
	AdcRegs.ADCTRL2.bit.RST_SEQ1=1;
	AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;
}
