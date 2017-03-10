/*!
 * @file       MK60_it.c
 * @brief      逆袭二号_中断服务函数
 * @author     黄翊峰 汪星
 * @version    v1.2
 * @date       2015-04-06
 */

#include    "MK60_it.h"
#include    "include.h"
//#define Canshu //如果定义，会关闭速度控制和弯道控制


uint8 CCD_succeed_flag=0;    //ccd数据已采集标志位
int zhangail=0;
int zhangair=0;
int zhangaijifen=0;

float Speed_L=0;             //用于计算的速度值
int danxian_kai=0;          //用于延时
int duiguan_time=0;
int duiguan_guanbi=0;
int Speed_Change=0;
extern int zhijiao_allow;
extern int zhijiao_hei;
extern int zhijiao_chu;
int zhijiao_allow_time=0;
int zhijiao_hei_time=0;
extern int zhijiao_chu_time;
int Stright_Count=0;//直线延时，在弯道清零，在直道增加
extern int zhijiao_chu_time;
extern int zhijiao_turning_time;
extern int zhijiao_turning;
extern int zhijiao_heheda;
//int Stright_Count=0;//直线延时，在弯道清零，在直道增加
int zhijiao_chu_time1=0;
float Speed_Rate1=0;

float Dynamic_P_L=0;//动态转弯P，可适当降低前瞻，提高系统响应速度，但参数过大过弯会不稳定
float Dynamic_P_R=0;

float Turning_Out_R=0;
float Turning_Out_L=0;
float Turning_Out_R_Lo=0;
float Turning_Out_L_Lo=0;
int podao=0;//检测到坡道后，延时减速
int Shut_Hill=0;//检测到坡道后，3秒不再坡道检测
float qianzhan=0.1;

float Speed_Control_Out=0;//速度控制输出量，直接给定到电机上

