#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
#include  "MK60_wdog.h"
#include  "MK60_it.H"
#include  "MK60_gpio.H"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
#include  "MK60_pit.h"      //PIT
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_adc.h"
#include  "MK60_FTM.h"

#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY

#include "IIC.h"            //L3G4200D和MMA845X
#include "calculation.h"    //CCD的驱动
#include "upper monitor.h"  //波形显示上位机的驱动
#include "math.h"           //高等数学.h
#include "oled.h"           //OLED驱动
#include "ccdwork.h"        //CCD取线、计算等



#endif  //__INCLUDE_H__
