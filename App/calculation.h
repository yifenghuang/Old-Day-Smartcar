
#ifndef  calculation_H
#define  calculation_H  

extern unsigned char Pixe1[128];
extern unsigned char Pixe2[128];
extern unsigned char Pixe3[128];
//#define TRUE        1
#define SI_SetVal()   PTA11_OUT = 1
#define SI_ClrVal()   PTA11_OUT = 0
#define CLK_ClrVal()  PTA9_OUT = 0
#define CLK_SetVal()  PTA9_OUT = 1

#define SI_SetVa2()   PTB1_OUT = 1
#define SI_ClrVa2()   PTB1_OUT = 0
#define CLK_ClrVa2()  PTA29_OUT = 0
#define CLK_SetVa2()  PTA29_OUT = 1

#define SI_SetVa3()   PTB2_OUT = 1
#define SI_ClrVa3()   PTB2_OUT = 0
#define CLK_ClrVa3()  PTB3_OUT = 0
#define CLK_SetVa3()  PTB3_OUT = 1

extern void CCD_init1(void) ;
extern void CCD_init2(void) ;
extern void CCD_init3(void) ;
extern void StartIntegration1(void) ; 
extern void StartIntegration2(void) ;
extern void StartIntegration3(void) ;
extern void ImageCapture1(uint8 * ImageData) ;
extern void ImageCapture2(uint8 * ImageData) ;
extern void ImageCapture3(uint8 * ImageData) ;
extern void SendHex(unsigned char hex) ;
extern void SamplingDelay(void) ;
extern void CalculateIntegrationTime(void) ;
extern uint8 PixelAverage(uint8 len, uint8 *data) ;
extern void SendImageData(unsigned char * ImageData) ;

#endif
