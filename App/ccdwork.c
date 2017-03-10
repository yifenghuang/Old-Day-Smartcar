#include "common.h"
#include "include.h"



uint8 Pixe3[128];

int zhijiao=0;              
int Into_zhijiao_right=0;
int Into_zhijiao_left=0;
int zhijiaoqian=0;
//int guanbizhijiao=0;
int d=0;
int shutdown_zhijiao=0;
int shutdown_zhijiao2=0;
int zhijiao_bai=0;

int danxian_check_sum=0;
int danxian=0;
int danxian_zhi=0;         //本次单线的值
int danxian_zhi2[2]={'0'};        //上次单线的值

int danxian_ccd2=0;        //CCD2所看单线的值
int danxian_get=0;         //CCD2是否检测到单线标志位
int danxian_get1=0;    
/******************** 直角     ***************************/

int zhijiao_allow = 1;    //允许红外，防单线
int zhijiao_hei=0;
int zhijiao_line_left=0;
int zhijiao_line_right=0;
int zhijiao_turning=0;
int zhijiao_CCD3_chu=0;
int zhijiao_chu=0;
int zhijiao_chu_time=0;
extern int zhijiao_hei_time;
int targe_error3=15; 
int zhijiao_chu_shu=0;
int zhijiao_turning_time=2;
int zhijiao_heheda=0;
int zhijiao_hei_error=25;
int po=0;

/**********************二值化*****************************/
int targe_error2=15; 
int16 targe_error=40;                    //下降沿阀值
int danxian_error=35;                    //单线下降沿阀值
int32 ccd_sum=0,ccd_avg=0;               //CCD数值的和、平均值
uint8 ccd_max=0,ccd_min=0;               //CCD最大值、最小值
int16 ccd_target;                        //上次左右黑线的中间值，即中点
int16 ccd_target1; 
int Ccd_getline_flag=0;
int Turning1=0;
uint8 B_flog=0,H_flog=0;                 //全白标志位，即十字
int16 line_right[3]={90};                //右黑线位置存放数组，最近3次的
int16 line_left[3]={30};                //左黑线位置存放数组，最近3次的
int line_right1=90;                    //最新一次的右黑线位置
int line_left1=30;                    //最新一次的右黑线位置
int line_right2=90;                    //最新一次的右黑线位置
int line_left2=30;                    //最新一次的右黑线位置


/*********************    障碍    **********************/
int zhangai_hei=0;
int zhangai_hei_zuo=0;
int zhangai_hei_you=0;
int zhangai_time=0;
int zhangai_time1=7;
int B_flog_wangxing=0;

int Coner=1;
/*********************    ccd2    **********************/


void ccd_deal(uint8 ch[])      //求数组ch，即ccd的平均值、最大值、最小值
{
  uint8 i;
  int8 max=1,min=1;
  for(i=5;i<123;i++)
  {
    if(ch[max]<ch[i]) max=i;
    if(ch[min]>ch[i]) min=i;
    ccd_sum+=ch[i];
  }
  ccd_max=max;
  ccd_min=min;
  ccd_avg=ccd_sum/118;
  ccd_sum=0;//这里没清零平均值会飞起。。。
}