void Go(float Speed) //更新电机输出，包括转弯值和速度值
{
#ifndef Canshu  
  
  if(gear==1)
  {
    zhijiao_Power=210;
    Fangxiang_P_R=18.0;//left
    Fangxiang_D_R=0.1;
    Fangxiang_P_L=18.0;//right
    Fangxiang_D_L=0.1;
  }
  else if(gear == 2)
  {
    zhijiao_Power=210;
    Fangxiang_P_R=18.6;//left
    Fangxiang_D_R=0.11;
    Fangxiang_P_L=18.6;//right
    Fangxiang_D_L=0.11;
  }
  
  if(Turning>0)//动态P 很有效果
  {
    Dynamic_P_L=Fangxiang_P_L*(1+Speed_Rate*qianzhan*(1-Turning/64));
    Dynamic_P_R=Fangxiang_P_R*(1+Speed_Rate*qianzhan*(1-Turning/64));
  }
  else
  {
    Dynamic_P_L=Fangxiang_P_L*(1+Speed_Rate*qianzhan*(1+Turning/64));
    Dynamic_P_R=Fangxiang_P_R*(1+Speed_Rate*qianzhan*(1-Turning/64));
  }
  gpio_set   (PTC0,   0);
  if(zhijiao_turning>=zhijiao_turning_time && zhijiao_line_right == 1)
  {    
    Turning_Out_R = -zhijiao_Power;
    Turning_Out_L = -zhijiao_Power;
    //gpio_set   (PTC0,   1);
  }
  else if(zhijiao_turning>=zhijiao_turning_time && zhijiao_line_left == 1)
  {
    Turning_Out_R = zhijiao_Power;
    Turning_Out_L = zhijiao_Power;//
    //gpio_set   (PTC0,   1);
  }else if(zhangaijifen<1000 && zhangai_hei_zuo == 1 && qipaoyanshi > 550) 
  {
    Turning_Out_R = -500;//*Speed_Rate
    Turning_Out_L = -500;
    gpio_set   (PTC0,   1);
  }
  else if(zhangaijifen<1000 && zhangai_hei_you == 1 && qipaoyanshi > 550) //zhangai_time <= 20 && zhangai_hei_you == 1
  {
    Turning_Out_R = 500;
    Turning_Out_L = 500;
    gpio_set   (PTC0,   1);
  }
  else
  {
      Turning_Out_R=Speed_Rate*Dynamic_P_R*Turning+Fangxiang_D_R*GYRO_Y;
      Turning_Out_L=Speed_Rate*Dynamic_P_L*Turning+Fangxiang_D_L*GYRO_Y;
      
      if(qipaoyanshi>=510)//对一个bug的修复
      {
        if(Turning_Out_R>270) Turning_Out_R=270;
        if(Turning_Out_R<-270) Turning_Out_R=-270;
        if(Turning_Out_L>270) Turning_Out_L=270;
        if(Turning_Out_L<-270) Turning_Out_L=-270;
        
      }
      else
      {
        Turning_Out_R=0;
        Turning_Out_L=0;
        
      }
      
      if(podao!=0)
      {
        if(Turning_Out_R>70) Turning_Out_R=70;
        if(Turning_Out_R<-70) Turning_Out_R=-70;
        if(Turning_Out_L>70) Turning_Out_L=70;
        if(Turning_Out_L<-70) Turning_Out_L=-70;
      }
          
  }
#endif  
  
  if(pao_end==0)
  {
    
    if( (Speed+Turning_Out_R)>=0 && (Speed+Turning_Out_R)<=990)
    {
        FTM_PWM_Duty(FTM0, FTM_CH0, (uint32)(Speed+Turning_Out_R) );//r
        FTM_PWM_Duty(FTM0, FTM_CH1, 0);
        //gpio_set   (PTC0,   0); 
    }else if((Speed+Turning_Out_R) < 0 && (Speed+Turning_Out_R) > (0-990))
    {
        FTM_PWM_Duty(FTM0, FTM_CH0, 0);
        FTM_PWM_Duty(FTM0, FTM_CH1, (uint32)(0-Speed));
        //gpio_set   (PTC0,   0); 
    }
    else
    {

      if( Speed+Turning_Out_R>=990)
      {
        FTM_PWM_Duty(FTM0, FTM_CH0,990);//r
        FTM_PWM_Duty(FTM0, FTM_CH1, 0);
      }
      if( Speed+Turning_Out_R<=-990)
      {
        FTM_PWM_Duty(FTM0, FTM_CH0,0);//r
        FTM_PWM_Duty(FTM0, FTM_CH1, 990);
      }
    }
    
    if( (Speed-Turning_Out_L)>=0 && (Speed-Turning_Out_L)<=990)
    {
        FTM_PWM_Duty(FTM0, FTM_CH3, (uint32)(Speed-Turning_Out_L) );
        FTM_PWM_Duty(FTM0, FTM_CH2, 0);
        //gpio_set   (PTC0,   0); 
    }
    else if((Speed-Turning_Out_L) < 0 && (Speed-Turning_Out_L) > (0-990))
    {
        FTM_PWM_Duty(FTM0, FTM_CH3, 0);
        FTM_PWM_Duty(FTM0, FTM_CH2, (uint32)(0-Speed));
        //gpio_set   (PTC0,   0); 
    }
    else
    {
      if( Speed-Turning_Out_L>=990 )
      {
        FTM_PWM_Duty(FTM0, FTM_CH3,990);//r
        FTM_PWM_Duty(FTM0, FTM_CH2, 0);
      }
      if( Speed-Turning_Out_L<=-990 )
      {
        FTM_PWM_Duty(FTM0, FTM_CH3,0);//r
        FTM_PWM_Duty(FTM0, FTM_CH2, 990);
      }
      //gpio_set   (PTC0,   1); 
    }
    
  }
  else 
  {
    FTM_PWM_Duty(FTM0, FTM_CH0, 0);
    FTM_PWM_Duty(FTM0, FTM_CH1, 0);
    FTM_PWM_Duty(FTM0, FTM_CH2, 0);
    FTM_PWM_Duty(FTM0, FTM_CH3, 0);
  }
    
}

