#ifndef screenCurrentLightStatusH
#define screenCurrentLightStatusH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenCurrentLightStatus:public SCREEN
{
    public:
      ScreenCurrentLightStatus(void);
      ~ScreenCurrentLightStatus(void);

      void DisplayCurrentLightStatus(void);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

      void vRefreshCurrentLightStatusData(void);
      void vRefreshStepSec(void);

      //OT20140415
      void DisplayDynSegStatus(void);

    private:
      DISP_WORD markSubPhase[4];
      DISP_WORD markStep[4];
      DISP_WORD markPhaseID[2];
      DISP_WORD markPlanID[3];
      DISP_WORD markStepSecCount[4];
      //OT20140415
      DISP_WORD markDynSegStatus;

      void initDispWord(void);                                                  //初始化此畫面的座標等參數
      int cSelect;                                                              //現在選擇的選項

      BYTE ucCurrentLightStatusBitmap[3840];
      void loadBitmapFromFile(void);                                            //將底圖Load進記憶體

      void doKey0Work(void);
      void doKey1Work(void);
      void doKey2Work(void);
      void doKey3Work(void);
      void doKey4Work(void);
      void doKey5Work(void);
      void doKey6Work(void);
      void doKey7Work(void);
      void doKey8Work(void);
      void doKey9Work(void);
      void doKeyAWork(void);
      void doKeyBWork(void);
      void doKeyCWork(void);
      void doKeyDWork(void);
      void doKeyEWork(void);
      void doKeyFWork(void);
      void doKeyF1Work(void);
      void doKeyF2Work(void);
      void doKeyF3Work(void);
      void doKeyF4Work(void);
      void doKeyUPWork(void);
      void doKeyDOWNWork(void);
      void doKeyLEFTWork(void);
      void doKeyRIGHTWork(void);
      void doKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern ScreenCurrentLightStatus screenCurrentLightStatus;
#endif