void ccd3_work()
{
  int i=0;

/*****************   弯道检测    ****************************/
  
  Coner=1;
  for(i=30;i<90;i++)
  {
    if(Pixe3[i]-Pixe3[i+4]>targe_error3) 
    {
      Coner=0;
      
      break;
    }
  }
  
/*****************    出直角检测     *************************/
  zhijiao_CCD3_chu=0;
  if(zhijiao_line_right==1 || zhijiao_line_left==1)  
  {
    zhijiao_CCD3_chu=1;
    for(i=10;i<65;i++)
    {
      if(Pixe3[i]-Pixe3[i+4]>targe_error3) 
      {
        zhijiao_CCD3_chu=0;
        break;
      }
    }
  }
  
}
void ccd2_work()
{
  uint8 i=0;

//  int16 error;                                      //error=Pixe2[i]-Pixe2[i-4]，用于判断是否大于阀值
  //uint8 no_left_line=0,no_right_line=0;             //是否寻到左右线的标志位
//  if(danxian==1)        //若入单线，CCD2开始搜单线
//  {
//    danxian_get=0;      //标志位清0
//    for(i=10; i<=117; i++)
//    {
//      if(Pixe2[i-5]-Pixe2[i]>targe_error2 && Pixe2[i+5]-Pixe2[i]>targe_error2)
//      {//CCD2检测单线
//        danxian_ccd2=i;  //单线所在的点
//        danxian_get=1;   //检测到单线标志位
//        break;
//        
//      }
//    }
/*******************************************************************
    
    下面搜索CCD2的左右黑线
    
********************************************************************/    
  
   // if(danxian_ccd2<1) danxian_ccd2=1;       //如果中间值小于1，则默认为0
   // if(danxian_ccd2>126) danxian_ccd2=126;   //如果中间值大于126，则默认为126
   //单线为搜线的中心值

//    if(danxian_get==0)
//    {
//      danxian=0;
//      danxian_ccd2=0;
//      zhijiao_allow = 1;
//      zhijiao_hei=0;
//    }      
//  }
}