//定时器0中断服务
void PIT0_IRQHandler(void)
{
   CCD_Time++;//用来调曝光时
   Speed_Time++;//测一次速度的间隔时间
   DSpeed_Time++;
   Speed_Con++;
   if(zhijiao_chu_shu==1)  zhijiao_chu_time1++;

   if(qipaoyanshi>=510) dengta_yanshi++;
   
   zhangaijifen = (zhangail+zhangair)/2;
   if(zhangaijifen>=340) 
   {
     zhangai_hei_zuo = 0;
     zhangai_hei_you = 0;
     zhangaijifen=0;
     zhangail=0;
     zhangair=0;
   }
   //采集陀螺仪数据
   GYRO_X=Get_Gyro(1,'X');//直立控制陀螺仪
   GYRO_Y=Get_Gyro(1,'Y');//方向控制陀螺仪
        
   //采集加速度计数据
   ACC_X=Get_mma8451_once('X');//这里有一个硕大的bug！
   ACC_Z=Get_mma8451_once('Z');
   
   //以加速度计数据计算角度
   Ang_Acc=Ang_Set+Ang_Cha+Parameter3-186*ACC_Z;
   //计算陀螺仪积分角度+++++互补滤波
   Ang_IGyro-=(GYRO_X+Ang_Error*3.5)*0.00025;//此时的Ang_IGyro就是最终使用的角度
   //计算两个角度之间的偏差
   Ang_Error=Ang_IGyro-Ang_Acc;
   
   //直立PID,fixed parameter
   Speed_L=Ang_IGyro*Zhili_P-GYRO_X*Zhili_D*Zhili_P*0.025;//直立PID
   
   if(100 == Speed_Con)
   {
     Speed_Con=0;
     Speed_Last=Speed_Final_Out;
   }
   
   //计算速度和D速度、I速度
   if(10==Speed_Time)                //50ms测一次速度
   {
       Speed_Time=0;
       
       Check_Speed=(LeftWheel_Count+RightWheel_Count)/2;  //平均一下,Speed_Change没有加上 
       
       //速度
       SpeedQueue[4]=SpeedQueue[3];
       SpeedQueue[3]=SpeedQueue[2];
       SpeedQueue[2]=SpeedQueue[1];
       SpeedQueue[1]=SpeedQueue[0]; 
       SpeedQueue[0]=Check_Speed;
        
       if(qipaoyanshi >1000)
       {
           if( (Check_Speed-SpeedQueue[4])>15 ) Check_Speed=SpeedQueue[4];
           if( (SpeedQueue[4]-Check_Speed)>15 ) Check_Speed=SpeedQueue[4];
       }
       
       if(qipaoyanshi > 1000)
       {
          if(Check_Speed>(Speed_Set-Speed_Down+10)) Check_Speed=Speed_Set-Speed_Down+10;//速度测量限幅
          if(Check_Speed<(Speed_Set-Speed_Down-10)) Check_Speed=Speed_Set-Speed_Down-10;
       }
       
       Speed_Rate=(float)(Check_Speed)/Speed_Set;//速度漂移
       
       Speed_Error=Speed_Set-Speed_Down-Check_Speed;
       LeftWheel_Count=0;       RightWheel_Count=0;

       if(qipaoyanshi>900)
       {
        Speed_Int=Speed_Int+Speed_Error*Sudu_I;
       }
       
       if(Speed_Int>100) Speed_Int=100;
       if(Speed_Int<-100) Speed_Int=-100;
       
       Speed_Control_Out=Sudu_P*(float)Speed_Error+Speed_Int;
       SpeedAverOut=(Speed_Control_Out-Speed_Last)/100;
       
       dengta1=1;
   }
   
   Speed_Final_Out=Speed_Final_Out+SpeedAverOut;
   
   //最终输出限幅
   if(qipaoyanshi > 1000)
   {
       if(Speed_Final_Out>500)      Speed_Final_Out=500;
       else if(Speed_Final_Out<-500)       Speed_Final_Out=-500;
   }
   
   //速度PID
#ifndef Canshu  
   Speed_L=Speed_L-Speed_Final_Out;
#endif   
   
   //启动曝光，可以不曝光  
   if(CCD_Time==0)
   {
     ////StartIntegration1() ;    //启动CCD1,即曝光
     StartIntegration2() ;   //启动CCD2,即曝光
     StartIntegration3() ;
   }else if(CCD_Time==5)//调整曝光时间采集数据
   {
     //ImageCapture1(Pixe1) ;
   }
   else if(CCD_Time==7)//调整曝光时间采集数据
   {
     ImageCapture3(Pixe3) ;
   }else if(CCD_Time==3)//调整曝光时间采集数据
   {
     ImageCapture2(Pixe2) ;
   }
   else if(CCD_Time>=10)//调整采集时间间隔
   {
     CCD_Time=0;
     CCD_succeed_flag=1;//ccd数据已采集
   }
   
   //ccd数据采集一次后，计算左右黑线，防止重复计算

   zhijiao_heheda++;
   if(zhijiao_allow >= 1) zhijiao_allow++;
   if(zhijiao_allow >= 200 && zhijiao_heheda>=1000) {
     zhijiao_allow=300;
     zhijiao_heheda=1010;
     if(gpio_get(PTD12)==1)
     {
       zhijiao_hei=1;
       Speed_Rate1=Speed_Rate;
     }
   }
   
   if(zhijiao_hei>=1) 
   {
     zhijiao_hei++;//检测到黑线标志 
   }
   if(zhijiao_hei>=1000){  //检测到黑线一段时间内检测丢线
    zhijiao_hei=0;       //超时则认为黑线无效
   }
   if(CCD_succeed_flag==1)
   {
     if(qipaoyanshi>520) { //未起跑时不处理主横线
       ccd_getline(); 
       ccd3_work();
     }
     
     CCD_succeed_flag=0;//处理完毕再次打开采集
   }
   //更新电机
   Go(Speed_L);//大于0左拐
   
   if( ((Turning>=0 && Turning<20) || 
        (Turning<=0 && Turning>-20)) 
                                    && qipaoyanshi>720)
   {
     Stright_Count++;
   }
   else
   {
     Stright_Count=0;
   }

   if(Stright_Count>920 &&Coner)
   {
     Ang_Cha=29;
     Stright_Count=921;
     
   }
   else
   {
     Ang_Cha=0;
    Speed_Down=0;
   }
   
   if(gpio_get(PTD13)==1)
   {
      
   }
   else if(qipaoyanshi>700 && Shut_Hill == 0)
   {
      
      podao=500;
      
   }
   
   if(podao!=0)
   {
     //gpio_set   (PTC0,   1);
     podao--;
     Shut_Hill=1500;
   }
   else
   {

     //gpio_set   (PTC0,   0); 
     if(Shut_Hill!=0)
       Shut_Hill--;
   }
   
   
   
   
//   if(dang == 1)
//   {
//       if(zhijiao_chu_time1>=140)
//       {
//         if(gear == 1)
//         {
//            Speed_Down=38;
//            Ang_Cha=50;
//            //gpio_set   (PTC0,   1);
//         }
//         else if(gear == 2)
//         {
//            Speed_Down=25;
//            Ang_Cha=80;
//         }
//       }
//       
//       if(zhijiao_chu_time1>=820)
//       {
//         if(gear == 1)
//         {
//           Speed_Down=38;
//           Ang_Cha=-40;
//           //gpio_set   (PTC0,   1);
//         }
//         else if(gear == 2)
//         {
//           Speed_Down=38;
//           Ang_Cha=-50;
//           //gpio_set   (PTC0,   1);
//         }
//       }
//
//       if(zhijiao_chu_time1>=1900) 
//       {
//         if(gear == 1)
//         {
//           Speed_Down=8;
//           Ang_Cha=-3;
//           //zhijiao_chu_shu=0;
//           
//           //gpio_set   (PTC0,0);
//         }
//         else if(gear == 2)
//         {
//           Speed_Down=38;
//           Ang_Cha=-3;
//           //zhijiao_chu_shu=0;
//           
//          // gpio_set   (PTC0,   0);
//         }
//         zhijiao_turning_time=3;
//       }
//   }
   
   
   //更新上位机输出数据
   //OutData[0]=(float)Turning_Out_R;
   OutData[1]=(float)dengta;
   //OutData[2]=(float)(Check_Speed-SpeedQueue[4]);
   //清中断标志位
   PIT_Flag_Clear(PIT0);       
}

