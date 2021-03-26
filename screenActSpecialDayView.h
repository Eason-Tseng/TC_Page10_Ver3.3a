#ifndef screenActSpecialDayViewH
#define screenActSpecialDayViewH
//---------------------------------------------------------------------------
#include "SCREEN.h"
#include "CSegmentInfo.h"
//---------------------------------------------------------------------------
class ScreenActSpecialDayView:public SCREEN
{
    public:
      ScreenActSpecialDayView(void);
      ~ScreenActSpecialDayView(void);

      void DisplayActSpecialDayView(int);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:
      DISP_WORD dateWord[8];                                                    //��ܤ�j���m
      DISP_WORD specialDayWord[16];                                             //��ܸ�ƪ���m
      DISP_WORD SpecialDayIDWord[2];                                            //special day No.
      DISP_WORD SegIDWord[2];

      int cPosition;                                                            //
      int cSelect;                                                              //0:���\uFFFD 1:�S�O�餺�e
      int cFace;

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      void DisplayDate(void);                                                   //��ܤ��
      void vDiskplaySegID(int);
      void vDiskplayActSpecialDayID(int);

      CHoliDaySegType specialDaySegtype[13];

      int iSeg;
      unsigned int iSpecialDayID;

      void LoadActSpecialDaySegtype(void);                                         //�q�ǫi���̱o��
      void DisplayActSpecialDaySegtype(int);                                       //��ܦb�e���W

      BYTE specialDayBitmap[3840];                                              //����
      void loadBitmapFromFile(void);                                            //�N����Load�i�O����

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
      void doKeyDefaultWork(void);
};
//---------------------------------------------------------------------------
extern ScreenActSpecialDayView screenActSpecialDayView;
#endif
