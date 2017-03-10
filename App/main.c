/*!
 * @file       main.c
 * @brief      逆袭二号_主函数
 * @author     黄翊峰
 * @version    v1.2
 * @date       2015-04-06
 */

#include "common.h"
#include "include.h"




//需要用上位机调整的参数们
//直立PD 50 0.27
float Zhili_P=50.0;
float Zhili_D=0.28;//10times

int test = 0;

//速度PID
float Sudu_P=0.0;
float Sudu_I=0.0;
float Sudu_D=0.0;

//方向PD
float Fangxiang_P_R=21.0;//left
float Fangxiang_D_R=0.1;
float Fangxiang_P_L=21.0;//right
float Fangxiang_D_L=0.1;

float Ang_Set=82;//角度便宜
float zhijiao_Power=0;


float Ang_Cha=0; //在直线速度冲刺起来时候和坡道上减速用的角度偏移量
float Ang_Podao=0;
int Expose_Time=5;//曝光时间

int qipaoyanshi=500;//起跑延时时间在灯塔出现之前

//预留变量
//uint32 data32=0;//用于扇区读取测试变量

//储存两个CCD的值，外部变量
unsigned char Pixe1[128]={1,1,1,1,1,1,1,1,1};
unsigned char Pixe2[128]={1,1,1,1,1,1,1,1,1};
extern int16 line_left[3];//左黑线位置存放数组，最近3次的
extern int16 line_right[3];//右黑线位置存放数组，最近3次的

//用于定时器扩展时间
int CCD_Time=0;//ccd采集时间间隔变量
int Speed_Time=0;//测一次速度的间隔时间变量
int DSpeed_Time=0;
int D2Speed_Time=0;
int Speed_Con=0;//速度控制时间

//储存角速度值
int GYRO_X=0;
int GYRO_Y=0;
int GYRO_Z=0;

//储存加速度值
double ACC_X=1.1;
double ACC_Y=1.1;
double ACC_Z=1.1;

//互补滤波的两个角度
float Ang_Acc=1.1;//加速度计atan所得角度
float Ang_IGyro=1.1;//角速度积分所得角度

float Ang_Error=1.1;//互补滤波的偏差

float Ang=1.1;//最终要用到PID的角度

float OutData[4];//4路上位机显示

//测速
int LeftWheel_Count=0;//左轮脉冲累积
int RightWheel_Count=0;//右轮脉冲累积
int Check_Speed=0;//编码器检测得到的速度（512线）
int D_Check_Speed=0;//速度微分
int D2_Check_Speed=0;
int I_Check_Speed=0;    //速度积分
int Speed_Set=0;        //速度设定值
int Speed_Error=0;      //速度误差值
int Check_Speed_Last=0; //用于计算速度D
int Check_Speed_Last2=0;//用于计算速度D2
extern int zhijiao_hei;
//转弯
int Turning=0;
int guanche=0;

int dengta_yanshi=0;
int dengta=0;
int dengta1=0;
int dengta_end_you=0;
int dengta_end_wu=0;
int dengta_num=0;
int dengta_num1=0;
int qipao=0;
int pao_end_qian=0;
int pao_end=0;
int heheda=0;

//速度控制变量新增：
float Speed_Final_Out=0;//最终的速度控制输出
float Speed_Last=0;
float SpeedAverOut=0;//速度平滑输出
float SpeedQueue[5]={0};
float Speed_Int=0;
float Speed_Rate=0;
float Speed_Down=0;

int Parameter2=0;
int Parameter1=0;
float Parameter3=0;
int Parameter4=0;

int dang=0;
int gear=0;
int yanshiqipao=0;
int yanshi = 0;
/*!
 *  @brief      main函数
 */   //新车