void FTM1_INPUT_IRQHandler(void)    //第一路测速，右轮
{
    uint8 s = FTM1_STATUS;          //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;

    FTM1_STATUS = 0x00;             //清中断标志位

    CHn = 0;
    if( s & (1 << CHn) )
    {
      if(gpio_get(PTC10))
      {
         RightWheel_Count++;
      }
      else
      {
         RightWheel_Count--;
      }
      
      if(zhangai_hei_zuo == 1 || zhangai_hei_you == 1)
      {
         zhangair++;
      }
    }

}

void FTM2_INPUT_IRQHandler(void)    //第一路测速，左轮
{
    uint8 s = FTM2_STATUS;          //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;

    FTM2_STATUS = 0x00;             //清中断标志位

    CHn = 0;
    if( s & (1 << CHn) )
    {

       if(gpio_get(PTC11))             
       {
          LeftWheel_Count--;
       }
       else
       {
          LeftWheel_Count++;
       };
       
       if(zhangai_hei_zuo == 1 || zhangai_hei_you == 1)
       {
         zhangail++;
       }
        /*********************/
    }
}

/*!
 *  @brief      PORTD端口中断服务函数
 *  @since      v5.0
 */
void PORTC_IRQHandler(void)
{

    uint8  n = 0;    //引脚号
    n = 5;
    if(PORTC_ISFR & (1 << n))           //PTD7 触发中断
    {
        dengta++;
      
        PORTC_ISFR  = (1 << n);        //写1清中断标志位


    }

}