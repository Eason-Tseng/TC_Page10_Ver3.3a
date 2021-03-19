#include "screenActuateSegtypeEditF2.h"
#include "screenActuateSegtype.h"
#include "screenSegtypeMenu.h"
#include "screenPlanPreview.h"

#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SMEM.h"

#include "screenPlan.h"  //OT Debug 0523

#include "CSTC.h"
#include <stdio.h>
//---------------------------------------------------------------------------
ScreenActuateSegtypeEditF2 screenActuateSegtypeEditF2;
//---------------------------------------------------------------------------
ScreenActuateSegtypeEditF2::ScreenActuateSegtypeEditF2(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
    cSelect=2;
    cFace=1;
}
//---------------------------------------------------------------------------
ScreenActuateSegtypeEditF2::~ScreenActuateSegtypeEditF2(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//ActsegtypeEditF2.bit","rb");
    if (bitmap) {
        fread(segtypeBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::DisplayActuateSegtype(int segtype)
{
try {
    //OTADD
    smem.SetcFace(cACTUATESEGTYPEEDITF2);
    lcd240x128.DISPLAY_GRAPHIC(0,segtypeBitmap,128,30);              
    DisplayTime();                                       

    LoadActuateSegtype(segtype, true);                                                
    vDataToTmpArray();
    int count=GetNowPlanOfActuateSegtypeCount();                                 

    printf("count= %d\n", count);

    cFace=((count-1)/6) +1;     

    printf("cface= %d\n", cFace);

    DisplayActuateSegtypeTable(cFace);                                                 

    int temp=(count-1)%6;                                                   

    if (temp==0)  cPosition=4;  
    else if (temp==3) cPosition=11;
    else if (temp==1) cPosition=18;
    else if (temp==4) cPosition=25;
    else if (temp==2) cPosition=32;
    else if (temp==5) cPosition=39;

    cSelect=2;                                                                
    setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);        

    DisplayActuateSegmnetNo();                               
    DisplayActuateSegmnetCount();                          

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::LoadActuateSegtype(int segtype, bool bReloadFromCSTC)
{
try {
  bool bRet = true;
  if(bReloadFromCSTC) {
    stc.Lock_to_Load_Actuate_Segment_for_Panel(segtype);
  }
    if (bRet) {
        segment._segment_type=stc._act_panel_segment._segment_type;
        segment._segment_count=stc._act_panel_segment._segment_count;
        for (int i=0;i<32;i++) {                     
             segment._ptr_seg_exec_time[i]._hour=0;
             segment._ptr_seg_exec_time[i]._minute=0;
             segment._ptr_seg_exec_time[i]._actmod=0;
        }
        for (int i=0;i<segment._segment_count;i++) {  
             segment._ptr_seg_exec_time[i]._hour=stc._act_panel_segment._ptr_seg_exec_time[i]._hour;
             segment._ptr_seg_exec_time[i]._minute=stc._act_panel_segment._ptr_seg_exec_time[i]._minute;
             segment._ptr_seg_exec_time[i]._actmod=stc._act_panel_segment._ptr_seg_exec_time[i]._actMode;
        }
    } else {
        segment._segment_type=segtype;
        segment._segment_count=32;
        for (int i=0;i<segment._segment_count;i++) {
             segment._ptr_seg_exec_time[i]._hour=0;
             segment._ptr_seg_exec_time[i]._minute=0;
             segment._ptr_seg_exec_time[i]._actmod=0;
        }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
int ScreenActuateSegtypeEditF2::GetNowPlanOfActuateSegtypeCount(void)
{
try {
    int count=1;
    unsigned short cHour=0,cMin=0;
    time_t currentTime=time(NULL);
    struct tm *now=localtime(&currentTime);
    cHour=now->tm_hour;
    cMin=now->tm_min;
    //�H�W�o��{�b�ɶ�

    for (int i=0;i<segment._segment_count;i++) {                    //�H�{���ɬq�@�@���
         if (cHour>=segment._ptr_seg_exec_time[i]._hour) {           //�ɤj��
             count=i+1;                                             //���w�ɬq
         } else {                                                   //�p�G�S�j��
              if (cHour==segment._ptr_seg_exec_time[i]._hour)       //�P�_�O�_����
              //OT Debug 0523
              if (cMin>=segment._ptr_seg_exec_time[i]._minute)       //�P�_�����Ƥj��
                  count=i+1;                                        //���w�ɬq
         }
    }

    if (count>32) count=32;

    return count;
  } catch (...){}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::DisplayActuateSegtypeTable(int page)
{
try {

    unsigned short int usi1, usi2, usi3;

    BYTE space[16]={0};
    cFace=page;
    DisplayItem(page);

    for (int loop=0;loop<1;loop++) {
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[0].X,segmentWord[0].Y,word8x16[segment._ptr_seg_exec_time[page*6-6]._hour/10],segmentWord[0].height,segmentWord[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[1].X,segmentWord[1].Y,word8x16[segment._ptr_seg_exec_time[page*6-6]._hour%10],segmentWord[1].height,segmentWord[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[2].X,segmentWord[2].Y,word8x16[segment._ptr_seg_exec_time[page*6-6]._minute/10],segmentWord[2].height,segmentWord[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[3].X,segmentWord[3].Y,word8x16[segment._ptr_seg_exec_time[page*6-6]._minute%10],segmentWord[3].height,segmentWord[3].width/8);

        usi1  = segment._ptr_seg_exec_time[page*6-6]._actmod / 100;
        usi2 = (segment._ptr_seg_exec_time[page*6-6]._actmod - usi1*100 ) / 10;
        usi3 = (segment._ptr_seg_exec_time[page*6-6]._actmod - usi1*100 - usi2*10) % 10;
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[4].X,segmentWord[4].Y,word8x16[usi1],segmentWord[4].height,segmentWord[4].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[5].X,segmentWord[5].Y,word8x16[usi2],segmentWord[5].height,segmentWord[5].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[6].X,segmentWord[6].Y,word8x16[usi3],segmentWord[6].height,segmentWord[6].width/8);

        if (page<6) {
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[7].X,segmentWord[7].Y,word8x16[segment._ptr_seg_exec_time[page*6-3]._hour/10],segmentWord[7].height,segmentWord[7].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[8].X,segmentWord[8].Y,word8x16[segment._ptr_seg_exec_time[page*6-3]._hour%10],segmentWord[8].height,segmentWord[8].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[9].X,segmentWord[9].Y,word8x16[segment._ptr_seg_exec_time[page*6-3]._minute/10],segmentWord[9].height,segmentWord[9].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[10].X,segmentWord[10].Y,word8x16[segment._ptr_seg_exec_time[page*6-3]._minute%10],segmentWord[10].height,segmentWord[10].width/8);

            usi1  = segment._ptr_seg_exec_time[page*6-3]._actmod / 100;
            usi2 = (segment._ptr_seg_exec_time[page*6-3]._actmod - usi1*100 ) / 10;
            usi3 = (segment._ptr_seg_exec_time[page*6-3]._actmod - usi1*100 - usi2*10) % 10;
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[11].X,segmentWord[11].Y,word8x16[usi1],segmentWord[11].height,segmentWord[11].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[12].X,segmentWord[12].Y,word8x16[usi2],segmentWord[12].height,segmentWord[12].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[13].X,segmentWord[13].Y,word8x16[usi3],segmentWord[13].height,segmentWord[13].width/8);

        } else {
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[7].X,segmentWord[7].Y,space,segmentWord[7].height,segmentWord[7].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[8].X,segmentWord[8].Y,space,segmentWord[8].height,segmentWord[8].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[9].X,segmentWord[9].Y,space,segmentWord[9].height,segmentWord[9].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[10].X,segmentWord[10].Y,space,segmentWord[10].height,segmentWord[10].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[11].X,segmentWord[11].Y,space,segmentWord[11].height,segmentWord[11].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[12].X,segmentWord[12].Y,space,segmentWord[12].height,segmentWord[12].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[13].X,segmentWord[13].Y,space,segmentWord[13].height,segmentWord[13].width/8);
        }
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[14].X,segmentWord[14].Y,word8x16[segment._ptr_seg_exec_time[page*6-5]._hour/10],segmentWord[14].height,segmentWord[14].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[15].X,segmentWord[15].Y,word8x16[segment._ptr_seg_exec_time[page*6-5]._hour%10],segmentWord[15].height,segmentWord[15].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[16].X,segmentWord[16].Y,word8x16[segment._ptr_seg_exec_time[page*6-5]._minute/10],segmentWord[16].height,segmentWord[16].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[17].X,segmentWord[17].Y,word8x16[segment._ptr_seg_exec_time[page*6-5]._minute%10],segmentWord[17].height,segmentWord[17].width/8);

        usi1  = segment._ptr_seg_exec_time[page*6-5]._actmod / 100;
        usi2 = (segment._ptr_seg_exec_time[page*6-5]._actmod - usi1*100 ) / 10;
        usi3 = (segment._ptr_seg_exec_time[page*6-5]._actmod - usi1*100 - usi2*10) % 10;
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[18].X,segmentWord[18].Y,word8x16[usi1],segmentWord[18].height,segmentWord[18].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[19].X,segmentWord[19].Y,word8x16[usi2],segmentWord[19].height,segmentWord[19].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[20].X,segmentWord[20].Y,word8x16[usi3],segmentWord[20].height,segmentWord[20].width/8);
        if (page<6) {
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[21].X,segmentWord[21].Y,word8x16[segment._ptr_seg_exec_time[page*6-2]._hour/10],segmentWord[21].height,segmentWord[21].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[22].X,segmentWord[22].Y,word8x16[segment._ptr_seg_exec_time[page*6-2]._hour%10],segmentWord[22].height,segmentWord[22].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[23].X,segmentWord[23].Y,word8x16[segment._ptr_seg_exec_time[page*6-2]._minute/10],segmentWord[23].height,segmentWord[23].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[24].X,segmentWord[24].Y,word8x16[segment._ptr_seg_exec_time[page*6-2]._minute%10],segmentWord[24].height,segmentWord[24].width/8);

            usi1  = segment._ptr_seg_exec_time[page*6-2]._actmod / 100;
            usi2 = (segment._ptr_seg_exec_time[page*6-2]._actmod - usi1*100 ) / 10;
            usi3 = (segment._ptr_seg_exec_time[page*6-2]._actmod - usi1*100 - usi2*10) % 10;
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[25].X,segmentWord[25].Y,word8x16[usi1],segmentWord[25].height,segmentWord[25].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[26].X,segmentWord[26].Y,word8x16[usi2],segmentWord[26].height,segmentWord[26].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[27].X,segmentWord[27].Y,word8x16[usi3],segmentWord[27].height,segmentWord[27].width/8);

            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[28].X,segmentWord[28].Y,word8x16[segment._ptr_seg_exec_time[page*6-4]._hour/10],segmentWord[28].height,segmentWord[28].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[29].X,segmentWord[29].Y,word8x16[segment._ptr_seg_exec_time[page*6-4]._hour%10],segmentWord[29].height,segmentWord[29].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[30].X,segmentWord[30].Y,word8x16[segment._ptr_seg_exec_time[page*6-4]._minute/10],segmentWord[30].height,segmentWord[30].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[31].X,segmentWord[31].Y,word8x16[segment._ptr_seg_exec_time[page*6-4]._minute%10],segmentWord[31].height,segmentWord[31].width/8);

            usi1  = segment._ptr_seg_exec_time[page*6-4]._actmod / 100;
            usi2 = (segment._ptr_seg_exec_time[page*6-4]._actmod - usi1*100 ) / 10;
            usi3 = (segment._ptr_seg_exec_time[page*6-4]._actmod - usi1*100 - usi2*10) % 10;
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[32].X,segmentWord[32].Y,word8x16[usi1],segmentWord[32].height,segmentWord[32].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[33].X,segmentWord[33].Y,word8x16[usi2],segmentWord[33].height,segmentWord[33].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[34].X,segmentWord[34].Y,word8x16[usi3],segmentWord[34].height,segmentWord[34].width/8);

            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[35].X,segmentWord[35].Y,word8x16[segment._ptr_seg_exec_time[page*6-1]._hour/10],segmentWord[35].height,segmentWord[35].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[36].X,segmentWord[36].Y,word8x16[segment._ptr_seg_exec_time[page*6-1]._hour%10],segmentWord[36].height,segmentWord[36].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[37].X,segmentWord[37].Y,word8x16[segment._ptr_seg_exec_time[page*6-1]._minute/10],segmentWord[37].height,segmentWord[37].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[38].X,segmentWord[38].Y,word8x16[segment._ptr_seg_exec_time[page*6-1]._minute%10],segmentWord[38].height,segmentWord[38].width/8);

            usi1  = segment._ptr_seg_exec_time[page*6-1]._actmod / 100;
            usi2 = (segment._ptr_seg_exec_time[page*6-1]._actmod - usi1*100 ) / 10;
            usi3 = (segment._ptr_seg_exec_time[page*6-1]._actmod - usi1*100 - usi2*10) % 10;
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[39].X,segmentWord[39].Y,word8x16[usi1],segmentWord[39].height,segmentWord[39].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[40].X,segmentWord[40].Y,word8x16[usi2],segmentWord[40].height,segmentWord[40].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[41].X,segmentWord[41].Y,word8x16[usi3],segmentWord[41].height,segmentWord[41].width/8);
        } else {
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[18].X,segmentWord[18].Y,space,segmentWord[18].height,segmentWord[18].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[19].X,segmentWord[19].Y,space,segmentWord[19].height,segmentWord[19].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[20].X,segmentWord[20].Y,space,segmentWord[20].height,segmentWord[20].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[21].X,segmentWord[21].Y,space,segmentWord[21].height,segmentWord[21].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[22].X,segmentWord[22].Y,space,segmentWord[22].height,segmentWord[22].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[23].X,segmentWord[23].Y,space,segmentWord[23].height,segmentWord[23].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[24].X,segmentWord[24].Y,space,segmentWord[24].height,segmentWord[24].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[25].X,segmentWord[25].Y,space,segmentWord[25].height,segmentWord[25].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[26].X,segmentWord[26].Y,space,segmentWord[26].height,segmentWord[26].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[27].X,segmentWord[27].Y,space,segmentWord[27].height,segmentWord[27].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[28].X,segmentWord[28].Y,space,segmentWord[28].height,segmentWord[28].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[29].X,segmentWord[29].Y,space,segmentWord[29].height,segmentWord[29].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[30].X,segmentWord[30].Y,space,segmentWord[30].height,segmentWord[30].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[31].X,segmentWord[31].Y,space,segmentWord[31].height,segmentWord[31].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[32].X,segmentWord[32].Y,space,segmentWord[32].height,segmentWord[32].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[33].X,segmentWord[33].Y,space,segmentWord[33].height,segmentWord[33].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[34].X,segmentWord[34].Y,space,segmentWord[34].height,segmentWord[34].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[35].X,segmentWord[35].Y,space,segmentWord[35].height,segmentWord[35].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[36].X,segmentWord[36].Y,space,segmentWord[36].height,segmentWord[36].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[37].X,segmentWord[37].Y,space,segmentWord[37].height,segmentWord[37].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[38].X,segmentWord[38].Y,space,segmentWord[38].height,segmentWord[38].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[39].X,segmentWord[39].Y,space,segmentWord[39].height,segmentWord[39].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[40].X,segmentWord[40].Y,space,segmentWord[40].height,segmentWord[40].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[41].X,segmentWord[41].Y,space,segmentWord[41].height,segmentWord[41].width/8);
        }
    }

    DisplayUpAndDownPage(page);  DisplayUpAndDownPage(page);

    vDataToTmpArray();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::SaveActuateSegtype(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::initDispWord(void)
{
try {
    for (int i=0;i<4;i++) {
         if (i>=0 && i<=1) timeWord[i].X=80+i*8;
         else if (i>=2 && i<=3) timeWord[i].X=104+(i-2)*8;
         timeWord[i].Y=2;
         timeWord[i].width=8;
         timeWord[i].height=16;
    }

    for (int i=0;i<12;i+=2) {
         if (i>=0 && i<=5) {
             itemWord[i].X=8+(i%2)*8;
             itemWord[i+1].X=8+((i%2)+1)*8;

             itemWord[i].Y=40+(i/2)*17;
             itemWord[i+1].Y=40+(i/2)*17;
         } else if (i>=6 && i<=11) {
             itemWord[i].X=120+(i%2)*8;
             itemWord[i+1].X=120+((i%2)+1)*8;

             itemWord[i].Y=40+((i-6)/2)*17;
             itemWord[i+1].Y=40+((i-6)/2)*17;
         }

         itemWord[i].width=8;
         itemWord[i].height=16;
         itemWord[i+1].width=8;
         itemWord[i+1].height=16;
    }

    for (int i=0;i<42;i++) {
         if (i>=0 && i<=1 )  segmentWord[i].X=32+i*8;
         else if (i>=2 && i<=3)  segmentWord[i].X=56+(i-2)*8;
         else if (i>=4 && i<=6)  segmentWord[i].X=80+(i-4)*8;
         else if (i>=7 && i<=8)  segmentWord[i].X=144+(i-7)*8;
         else if (i>=9 && i<=10)  segmentWord[i].X=168+(i-9)*8;
         else if (i>=11 && i<=13)  segmentWord[i].X=192+(i-11)*8;

         else if (i>=14 && i<=15)  segmentWord[i].X=32+(i-14)*8;
         else if (i>=16 && i<=17)  segmentWord[i].X=56+(i-16)*8;
         else if (i>=18 && i<=20)  segmentWord[i].X=80+(i-18)*8;
         else if (i>=21 && i<=22)  segmentWord[i].X=144+(i-21)*8;
         else if (i>=23 && i<=24)  segmentWord[i].X=168+(i-23)*8;
         else if (i>=25 && i<=27)  segmentWord[i].X=192+(i-25)*8;

         else if (i>=28 && i<=29)  segmentWord[i].X=32+(i-28)*8;
         else if (i>=30 && i<=31)  segmentWord[i].X=56+(i-30)*8;
         else if (i>=32 && i<=34)  segmentWord[i].X=80+(i-32)*8;
         else if (i>=35 && i<=36)  segmentWord[i].X=144+(i-35)*8;
         else if (i>=37 && i<=38)  segmentWord[i].X=168+(i-37)*8;
         else if (i>=39 && i<=41)  segmentWord[i].X=192+(i-39)*8;

         if (i>=0 && i<=13)  segmentWord[i].Y=40;
         else if (i>=14 && i<=27)  segmentWord[i].Y=57;
         else if (i>=28 && i<=41)  segmentWord[i].Y=74;
         segmentWord[i].width=8;
         segmentWord[i].height=16;
    }

    for (int i=0;i<2;i++) {
      segmnetNoWord[i].X=24+i*8;
      segmnetCountWord[i].X=40+i*8;
      segmnetNoWord[i].Y=97;
      segmnetCountWord[i].Y=112;
      segmnetNoWord[i].width=8;
      segmnetCountWord[i].width=8;
      segmnetNoWord[i].height=16;
      segmnetCountWord[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::DisplayTime(void)
{
try {
    unsigned short cHour=0,cMin=0;
    time_t currentTime=time(NULL);
    struct tm *now=localtime(&currentTime);
    cHour=now->tm_hour;
    cMin=now->tm_min;

    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[0].X,timeWord[0].Y,word8x16[cHour/10],timeWord[0].height,timeWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[1].X,timeWord[1].Y,word8x16[cHour%10],timeWord[1].height,timeWord[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[2].X,timeWord[2].Y,word8x16[cMin/10],timeWord[2].height,timeWord[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[3].X,timeWord[3].Y,word8x16[cMin%10],timeWord[3].height,timeWord[3].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::DisplayItem(int page)
{
try {
    BYTE space[16]={0};
    for (int i=0;i<12;i+=2) {
         if ((page*6-(5-i/2))<=32) {
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i].X,itemWord[i].Y,word8x16[(page*6-(5-i/2))/10],itemWord[i].height,itemWord[i].width/8);
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i+1].X,itemWord[i+1].Y,word8x16[(page*6-(5-i/2))%10],itemWord[i+1].height,itemWord[i+1].width/8);
         } else {
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i].X,itemWord[i].Y,space,itemWord[i].height,itemWord[i].width/8);
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i+1].X,itemWord[i+1].Y,space,itemWord[i+1].height,itemWord[i+1].width/8);
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::DisplayUpAndDownPage(int page)
{
try {
    unsigned char temp[16]={0};
    if (page==1) {                          //�Ĥ@��
        lcd240x128.DISPLAY_GRAPHIC_XY(224,40,temp,16,1);
        lcd240x128.DISPLAY_GRAPHIC_XY(224,74,arrow8x16[1],16,1);
    } else if (page==6) {                  //�̫�@��
        lcd240x128.DISPLAY_GRAPHIC_XY(224,40,arrow8x16[0],16,1);
        lcd240x128.DISPLAY_GRAPHIC_XY(224,74,temp,16,1);
    } else {                                //������
        lcd240x128.DISPLAY_GRAPHIC_XY(224,40,arrow8x16[0],16,1);
        lcd240x128.DISPLAY_GRAPHIC_XY(224,74,arrow8x16[1],16,1);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::DisplayActuateSegmnetNo(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(segmnetNoWord[0].X,segmnetNoWord[0].Y,word8x16[segment._segment_type/10],segmnetNoWord[0].height,segmnetNoWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(segmnetNoWord[1].X,segmnetNoWord[1].Y,word8x16[segment._segment_type%10],segmnetNoWord[1].height,segmnetNoWord[1].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::DisplayActuateSegmnetCount(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(segmnetCountWord[0].X,segmnetCountWord[0].Y,word8x16[segment._segment_count/10],segmnetCountWord[0].height,segmnetCountWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(segmnetCountWord[1].X,segmnetCountWord[1].Y,word8x16[segment._segment_count%10],segmnetCountWord[1].height,segmnetCountWord[1].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x80:
          doKey0Work();
        break;
        case 0x81:
          doKey1Work();
        break;
        case 0x82:
          doKey2Work();
        break;
        case 0x83:
          doKey3Work();
        break;
        case 0x84:
          doKey4Work();
        break;
        case 0x85:
          doKey5Work();
        break;
        case 0x86:
          doKey6Work();
        break;
        case 0x87:
          doKey7Work();
        break;
        case 0x88:
          doKey8Work();
        break;
        case 0x89:
          doKey9Work();
        break;
        case 0x8A:
          doKeyAWork();
        break;
        case 0x8B:
          doKeyBWork();
        break;
        case 0x8C:
          doKeyCWork();
        break;
        case 0x8D:
          doKeyDWork();
        break;
        case 0x8E:
          doKeyEWork();
        break;
        case 0x8F:
          doKeyFWork();
        break;
        case 0x90:
          doKeyF1Work();
        break;
        case 0x91:
          doKeyF2Work();
        break;
        case 0x92:
          doKeyF3Work();
        break;
        case 0x93:
          doKeyF4Work();
        break;
        case 0x94:
          doKeyUPWork();
        break;
        case 0x95:
          doKeyDOWNWork();
        break;
        case 0x96:
          doKeyLEFTWork();
        break;
        case 0x97:
          doKeyRIGHTWork();
        break;
        case 0x98:
          doKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey0Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 0; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[0],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey1Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 1; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[1],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey2Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 2; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[2],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey3Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 3; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[3],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey4Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 4; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[4],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey5Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 5; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[5],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey6Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 6; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[6],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey7Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 7; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[7],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey8Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 8; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[8],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKey9Work(void)
{
  if(cSelect == 2) { ucSegmentWord[cPosition] = 9; lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[cPosition].X,segmentWord[cPosition].Y,word8x16[9],segmentWord[cPosition].height,segmentWord[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyAWork(void)
{
  unsigned short int usiCopySeg = smem.vGetScreenCopySegID();
  if(usiCopySeg != INVAILDVALUE) {
    vAppendSeg();
    vDataToTmpArray();
    int count=GetNowPlanOfActuateSegtypeCount();                                       //�o��{�b�ĴX�q
    cFace=((count-1)/6) +1;     //OT Debug 0523             from 1~6           //���I�ĴX��
    DisplayActuateSegtypeTable(cFace);                                                 //�I�ӭ�
    int temp=(count-1)%6;                                                       //�P�_���q�O�����ĴX��

    if (temp==0)  cPosition=4;   //OT Debug 0523                               //from 1~6
    else if (temp==3) cPosition=11;
    else if (temp==1) cPosition=18;
    else if (temp==4) cPosition=25;
    else if (temp==2) cPosition=32;
    else if (temp==5) cPosition=39;

    cSelect=2;                                                                  //�{�b�b�ɶ������
    setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);        //�eCursor

    DisplayActuateSegmnetNo();                                 //�e�ɶ����s��
    DisplayActuateSegmnetCount();                              //�e�ɬq��
  }

}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyEWork(void)
{
  if (cFace>1) {
    clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    vTmpArrayToData();
//    cPosition=0;
    cSelect=2;
    DisplayTime();
    DisplayActuateSegtypeTable(cFace-1);
    setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
  }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyFWork(void)
{
  if (cFace<6) {
    clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    vTmpArrayToData();
//    cPosition=0;
    cSelect=2;
    DisplayTime();
    DisplayActuateSegtypeTable(cFace+1);
    setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
  }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyF3Work(void)
{
  bool bRet;
  vTmpArrayToData();
  vCalSegCount();
  bRet = vCheckRationality();
  printf("printfMsg goto EnterWork!\n");

  if(bRet) {
    vSaveActuateSeg(true);
    screenActuateSegtype.DisplayActuateSegtype(segment._segment_type, INVAILDVALUE);
    //OT Debug 951128
    smem.vSetVDUpdateDB(1,true);
  }
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyF4Work(void)
{
    screenActuateSegtype.DisplayActuateSegtype(segment._segment_type, INVAILDVALUE);
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyUPWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect==1) {
    } else if (cSelect==2) {
               clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
               if (cPosition>=7 && cPosition<=13 ) cPosition += 21;
               else if (cPosition < 7) {
                 if (cFace>1) {
                   vTmpArrayToData();
                   DisplayTime();
                   DisplayActuateSegtypeTable(cFace-1);
                   cPosition=41;
                   cSelect=2;
                 }

               }
               else if (cPosition>=14 && cPosition<=41) cPosition-=14;
               else {
                   if (cFace>1) {
                     /* disable change page
                       vTmpArrayToData();
                       DisplayDate();
                       DisplayTime();
                       DisplaySegtypeTable(cFace-1);
                       cPosition+=35;
                       cSelect=2;
                     */
                   }
               }
               setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyDOWNWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect==1) {
    } else if (cSelect==2) {
               clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
               if (cPosition>=0 && cPosition<=27) { cPosition+=14; //OT Debug 0523
               } else if(cPosition>=28 && cPosition<=34) {
                 cPosition -= 21;
               } else if(cPosition > 34) {
                 if (cFace<6) {
                   vTmpArrayToData();
                   DisplayTime();
                   DisplayActuateSegtypeTable(cFace+1);
                   cPosition=0;
                   cSelect=2;
                 }
               } else {
                   if (cFace<6) {
                     /* disable change page
                       vTmpArrayToData();
                       DisplayDate();
                       DisplayTime();
                       DisplaySegtypeTable(cFace+1);
                       cPosition -= 35;
                       cSelect=2;
                       */
                   }
               }
               setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyLEFTWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect==1) {
    } else if (cSelect==2) {
               clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);

               if( cPosition >0 && cPosition <= 6 ||
                   cPosition >7 && cPosition <= 13||
                   cPosition >14 && cPosition <= 20 ||
                   cPosition >21 && cPosition <= 27 ||
                   cPosition >28 && cPosition <= 34 ||
                   cPosition >35 && cPosition <= 41    )
               {
                 cPosition--;
               } else if ( cPosition == 14 || cPosition == 21 || cPosition == 28 || cPosition == 35 )
               {
                 cPosition-=8;
               } else if ( cPosition == 7)
               {
                 cPosition = 34;
               } else if ( cPosition == 0 )
               {
                 if (cFace>1) {
                     vTmpArrayToData();
                     DisplayTime();
                     DisplayActuateSegtypeTable(cFace-1);
                     cPosition=41;
                     cSelect=2;
                 }
               }


//               if (cPosition==0) cPosition=1;
//               cPosition--;
               setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyRIGHTWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect==1) {
    } else if (cSelect==2) {
               clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);

               if( cPosition >=0 && cPosition < 6 ||
                   cPosition >=7 && cPosition < 13||
                   cPosition >=14 && cPosition < 20 ||
                   cPosition >=21 && cPosition < 27 ||
                   cPosition >=28 && cPosition < 34 ||
                   cPosition >=35 && cPosition < 41    )
               {
                 cPosition++;
               } else if ( cPosition == 6 || cPosition == 13 || cPosition == 20 || cPosition == 27 )
               {
                 cPosition+=8;
               } else if ( cPosition == 34)
               {
                 cPosition = 7;
               } else if ( cPosition == 41 )
               {
                 if (cFace<6) {
                     vTmpArrayToData();
                     DisplayTime();
                     DisplayActuateSegtypeTable(cFace+1);
                     cPosition=0;
                     cSelect=2;
                 }
               }

               setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::doKeyEnterWork(void)
{
try{
  // bool bRet;
  // int iSegLocate;

  // vTmpArrayToData();
  // vCalSegCount();
  // bRet = vCheckRationality();
  // printf("printfMsg goto EnterWork!\n");

  // if(bRet) {
  //   iSegtypeEditing = segment._segment_type;
  //   vSaveActuateSeg(false);

  //   if (cPosition>=0 && cPosition<=6) iSegLocate = (cFace-1)*6 + 0;
  //   else if (cPosition>=7 && cPosition<=13 && cFace<6)  iSegLocate = (cFace-1)*6 + 3;
  //   else if (cPosition>=14 && cPosition<=20)  iSegLocate = (cFace-1)*6 + 1;
  //   else if (cPosition>=21 && cPosition<=27 && cFace<6) iSegLocate = (cFace-1)*6 + 4;
  //   else if (cPosition>=28 && cPosition<=34 && cFace<6) iSegLocate = (cFace-1)*6 + 2;
  //   else if (cPosition>=35 && cPosition<=41 && cFace<6) iSegLocate = (cFace-1)*6 + 5;

  //   screenPlanPreview.DisplayPlanPreview(segment._ptr_seg_exec_time[iSegLocate]._planid);
  // }

}catch(...){}
}
//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::vDataToTmpArray(void)
{
try{
    if(cFace < 6) {
            ucSegmentWord[0] = (segment._ptr_seg_exec_time[(cFace-1)*6]._hour/10)%10;
            ucSegmentWord[1] =  segment._ptr_seg_exec_time[(cFace-1)*6]._hour%10;
            ucSegmentWord[2] = (segment._ptr_seg_exec_time[(cFace-1)*6]._minute/10)%10;
            ucSegmentWord[3] =  segment._ptr_seg_exec_time[(cFace-1)*6]._minute%10;
            ucSegmentWord[4] = (segment._ptr_seg_exec_time[(cFace-1)*6]._actmod/100)%10;
            ucSegmentWord[5] = (segment._ptr_seg_exec_time[(cFace-1)*6]._actmod/10)%10;
            ucSegmentWord[6] =  segment._ptr_seg_exec_time[(cFace-1)*6]._actmod%10;

            ucSegmentWord[7] = (segment._ptr_seg_exec_time[(cFace-1)*6+3]._hour/10)%10;
            ucSegmentWord[8] =  segment._ptr_seg_exec_time[(cFace-1)*6+3]._hour%10;
            ucSegmentWord[9] = (segment._ptr_seg_exec_time[(cFace-1)*6+3]._minute/10)%10;
            ucSegmentWord[10] =  segment._ptr_seg_exec_time[(cFace-1)*6+3]._minute%10;
            ucSegmentWord[11] = (segment._ptr_seg_exec_time[(cFace-1)*6+3]._actmod/100)%10;
            ucSegmentWord[12] = (segment._ptr_seg_exec_time[(cFace-1)*6+3]._actmod/10)%10;
            ucSegmentWord[13] =  segment._ptr_seg_exec_time[(cFace-1)*6+3]._actmod%10;

            ucSegmentWord[14] = (segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour/10)%10;
            ucSegmentWord[15] =  segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour%10;
            ucSegmentWord[16] = (segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute/10)%10;
            ucSegmentWord[17] =  segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute%10;
            ucSegmentWord[18] = (segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod/100)%10;
            ucSegmentWord[19] = (segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod/10)%10;
            ucSegmentWord[20] =  segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod%10;

            ucSegmentWord[21] = (segment._ptr_seg_exec_time[(cFace-1)*6+4]._hour/10)%10;
            ucSegmentWord[22] =  segment._ptr_seg_exec_time[(cFace-1)*6+4]._hour%10;
            ucSegmentWord[23] = (segment._ptr_seg_exec_time[(cFace-1)*6+4]._minute/10)%10;
            ucSegmentWord[24] =  segment._ptr_seg_exec_time[(cFace-1)*6+4]._minute%10;
            ucSegmentWord[25] = (segment._ptr_seg_exec_time[(cFace-1)*6+4]._actmod/100)%10;
            ucSegmentWord[26] = (segment._ptr_seg_exec_time[(cFace-1)*6+4]._actmod/10)%10;
            ucSegmentWord[27] =  segment._ptr_seg_exec_time[(cFace-1)*6+4]._actmod%10;

            ucSegmentWord[28] = (segment._ptr_seg_exec_time[(cFace-1)*6+2]._hour/10)%10;
            ucSegmentWord[29] =  segment._ptr_seg_exec_time[(cFace-1)*6+2]._hour%10;
            ucSegmentWord[30] = (segment._ptr_seg_exec_time[(cFace-1)*6+2]._minute/10)%10;
            ucSegmentWord[31] =  segment._ptr_seg_exec_time[(cFace-1)*6+2]._minute%10;
            ucSegmentWord[32] = (segment._ptr_seg_exec_time[(cFace-1)*6+2]._actmod/100)%10;
            ucSegmentWord[33] = (segment._ptr_seg_exec_time[(cFace-1)*6+2]._actmod/10)%10;
            ucSegmentWord[34] =  segment._ptr_seg_exec_time[(cFace-1)*6+2]._actmod%10;

            ucSegmentWord[35] = (segment._ptr_seg_exec_time[(cFace-1)*6+5]._hour/10)%10;
            ucSegmentWord[36] =  segment._ptr_seg_exec_time[(cFace-1)*6+5]._hour%10;
            ucSegmentWord[37] = (segment._ptr_seg_exec_time[(cFace-1)*6+5]._minute/10)%10;
            ucSegmentWord[38] =  segment._ptr_seg_exec_time[(cFace-1)*6+5]._minute%10;
            ucSegmentWord[39] = (segment._ptr_seg_exec_time[(cFace-1)*6+5]._actmod/100)%10;
            ucSegmentWord[40] = (segment._ptr_seg_exec_time[(cFace-1)*6+5]._actmod/10)%10;
            ucSegmentWord[41] =  segment._ptr_seg_exec_time[(cFace-1)*6+5]._actmod%10;
    } else {
      ucSegmentWord[0] = (segment._ptr_seg_exec_time[(cFace-1)*6]._hour/10)%10;
      ucSegmentWord[1] =  segment._ptr_seg_exec_time[(cFace-1)*6]._hour%10;
      ucSegmentWord[2] = (segment._ptr_seg_exec_time[(cFace-1)*6]._minute/10)%10;
      ucSegmentWord[3] =  segment._ptr_seg_exec_time[(cFace-1)*6]._minute%10;
      ucSegmentWord[4] = (segment._ptr_seg_exec_time[(cFace-1)*6]._actmod/100)%10;
      ucSegmentWord[5] = (segment._ptr_seg_exec_time[(cFace-1)*6]._actmod/10)%10;
      ucSegmentWord[6] =  segment._ptr_seg_exec_time[(cFace-1)*6]._actmod%10;

      ucSegmentWord[14] = (segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour/10)%10;
      ucSegmentWord[15] =  segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour%10;
      ucSegmentWord[16] = (segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute/10)%10;
      ucSegmentWord[17] =  segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute%10;
      ucSegmentWord[18] = (segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod/100)%10;
      ucSegmentWord[19] = (segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod/10)%10;
      ucSegmentWord[20] =  segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod%10;

    }

/*
    segmnetNoWord[0] = segment._segment_type / 10;
    segmnetNoWord[1] = segment._segment_type % 10;
    segmnetCountWord[0] = segment._segment_count / 10;
    segmnetCountWord[1] = segment._segment_count % 10;
*/
}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::vTmpArrayToData(void)
{
try{
    if(cFace < 6) {

            segment._ptr_seg_exec_time[(cFace-1)*6]._hour = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6]._hour =  ucSegmentWord[0]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6]._hour += ucSegmentWord[1];
            segment._ptr_seg_exec_time[(cFace-1)*6]._minute = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6]._minute = ucSegmentWord[2]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6]._minute += ucSegmentWord[3];
            segment._ptr_seg_exec_time[(cFace-1)*6]._actmod = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6]._actmod = ucSegmentWord[4]*100;
            segment._ptr_seg_exec_time[(cFace-1)*6]._actmod += ucSegmentWord[5]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6]._actmod += ucSegmentWord[6];

            segment._ptr_seg_exec_time[(cFace-1)*6+3]._hour = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+3]._hour =  ucSegmentWord[7]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+3]._hour += ucSegmentWord[8];
            segment._ptr_seg_exec_time[(cFace-1)*6+3]._minute = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+3]._minute = ucSegmentWord[9]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+3]._minute += ucSegmentWord[10];
            segment._ptr_seg_exec_time[(cFace-1)*6+3]._actmod = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+3]._actmod = ucSegmentWord[11]*100;
            segment._ptr_seg_exec_time[(cFace-1)*6+3]._actmod += ucSegmentWord[12]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+3]._actmod += ucSegmentWord[13];

            segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour =  ucSegmentWord[14]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour += ucSegmentWord[15];
            segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute = ucSegmentWord[16]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute += ucSegmentWord[17];
            segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod = ucSegmentWord[18]*100;
            segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod += ucSegmentWord[19]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod += ucSegmentWord[20];

            segment._ptr_seg_exec_time[(cFace-1)*6+4]._hour = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+4]._hour =  ucSegmentWord[21]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+4]._hour += ucSegmentWord[22];
            segment._ptr_seg_exec_time[(cFace-1)*6+4]._minute = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+4]._minute = ucSegmentWord[23]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+4]._minute += ucSegmentWord[24];
            segment._ptr_seg_exec_time[(cFace-1)*6+4]._actmod = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+4]._actmod = ucSegmentWord[25]*100;
            segment._ptr_seg_exec_time[(cFace-1)*6+4]._actmod += ucSegmentWord[26]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+4]._actmod += ucSegmentWord[27];

            segment._ptr_seg_exec_time[(cFace-1)*6+2]._hour = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+2]._hour =  ucSegmentWord[28]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+2]._hour += ucSegmentWord[29];
            segment._ptr_seg_exec_time[(cFace-1)*6+2]._minute = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+2]._minute = ucSegmentWord[30]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+2]._minute += ucSegmentWord[31];
            segment._ptr_seg_exec_time[(cFace-1)*6+2]._actmod = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+2]._actmod = ucSegmentWord[32]*100;
            segment._ptr_seg_exec_time[(cFace-1)*6+2]._actmod += ucSegmentWord[33]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+2]._actmod += ucSegmentWord[34];

            segment._ptr_seg_exec_time[(cFace-1)*6+5]._hour = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+5]._hour =  ucSegmentWord[35]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+5]._hour += ucSegmentWord[36];
            segment._ptr_seg_exec_time[(cFace-1)*6+5]._minute = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+5]._minute = ucSegmentWord[37]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+5]._minute += ucSegmentWord[38];
            segment._ptr_seg_exec_time[(cFace-1)*6+5]._actmod = 0;
            segment._ptr_seg_exec_time[(cFace-1)*6+5]._actmod = ucSegmentWord[39]*100;
            segment._ptr_seg_exec_time[(cFace-1)*6+5]._actmod += ucSegmentWord[40]*10;
            segment._ptr_seg_exec_time[(cFace-1)*6+5]._actmod += ucSegmentWord[41];

    } else {

      segment._ptr_seg_exec_time[(cFace-1)*6]._hour = 0;
      segment._ptr_seg_exec_time[(cFace-1)*6]._hour =  ucSegmentWord[0]*10;
      segment._ptr_seg_exec_time[(cFace-1)*6]._hour += ucSegmentWord[1];
      segment._ptr_seg_exec_time[(cFace-1)*6]._minute = 0;
      segment._ptr_seg_exec_time[(cFace-1)*6]._minute = ucSegmentWord[2]*10;
      segment._ptr_seg_exec_time[(cFace-1)*6]._minute += ucSegmentWord[3];
      segment._ptr_seg_exec_time[(cFace-1)*6]._actmod = 0;
      segment._ptr_seg_exec_time[(cFace-1)*6]._actmod = ucSegmentWord[4]*100;
      segment._ptr_seg_exec_time[(cFace-1)*6]._actmod += ucSegmentWord[5]*10;
      segment._ptr_seg_exec_time[(cFace-1)*6]._actmod += ucSegmentWord[6];

      segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour = 0;
      segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour =  ucSegmentWord[14]*10;
      segment._ptr_seg_exec_time[(cFace-1)*6+1]._hour += ucSegmentWord[15];
      segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute = 0;
      segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute = ucSegmentWord[16]*10;
      segment._ptr_seg_exec_time[(cFace-1)*6+1]._minute += ucSegmentWord[17];
      segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod = 0;
      segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod = ucSegmentWord[18]*100;
      segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod += ucSegmentWord[19]*10;
      segment._ptr_seg_exec_time[(cFace-1)*6+1]._actmod += ucSegmentWord[20];

    }

