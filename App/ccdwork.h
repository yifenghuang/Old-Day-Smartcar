#ifndef  ccdwork_H
#define  ccdwork_H  

extern int16 line_left[3];//左黑线位置存放数组，最近3次的
extern int16 line_right[3];//右黑线位置存放数组，最近3次的
extern int zhangai_hei;
extern int zhangai_hei_zuo;
extern int zhangai_hei_you;
extern int zhangai_time;


void ccd_deal(uint8 ch[]);//求数组ch，即ccd的平均值、最大值、最小值
void ccd_getline();       //求CCD的左右黑线
void Delay(uint32 a);
void ccd2_work();
void ccd3_work();
void zhijiao_hei_work();

#endif