void main()
{
    unsigned int i=0,en=0;//loop
    uint8 send_data_cnt=0;//ccd上位机发送间隔时间变量
    
//如果是FX芯片则要开启硬件浮点单元，快5倍
#if  MK60F15 
    SCB->CPACR |=((3UL << 10*2)|(3UL << 11*2));     /* set CP10 and CP11 Full Access */
#endif
    
    //初始化OLED模块
    OLED_Init();    
    OLED_Refresh_Gram();//更新显示
    
    //初始化加速度计和陀螺仪
    Init_L3G4200D();    
    MMA845x_init();

    //初始化两个CCD
    //CCD_init1() ;   
    CCD_init2() ;   
    CCD_init3() ;   

    //初始化四路pwm去电机
    FTM_PWM_init(FTM0, FTM_CH0, 8000, 0);                  
    FTM_PWM_init(FTM0, FTM_CH1, 8000, 0);
    FTM_PWM_init(FTM0, FTM_CH2, 8000, 0);
    FTM_PWM_init(FTM0, FTM_CH3, 8000, 0);
    
    //GPIO们的初始化
    gpio_init  (PTC10, GPI, 0);//初始化 PTB10(第一路测速方向位)
    gpio_init  (PTC11, GPI, 0);//初始化 PTB11(第二路测速方向位)
    //gpio_init  (PTC5, GPI, 0);//初始化
    
    port_init(PTC5, ALT1 | IRQ_FALLING);          
    set_vector_handler(PORTC_VECTORn ,PORTC_IRQHandler);    
    enable_irq (PORTC_IRQn);                                
   
    gpio_init  (PTC6, GPI, 0);//初始化
    gpio_init  (PTC0 , GPO, 0);//初始化蜂鸣器
    gpio_init  (PTD12, GPI, 0);//初始化 两个光电管 临时使用按键接口
    gpio_init  (PTD13, GPI, 0);//初始化 两个光电管 临时使用按键接口                             
    
    gpio_init  (PTB4, GPI, 0);//初始化 薄码
    gpio_init  (PTB5, GPI, 0);//初始化 
    
    //初始化输入捕捉中断们
    FTM_Input_init(FTM1, FTM_CH0, FTM_Falling,FTM_PS_1);              //初始化FTM输入捕捉模式（用于第一路测速）
    port_init_NoALT(FTM1_CH0 ,PULLUP);

    set_vector_handler(FTM1_VECTORn ,FTM1_INPUT_IRQHandler);//设置FTM1的中断服务函数为 FTM1_INPUT_IRQHandler
    enable_irq (FTM1_IRQn);

    FTM_Input_init(FTM2, FTM_CH0, FTM_Falling,FTM_PS_1);              //初始化FTM输入捕捉模式（用于第二路测速）
    port_init_NoALT(FTM2_CH0 ,PULLUP);

    set_vector_handler(FTM2_VECTORn ,FTM2_INPUT_IRQHandler);//设置FTM2的中断服务函数为 FTM1_INPUT_IRQHandler
    enable_irq (FTM2_IRQn);  
    
    //初始化6个按键
    key_init(KEY_U);
    key_init(KEY_D);
    key_init(KEY_L);
//    key_init(KEY_R);
//    key_init(KEY_A);
//    key_init(KEY_B);
    
    //设定定时器0 1ms中断
    pit_init_ms(PIT0, 1);                                //初始化PIT0，定时时间为： 1ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
    enable_irq (PIT0_IRQn);
    
    //zheng
    /*
    //初始化flash
    flash_init();
    //擦除最后一个扇区，容量4k
    flash_erase_sector(FLASH_SECTOR_NUM-1);
    //写入数据到扇区，偏移地址为0，必须一次写入4字节
    if( 1 == flash_write(FLASH_SECTOR_NUM-1, 0, 123456) )   
    {
        data32 =    flash_read(FLASH_SECTOR_NUM-1, 0, uint32);  //读取4字节
    }
    */

    while(1)
    {      
      //gpio_set   (PTC0,   1); 
        en++;
        yanshi++;
        if(dengta_yanshi<=10 && dengta1==1)
        {
          dengta1=0;
          dengta_num1++;
          if(dengta<=3) dengta_num++;
          dengta=0;
          if(dengta_num1>=3 )
          {
            if(dengta_num>=3 ) qipao=1;
            else qipao=0;
            dengta_num1=0;
            dengta_num=0;            
          }
        }
        
        if(dengta_yanshi>=5000 && dengta1==1)
        {
          dengta_yanshi=5100;
          dengta1=0;
          dengta_num1++;
          if(dengta<=3) dengta_num++;
          dengta=0;
          if(dengta_num1>=3 )
          {
            if(dengta_num>=2 ) 
            {
              dengta_end_wu=1;
              //dengta_end_you=0;
            }else 
            {
              dengta_end_you++;
              dengta_end_wu=0;
            }
            dengta_num1=0;
            dengta_num=0;            
          }
        }
        if(heheda==0 && dengta_end_you>=2 && dengta_end_wu==1) pao_end=1;
        
       // gpio_set   (PTC0,   1); 
        if(test == 1)
        {
            if(yanshi > 300)//替换此处条件为起跑检测即可
            {
              qipaoyanshi++;
              
              if(qipaoyanshi>=3000) qipaoyanshi=3100;
              
               if(Speed_Set<=40)
               {
                 Speed_Set+=20;
               }
               
               if(Speed_Set<=47 && Speed_Set>40)
               {
                 Speed_Set+=1;
               }
               
               if(Ang_Set>37)
               {
                 Ang_Set=37;
               }
               
               if(dang == 1)
               {
                   if(Sudu_P<30.0)
                   {
                      Sudu_P+=10.0;
             
                      Sudu_I=1.3;
                   }
               }
               else
               {
                    if(Sudu_P<30.0)
                   {
                      Sudu_P+=10.0;
             
                      Sudu_I=1.3;
                   }
               }

            }
            else
            {
              Ang_Cha=0;
            }
        }
        else
        {
            if(qipao==1 && en>100)//替换此处条件为起跑检测即可
            {
                            qipaoyanshi++;
              
              if(qipaoyanshi>=3000) qipaoyanshi=3100;
              
               if(Speed_Set<=40)
               {
                 Speed_Set+=20;
               }
               
               if(Speed_Set<=47 && Speed_Set>40)
               {
                 Speed_Set+=1;
               }
               
               if(Ang_Set>37)
               {
                 Ang_Set=37;
               }
               
               if(dang == 1)
               {
                   if(Sudu_P<30.0)
                   {
                      Sudu_P+=10.0;
             
                      Sudu_I=1.3;
                   }
               }
               else
               {
                    if(Sudu_P<30.0)
                   {
                      Sudu_P+=10.0;
             
                      Sudu_I=1.3;
                   }
               }

            }
            else
            {
              Ang_Cha=0;
            }
        }

        
                  //键盘调整一些必要的参数
          //        if(key_check(KEY_U) == KEY_DOWN) Ang_Set+=1;
          //        else if(key_check(KEY_D) == KEY_DOWN)Ang_Set-=1;
          //        
          //        if(key_check(KEY_L) == KEY_DOWN) Parameter2++;
          //        else if(key_check(KEY_R) == KEY_DOWN)Parameter2--;
          //        
          //        if(key_check(KEY_A) == KEY_DOWN)Parameter3++;
          //        else if(key_check(KEY_B) == KEY_DOWN)Parameter3--;                                    
          //  
        OLED_Clear();//清除RAM
        if(gpio_get(PTB4))
        {      
            if(key_check(KEY_D) == KEY_DOWN) Parameter2++;
            else if(key_check(KEY_U) == KEY_DOWN)Parameter4++;         
        }
        else
        {
          OLED_ShowString(64,0,"Change");
          if(key_check(KEY_D) == KEY_DOWN) Parameter3+=1;
          else if(key_check(KEY_U) == KEY_DOWN)Parameter3-=1;         
        }
        
        if(key_check(KEY_L) == KEY_DOWN) Parameter1++;
        
        if(Parameter1 >= 15)
        {
          Parameter1 = 0;
        }
        
        if(Parameter4>=15)
        {
          Parameter4=0;
        }
         
         if(Parameter2>=0 && Parameter2<5)
         {
           OLED_ShowString(4,0,"hh");
           gear=1;
         }
         else if(Parameter2>=5 && Parameter2<10)
         {
           OLED_ShowString(4,0,"Standard");
           gear=1;
         }
         else if(Parameter2>=10 && Parameter2<15)
         {
           OLED_ShowString(4,0,"Turtle");
           gear=2;
         }

         
         if(Parameter2 >= 15)
         {
           Parameter2=0;
         }
         if(Parameter2 <0)
         {
           Parameter2=15;
         }

         if(gpio_get(PTB5))
         {
           OLED_ShowString(4,42,"final");
           dang=1;
           test=0;
         }
         else
         {
           OLED_ShowString(4,42,"test");
           dang=1;
           test=1;
         }
          //OLED显示3个参数
         
         if(qipao == 1)
         {
           OLED_ShowString(4,14,"Set Off!!!");
         }
         else
         {
           OLED_ShowString(4,14,"Waiting...");
         }
         OLED_ShowString(4,28,"Angel:");
          //LCD_Display(3,0,GYRO_Y);
          //LCD_Display(63,28,gpio_get(PTD11));
          //LCD_Display(33,14,gpio_get(PTC5));
          LCD_Display(53,28,zhangaijifen);//Ang_IGyro
          LCD_Display(93,28,Parameter3);
          //LCD_Display(3,42,zhijiao);

          
          //serial scoper上位机数据输出
          
           
          //CCD上位机数据输出
          send_data_cnt++;
          if(send_data_cnt >= 1) 
          {
            //SendImageData(Pixe2);  //发送数据到上位机
            send_data_cnt = 0;
          }
          if(send_data_cnt >= 4) 
          {
            //SendImageData(Pixe2);  //发送数据到上位机
            send_data_cnt = 0;
          }
          
          //显示横ccd
          if(Parameter1>=5 && Parameter1<10)
          {
              for(i=0; i<128; i++)
              {
                OLED_DrawPoint(i,64-Pixe3[i]/4,1);
              }

              for(i=0; i<128; i++)
              {
                OLED_DrawPoint(i,25,1);
              }
              
              for(i=0; i<128; i++)
              {
                OLED_DrawPoint(i,37,1);
              }
          }
          else if(Parameter1>=10 && Parameter1<15)
          {
              LCD_Display(53,42,gpio_get(PTD12));
              
          }
          
          if(Parameter4 > 5 && Parameter4<10)
          {
            OLED_ShowString(53,42,"Shut_All");
            heheda=1;
            yanshiqipao=0;
          }
          else if(Parameter4>10)
          {
            heheda=0;            
            yanshiqipao=0;
          }
          else
          { 
            yanshiqipao=1;
          }
          
    //        //显示竖ccd
    //        for(i=0; i<64; i++)
    //        {
    //          OLED_DrawPoint(Pixe2[i]/2,i,1);
    //        }

          OLED_Refresh_Gram();//更新显示
          OutPut_Data();
    }
}