/*
    segment._segment_type = segmnetNoWord[0];
    segment._segment_type += segmnetNoWord[1];
    segment._segment_count = segmnetCountWord[0];
    segment._segment_count += segmnetCountWord[1];
*/
}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenActuateSegtypeEditF2::vCheckRationality(void)
{
try{

  bool bRET;
  bRET = true;
  unsigned short int usiTMP = 0;

  for(int i = 0; i < segment._segment_count; i++) {
    if( segment._ptr_seg_exec_time[i]._hour   > 23)  { bRET = false; printf("printfMsg error at hour.\n"); }
    if( segment._ptr_seg_exec_time[i]._minute > 59)  { bRET = false; printf("printfMsg error at min.\n"); }
    if( segment._ptr_seg_exec_time[i]._actmod >  5) { bRET = false; printf("printfMsg error at act mod.\n"); }
  }

/*
  if(smem.vGetTCPhasePlanSegTypeData(TC_Plan, segment._ptr_seg_exec_time[i]._planid) == false) {
    bRET = false;
  }
*/

  if(segment._ptr_seg_exec_time[0]._hour == 0 && segment._ptr_seg_exec_time[0]._minute == 0)
  { }                                                                         //do nothing
  else {
    bRET = false;
    bRET = false; printf("printfMsg error at first seg.\n");
  }

  for(int i = 1; i < segment._segment_count; i++) {
    if(segment._ptr_seg_exec_time[i-1]._hour >= segment._ptr_seg_exec_time[i]._hour)
    {  if(segment._ptr_seg_exec_time[i-1]._minute >= segment._ptr_seg_exec_time[i]._minute) { bRET = false; printf("printfMsg error at cmp: %d %d %d.\n", i,segment._ptr_seg_exec_time[i-1]._hour,segment._ptr_seg_exec_time[i]._minute);}
    }
  }

  return bRET;

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenActuateSegtypeEditF2::vCalSegCount(void)
{
try{
  int iSegCount;
  iSegCount = 1;

  for(int i = 0; i < 32; i++) {
    if(segment._ptr_seg_exec_time[iSegCount]._hour == 0 && segment._ptr_seg_exec_time[iSegCount]._minute == 0)
      break;
    else
      iSegCount++;
  }

  segment._segment_count = iSegCount;

  return true;

//  while(segment._ptr_seg_exec_time[iSegCount]._hour == 0 && segment._ptr_seg_exec_time[iSegCount]._minute == 0)

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenActuateSegtypeEditF2::vSaveActuateSeg(bool bSaveToCSTC)
{
try{
  printf("printfMsg goto vSaveSeg!\n");

  stc.Lock_to_Reset_Actuate_Segment_for_Panel(segment._segment_type, segment._segment_count);

      stc._act_panel_segment._segment_type = segment._segment_type;
      stc._act_panel_segment._segment_count = segment._segment_count;

      for (int i=0; i<segment._segment_count; i++) {
           stc._act_panel_segment._ptr_seg_exec_time[i]._hour = segment._ptr_seg_exec_time[i]._hour;
           stc._act_panel_segment._ptr_seg_exec_time[i]._minute = segment._ptr_seg_exec_time[i]._minute;
           stc._act_panel_segment._ptr_seg_exec_time[i]._actMode = segment._ptr_seg_exec_time[i]._actmod;
      }

      if(bSaveToCSTC)
        stc.Lock_to_Save_Actuate_Segment_from_Panel();



}catch(...){}
}


//---------------------------------------------------------------------------
bool ScreenActuateSegtypeEditF2::vAppendSeg(void)
{
try{
  unsigned short int usiOriSeg = segment._segment_type;
  unsigned short int usiCopySeg = smem.vGetScreenCopySegID();
  stc.Lock_to_Load_Actuate_Segment_for_Panel(usiCopySeg);

  segment._segment_type = usiOriSeg;
  printf("printfMsg segment._segment_type:%d\n", segment._segment_type);
  segment._segment_count = stc._act_panel_segment._segment_count;
  for (int i=0;i<32;i++) {                        //���M��
    segment._ptr_seg_exec_time[i]._hour=0;
    segment._ptr_seg_exec_time[i]._minute=0;
    segment._ptr_seg_exec_time[i]._actmod=0;
  }
  for (int i=0;i<segment._segment_count;i++) {    //�A���s�g
    segment._ptr_seg_exec_time[i]._hour=stc._act_panel_segment._ptr_seg_exec_time[i]._hour;
    segment._ptr_seg_exec_time[i]._minute=stc._act_panel_segment._ptr_seg_exec_time[i]._minute;
    segment._ptr_seg_exec_time[i]._actmod=stc._act_panel_segment._ptr_seg_exec_time[i]._actMode;
  }
  return true;
}catch(...){}
}