void ccd_getline()                                 //求CCD的左右黑线
{
  uint8 i=0;
  int16 error;                                      //error=Pixe2[i]-Pixe2[i-4]，用于判断是否大于阀值
  uint8 no_left_line=0,no_right_line=0;             //是否寻到左右线的标志位
  ccd_deal(Pixe2);                                  //求数组Pixe1，即ccd1的平均值、最大值、最小值
  
  Ccd_getline_flag=0;
  if(danxian==0)            //判断入单线的算法
  {
    for(i=20; i<=107; i++)  
    {
      if(Pixe2[i-4]-Pixe2[i]>danxian_error && Pixe2[i+4]-Pixe2[i]>danxian_error)
      {   //从i点向左右各6个点检测下降沿，判断单线
        if((i-line_left[0]>5)&&(line_right[0]-i>5))
        {   //判断单线是否在左右双线的中间
          danxian=1;       //入单线标志位
          danxian_zhi=i;   //初始化本次及上次的单线值
          danxian_zhi2[1]=danxian_zhi;
          danxian_zhi2[0]=danxian_zhi;
          danxian_get1=1;
          zhijiao_allow = 0;    
          zhijiao_hei=0;
          Ccd_getline_flag=1;
          break;
        }
      }
    }
  }else 
  {
    zhijiao_allow = 0;
    danxian_get1=0;
    for(i=20; i<=106; i++)
    {

      if(Pixe2[i-3]-Pixe2[i]>targe_error && Pixe2[i+3]-Pixe2[i]>targe_error)
      {//从i点向左右各6个点检测下降沿，判断单线
        danxian_zhi=i;
        danxian=1;
        
        if(danxian_zhi>=danxian_zhi2[0])
        {
          if(danxian_zhi-danxian_zhi2[0]<10)
          {//本次单线是否在上次单线的左右15个点之内
            danxian_zhi2[1]=danxian_zhi2[0];
            danxian_zhi2[0]=danxian_zhi;
            danxian_get1=1;
            Ccd_getline_flag=1;
            break;
          }
        }else if(danxian_zhi<danxian_zhi2[0])
        {
          if(danxian_zhi2[0]-danxian_zhi<10)
          {
            danxian_zhi2[1]=danxian_zhi2[0];
            danxian_zhi2[0]=danxian_zhi;
            danxian_get1=1;
            Ccd_getline_flag=1;
            break;
          }
        } 
      }
    }
    if(danxian_get1==0) 
    {
      danxian_zhi2[1]=danxian_zhi2[0];
      danxian_zhi2[0]=danxian_zhi2[0]+(int)(1.0*(danxian_zhi2[0]-danxian_zhi2[1]));
      Ccd_getline_flag=1;
    }
  }
  
  if(danxian == 0)  
    ccd_target=(line_left[0]+line_right[0])/2;       //上次左右黑线的中间值
  else ccd_target=danxian_zhi2[0];
  if(ccd_target<1) ccd_target=1;                    //如果中间值小于1，则默认为0
  if(ccd_target>126) ccd_target=126;                //如果中间值大于126，则默认为126
  
/****************   左线从上次的中间 向左寻线    ********************/  
//  if(zhijiao_hei>=1) zhijiao_hei_error=30;
//  else zhijiao_hei_error=15;
  no_left_line=1;                                   //设没寻到左线
  if(ccd_target>121) ccd_target1=126;
  else ccd_target1=ccd_target+5;
  for(i=ccd_target1;i>10;i--)                         //由于左右有畸变，从第5点之后有效
  {
    error=Pixe2[i]-Pixe2[i-5];        //            //4点之间的下降沿与所定的阀值判断
    if(error>targe_error && Pixe2[i-5]<100 ) {
        no_left_line=0;                             //找到左线
        line_left1=i-2;
        break;
    }
  }
/****************    右线从上次的中间 向右寻线   ********************/
  no_right_line=1;                                  //设没寻到右线
  if(ccd_target<6) ccd_target1=1;
  else ccd_target1=ccd_target-5;
  for(i=ccd_target1;i<118;i++)                       //由于左右有畸变，从第118点之前有效   
  {                                                 
    error=Pixe2[i]-Pixe2[i+5]; //                   //4点之间的下降沿与所定的阀值判断 
    if(error>targe_error && Pixe2[i+5]<100 )  {
        no_right_line=0;                            //找到右线
        line_right1=i+2;
        break;
    }
  } 
/*******************   讨论丢线情况   ****************************/
 // gpio_set   (PTC0,   0);
  if((no_right_line==1)&&(no_left_line==0))        //no_right_line=1，说明丢右线
  {
    //gpio_set   (PTC0,   1);// 
    //if( line_right[0]<114) gpio_set   (PTC0,   1);//
    if(danxian==0 && zhijiao_hei>=1 && line_right[0]<98) {//丢右边，直角，拧一下
      zhijiao_hei=0;
      zhijiao_line_right=1;//进入直角准备右转
    }else 
    {
      Ccd_getline_flag=1;
      line_left[2]=line_left[1];
      line_left[1]=line_left[0];
      line_left[0]=line_left1;                     //更新数组，即最新的左线
      line_right[2]=line_right[1];
      line_right[1]=line_right[0];
      line_right[0]=line_right[0]+(line_left[0]-line_left[1])*10/10;//丢一边的线，按另一边的偏差算
    }
  } else if((no_right_line==0)&&(no_left_line==1))        //no_left_line=1，说明丢左线
  {
    //gpio_set   (PTC0,   1);// 
    //if(line_left[0]>14 ) gpio_set   (PTC0,   1);//
    if(danxian==0 && zhijiao_hei>=1 && line_left[0]>30) {
      zhijiao_hei=0;
      zhijiao_line_left=1;//进入直角准备左转
    }else
    {
      Ccd_getline_flag=1;
      line_right[2]=line_right[1];
      line_right[1]=line_right[0];
      line_right[0]=line_right1;                    //更新数组，即最新的左右线
      line_left[2]=line_left[1];
      line_left[1]=line_left[0];
      line_left[0]=line_left[0]+(line_right[0]-line_right[1])*10/10;  //丢一边的线，按另一边的偏差算
    }
  }else if((no_right_line==1)&&(no_left_line==1))        //都丢线，全白或全黑
  {
    B_flog=0;  
    if(ccd_avg>100) 
    {
      B_flog=1;                       //ccd平均值大于阀值即全白在十字，其他是看外边丢线     
      Ccd_getline_flag=1;
    }
  }else if((no_right_line==0)&&(no_left_line==0))        //不丢线&&(line_right1-line_left1)>30
  {
    if(zhijiao_CCD3_chu==1)
    {
      zhijiao_line_left=0;
      zhijiao_line_right=0;
      zhijiao_CCD3_chu=0;
      zhijiao_turning=0;  
      zhijiao_hei=0;
      zhijiao_chu_shu=1;
      zhijiao_heheda=1;
      //gpio_set   (PTC0,   1);
    }else if(danxian==1 && danxian_get1==0 && line_right1-danxian_zhi2[0]>8 && danxian_zhi2[0]-line_left1>8)
    {
      danxian=0;
      zhijiao_allow = 1;
      zhijiao_hei=0;
    }else if(zhijiao_heheda>=30 && B_flog_wangxing==0 && danxian==0&& line_left1-line_left[0]>12 && line_right[0]-line_right1<3)
    {
      zhangai_hei_zuo=1; 
      //zhangai_time1=4+(zhangai_time1-4)/Speed_Rate;

    }else if(zhijiao_heheda>=30 && B_flog_wangxing==0 && danxian==0 && line_right[0]-line_right1>12 && line_left1-line_left[0]<3)
    {
      zhangai_hei_you=1;   
      //zhangai_time1=4+(zhangai_time1-4)/Speed_Rate;
    } 
    
    
    if(zhijiao_line_right==0 && zhijiao_line_left==0)
    {
      Ccd_getline_flag=1;
      //if(line_right1-line_left1>30)
      //{
        
        line_right[2]=line_right[1];
        line_right[1]=line_right[0];
        line_right[0]=line_right1;                     //更新数组，即最新的左右线
        line_left[2]=line_left[1];
        line_left[1]=line_left[0];
        line_left[0]=line_left1; 
      //}
    }
  }
  
/*********      计算Turning    ***************************/
  if(Ccd_getline_flag==0) 
  {
    Turning1=Turning;
    Turning=Turning+(Turning-Turning1);
  }else 
  {
    Turning1=Turning;
    Turning=64-(line_left[0]+line_right[0])/2;
  }
  
/*********   直角和障碍的辅助时间处理     *******************/
  if(zhijiao_line_right == 1 || zhijiao_line_left == 1)
  {
    zhijiao_turning++;    //看到直角后延时转
  }else zhijiao_turning=0;
  
//  if(zhangai_hei_zuo == 1 || zhangai_hei_you == 1)
//  {
//    zhangai_time++;
//  }else zhangai_time=0;
//  if(zhangai_time>=zhangai_time1)
//  {
//     zhangai_time=0;
//     zhangai_hei_zuo=0;
//     zhangai_hei_you=0;
//     zhangai_time1=8;
//  }
  if(B_flog==1 ) B_flog_wangxing=1;
  if(B_flog_wangxing>=1) B_flog_wangxing++;
  if(B_flog_wangxing>=20) B_flog_wangxing=0;
/*****************    特殊处理      **************************/
  //gpio_set   (PTC0,   0); 
  if(danxian==1) 
  {     
    Turning = (int)(1*(64-danxian_zhi2[0]));
    //gpio_set   (PTC0,   1); 
  }else if(zhijiao_turning>=zhijiao_turning_time && zhijiao_line_right == 1)
  {
    zhijiao_turning=zhijiao_turning_time;
    //gpio_set   (PTC0,   1); 
  }else if(zhijiao_turning>=zhijiao_turning_time && zhijiao_line_left == 1)
  {
    zhijiao_turning=zhijiao_turning_time;
    //gpio_set   (PTC0,   1);  
  }else if(zhangaijifen <= 1500 && zhangai_hei_zuo == 1) //zhangai_time
  {
    //Turning = Turning-20;
    //gpio_set   (PTC0,   1);
  }else if(zhangaijifen <= 1500 && zhangai_hei_you == 1) 
  {
    //Turning = Turning+20;
    //gpio_set   (PTC0,   1);
  }else if(B_flog==1) 
  {
    Turning = 0;//十字
    B_flog=0;
    gpio_set   (PTC0,   1); 
  }

}

void Delay(uint32 a)
{
  uint32 b=900000,c=900000;
  for(;a>0;a--)
    for(;b>0;b--)
      for(;c>0;) c--;
        
}


