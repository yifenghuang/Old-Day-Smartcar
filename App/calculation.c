#include "common.h"
#include "include.h"
#include "calculation.h"

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：CCD_init
*  功能说明：CCD初始化
*  参数说明：
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*************************************************************************/
void CCD_init1(void)
{
  gpio_init (PTA9 ,GPO,1);
  gpio_init (PTA11 ,GPO,1);
  adc_init(ADC0_SE10) ; 
}
void CCD_init2(void)
{
  gpio_init (PTB1, GPO,1);
  gpio_init (PTA29, GPO,1);
  adc_init(ADC1_SE17) ;
}
void CCD_init3(void)
{
  gpio_init (PTB2, GPO,1);
  gpio_init (PTB3, GPO,1);
  adc_init(ADC1_SE8) ;    //B0
}
/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：StartIntegration
*  功能说明：CCD启动程序
*  参数说明：
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*************************************************************************/
void StartIntegration1(void) {

    unsigned char i;

    SI_SetVal();            /* SI  = 1 */
    SamplingDelay();
    CLK_SetVal();           /* CLK = 1 */
    SamplingDelay();
    SI_ClrVal();            /* SI  = 0 */
    SamplingDelay();
    CLK_ClrVal();           /* CLK = 0 */

    for(i=0; i<127; i++) {
        SamplingDelay();
        SamplingDelay();
        CLK_SetVal();       /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        CLK_ClrVal();       /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    CLK_SetVal();           /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    CLK_ClrVal();           /* CLK = 0 */
}

void StartIntegration2(void) {

    unsigned char i;

    SI_SetVa2();            /* SI  = 1 */
    SamplingDelay();
    CLK_SetVa2();           /* CLK = 1 */
    SamplingDelay();
    SI_ClrVa2();            /* SI  = 0 */
    SamplingDelay();
    CLK_ClrVa2();           /* CLK = 0 */

    for(i=0; i<127; i++) {
        SamplingDelay();
        SamplingDelay();
        CLK_SetVa2();       /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        CLK_ClrVa2();       /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    CLK_SetVa2();           /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    CLK_ClrVa2();           /* CLK = 0 */
}

void StartIntegration3(void) {

    unsigned char i;

    SI_SetVa3();            /* SI  = 1 */
    SamplingDelay();
    CLK_SetVa3();           /* CLK = 1 */
    SamplingDelay();
    SI_ClrVa3();            /* SI  = 0 */
    SamplingDelay();
    CLK_ClrVa3();           /* CLK = 0 */

    for(i=0; i<127; i++) {
        SamplingDelay();
        SamplingDelay();
        CLK_SetVa3();       /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        CLK_ClrVa3();       /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    CLK_SetVa3();           /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    CLK_ClrVa3();           /* CLK = 0 */
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：ImageCapture
*  功能说明：CCD采样程序
*  参数说明：* ImageData   采样数组
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*ImageData =  ad_once(ADC1, AD6a, ADC_8bit);
*************************************************************************/

void ImageCapture1(uint8 * ImageData) {

    unsigned int i;
    extern uint8 AtemP ;

    SI_SetVal();            /* SI  = 1 */
    SamplingDelay();
    CLK_SetVal();           /* CLK = 1 */
    SamplingDelay();
    SI_ClrVal();            /* SI  = 0 */
    SamplingDelay();

    //Delay 10us for sample the first pixel
    /**/
    for(i = 0; i < 300; i++) {                    //更改250，让CCD的图像看上去比较平滑，
      SamplingDelay() ;  //200ns                  //把该值改大或者改小达到自己满意的结果。
    }

    //Sampling Pixe1 1
    *ImageData =  adc_once (ADC0_SE10 , ADC_8bit);
    ImageData ++ ;
    CLK_ClrVal();           /* CLK = 0 */

    for(i=0; i<127; i++) {
        SamplingDelay();
        SamplingDelay();
        CLK_SetVal();       /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        //Sampling Pixe1 2~128

        *ImageData =  adc_once(ADC0_SE10, ADC_8bit);
        ImageData ++ ;
        CLK_ClrVal();       /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    CLK_SetVal();           /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    CLK_ClrVal();           /* CLK = 0 */
}

void ImageCapture2(uint8 * ImageData) {

    unsigned int i;
    extern uint8 AtemP ;

    SI_SetVa2();            /* SI  = 1 */
    SamplingDelay();
    CLK_SetVa2();           /* CLK = 1 */
    SamplingDelay();
    SI_ClrVa2();            /* SI  = 0 */
    SamplingDelay();

    //Delay 10us for sample the first pixel
    /**/
    for(i = 0; i < 300; i++) {                    //更改250，让CCD的图像看上去比较平滑，
      SamplingDelay() ;  //200ns                  //把该值改大或者改小达到自己满意的结果。
    }

    //Sampling Pixe1 1
    *ImageData =  adc_once (ADC1_SE17, ADC_8bit);
    ImageData ++ ;
    CLK_ClrVa2();           /* CLK = 0 */

    for(i=0; i<127; i++) {
        SamplingDelay();
        SamplingDelay();
        CLK_SetVa2();       /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        //Sampling Pixe1 2~128

        *ImageData =  adc_once (ADC1_SE17, ADC_8bit);
        ImageData ++ ;
        CLK_ClrVa2();       /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    CLK_SetVa2();           /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    CLK_ClrVa2();           /* CLK = 0 */
}

void ImageCapture3(uint8 * ImageData) {

    unsigned int i;
    extern uint8 AtemP ;

    SI_SetVa3();            /* SI  = 1 */
    SamplingDelay();
    CLK_SetVa3();           /* CLK = 1 */
    SamplingDelay();
    SI_ClrVa3();            /* SI  = 0 */
    SamplingDelay();

    //Delay 10us for sample the first pixel
    /**/
    for(i = 0; i < 300; i++) {                    //更改250，让CCD的图像看上去比较平滑，
      SamplingDelay() ;  //200ns                  //把该值改大或者改小达到自己满意的结果。
    }

    //Sampling Pixe1 1
    *ImageData =  adc_once (ADC1_SE8, ADC_8bit);
    ImageData ++ ;
    CLK_ClrVa3();           /* CLK = 0 */

    for(i=0; i<127; i++) {
        SamplingDelay();
        SamplingDelay();
        CLK_SetVa3();       /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        //Sampling Pixe1 2~128

        *ImageData =  adc_once (ADC1_SE8, ADC_8bit);
        ImageData ++ ;
        CLK_ClrVa3();       /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    CLK_SetVa3();           /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    CLK_ClrVa3();           /* CLK = 0 */
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：CalculateIntegrationTime
*  功能说明：计算曝光时间
*  参数说明：
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*************************************************************************/

/* 曝光时间，单位ms */
uint8 IntegrationTime = 10;
void CalculateIntegrationTime(void) {
extern uint8 Pixe1[128];
/* 128个像素点的平均AD值 */
uint8 PixelAverageValue;
/* 128个像素点的平均电压值的10倍 */
uint8 PixelAverageVoltage;
/* 设定目标平均电压值，实际电压的10倍 */
int16 TargetPixelAverageVoltage = 25;
/* 设定目标平均电压值与实际值的偏差，实际电压的10倍 */
int16 PixelAverageVoltageError = 0;
/* 设定目标平均电压值允许的偏差，实际电压的10倍 */
int16 TargetPixelAverageVoltageAllowError = 2;

    /* 计算128个像素点的平均AD值 */
    PixelAverageValue = PixelAverage(128,Pixe1);
    /* 计算128个像素点的平均电压值,实际值的10倍 */
    PixelAverageVoltage = (unsigned char)((int)PixelAverageValue * 25 / 194);

    PixelAverageVoltageError = TargetPixelAverageVoltage - PixelAverageVoltage;
    if(PixelAverageVoltageError < -TargetPixelAverageVoltageAllowError)
    {
      PixelAverageVoltageError = 0- PixelAverageVoltageError ;
      PixelAverageVoltageError /= 2;
      if(PixelAverageVoltageError > 5 )
         PixelAverageVoltageError = 5 ;     //每次变化曝光时间
      IntegrationTime -= PixelAverageVoltageError;
    }
    if(PixelAverageVoltageError > TargetPixelAverageVoltageAllowError)
    { 
      PixelAverageVoltageError /= 2;
      if(PixelAverageVoltageError > 5 )
         PixelAverageVoltageError = 5 ;     //每次变化曝光时间
      IntegrationTime += PixelAverageVoltageError;
    }

    if(IntegrationTime <= 1)
        IntegrationTime = 1;
    if(IntegrationTime >= 100)
        IntegrationTime = 100;
}


/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：PixelAverage
*  功能说明：求数组的均值程序
*  参数说明：
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*************************************************************************/
uint8 PixelAverage(uint8 len, uint8 *data) {
  unsigned char i;
  unsigned int sum = 0;
  for(i = 0; i<len; i++) {
    sum = sum + *data++;
  }
  return ((unsigned char)(sum/len));
}
/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：SendHex
*  功能说明：采集发数程序
*  参数说明：
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*************************************************************************/
void SendHex(unsigned char hex) {
  unsigned char temp;
  temp = hex >> 4;
  if(temp < 10) {
    uart_putchar(UART3,temp + '0');
  } else {
    uart_putchar(UART3,temp - 10 + 'A');
  }
  temp = hex & 0x0F;
  if(temp < 10) {
    uart_putchar(UART3,temp + '0');
  } else {
   uart_putchar(UART3,temp - 10 + 'A');
  }
}
/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：SendImageData
*  功能说明：
*  参数说明：
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*************************************************************************/
void SendImageData(unsigned char * ImageData) {

    unsigned char i;
    unsigned char crc = 0;

    /* Send Data */
    uart_putchar(UART3,'*');
    uart_putchar(UART3,'L');
    uart_putchar(UART3,'D');

    SendHex(0);
    SendHex(0);
    SendHex(0);
    SendHex(0);

    for(i=0; i<128; i++) {
      SendHex(*ImageData++);
    }

    SendHex(crc);
    uart_putchar(UART3,'#');
}
/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：SamplingDelay
*  功能说明：CCD延时程序 200ns
*  参数说明：
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*************************************************************************/
 void SamplingDelay(void){
  /* volatile uint8 i ;
   for(i=0;i<2;i++) {
    asm("nop");
    asm("nop");
    asm("nop");
   }*/
   systick_delay_ns(200);
   
   
   
}

