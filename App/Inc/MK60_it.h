/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.h
 * @brief      山外K60 平台中断复位重定向头文件
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */


#ifndef __MK60_IT_H__
#define __MK60_IT_H__

extern unsigned char Pixe1[128];
extern unsigned char Pixe2[128];
extern long int ccd_avg;
extern int zhijiao;
extern int po;
extern int Into_zhijiao_right;
extern int Into_zhijiao_left;
extern int guanbizhijiao;
extern int zhijiaoqian;
extern int Parameter1;
extern int Parameter2;
extern int zhangaijifen;
extern float Parameter3;
extern int SUM;
extern float zhijiao_Power;
extern int Speed_Con;
extern int zhangail;
extern int zhangair;
extern int d;
extern int gear;
extern float Ang_Cha;
extern int DSpeed_Time;
extern int D2Speed_Time;
extern int Coner;
extern int Speed_Change;
extern int qipao;
extern int Stright_Count;
extern float Turning_Out_L;
extern float Turning_Out_R;
extern float Turning_Out_L_Lo;
extern float Turning_Out_R_Lo;
extern float Dynamic_P_L;
extern int D2_Check_Speed;
extern float Dynamic_P_R;
extern float qianzhan;
//需要用上位机调整的参数们
extern float Zhili_P;
extern float Zhili_D;
extern int dang;
extern int Parameter4;
extern float Sudu_P;
extern float Sudu_I;
extern float Sudu_D;
extern float Fangxiang_P_R;
extern float Fangxiang_D_R;
extern float Fangxiang_P_L;
extern float Fangxiang_D_L;
extern int Expose_Time;//曝光时间
extern int qipaoyanshi;
extern float Speed_Int;
extern float Speed_Rate;
extern float Speed_Down;
extern int danxian_check_sum;
extern int danxian;
extern int Min_Hor[2][4];
extern float SpeedQueue[5];
extern int CCD_Time;//用于定时器扩展时间
extern int Speed_Time;//用于测速的时间
extern int zhijiao_chu_shu;
//储存角速度值
extern int GYRO_X;
extern int GYRO_Y;
extern int GYRO_Z;

//储存加速度值
extern double ACC_X;
extern double ACC_Y;
extern double ACC_Z;

//互补滤波的两个角度
extern float Ang_Acc;//加速度计atan所得角度
extern float Ang_IGyro;//角速度积分所得角度

extern float Ang_Error;//互补滤波的偏差

extern float Ang;//最终要用到PID的角度
extern float Ang_Set;
extern int podao;
extern float Ang_Podao;
extern int LeftWheel_Count;
extern int RightWheel_Count;           
extern int Check_Speed;                  //编码器检测得到的速度（512线）
extern int D_Check_Speed;             //左轮速度测试值微分
extern int I_Check_Speed;             //左轮速度测试值积分
extern int Speed_Set;                      //速度设定值
extern int Check_Speed_Last;
extern int dengta;
extern int dengta1;
extern int dengta_yanshi;
extern int pao_end;
extern int Corner_Count;
extern int Turning;
extern int Check_Speed_Last2;//用于计算速度D
extern int danxian;
extern int guanche;
extern int duiguan_time;
extern int duiguan_guanbi;
extern int Shut_Hill;
extern int zhijiao_line_left;
extern int zhijiao_line_right;
extern float Speed_Control_Out;//速度控制输出量，直接给定到电机上
extern int Speed_Error;
extern float Speed_Final_Out;//最终的速度控制输出
extern float Speed_Last;
extern float SpeedAverOut;//速度平滑输出
/*                          重新定义中断向量表
 *  先取消默认的中断向量元素宏定义        #undef  VECTOR_xxx
 *  在重新定义到自己编写的中断函数      #define VECTOR_xxx    xxx_IRQHandler
 *  例如：
 *       #undef  VECTOR_003                         先取消映射到中断向量表里的中断函数地址宏定义
 *       #define VECTOR_003    HardFault_Handler    重新定义硬件上访中断服务函数
 */
   
extern void PIT0_IRQHandler(void);
void FTM1_INPUT_IRQHandler(void);
void FTM2_INPUT_IRQHandler(void);
void Go(float Speed); //更新电机输出，包括转弯值和速度值
void PORTC_IRQHandler(void);




#endif  //__MK60_IT_H__
