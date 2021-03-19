#ifndef screenActuateSegtypeH
#define screenActuateSegtypeH
//---------------------------------------------------------------------------
//OTADD
#include "SCREEN.h"
//---------------------------------------------------------------------------

    struct ActuateExecTime {
      unsigned short int _hour;
      unsigned short int _minute;
      unsigned short int _actmod;
    };
    struct ActuateSegtypeInfo {
      unsigned short int _segment_type;
      unsigned short int _segment_count;
      ActuateExecTime _ptr_seg_exec_time[32];
    }; //���s�w�q,ExecTime�w�̤j��

//---------------------------------------------------------------------------
class ScreenActuateSegtype:public SCREEN
{
    public:
      ScreenActuateSegtype(void);
      ~ScreenActuateSegtype(void);

      void DisplayActuateSegtype(int, int);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:
      DISP_WORD dateWord[8];                                                    //��ܤ������m
      DISP_WORD timeWord[4];                                                    //��ܮɶ�����m
      DISP_WORD itemWord[12];                                                   //��ܽs������m
      DISP_WORD segmentWord[42];                                                //��ܮɶ������e
      DISP_WORD segmnetNoWord[2];                                               //��ܮɶ����s��
      DISP_WORD segmnetCountWord[2];                                            //��ܮɶ����ɬq��
      int cPosition;
      int cSelect;                                                              //0:�ɶ� 1:��� 2:�ɶ������e 3:�ɶ����ɬq��
      int cFace;                                                                //����1~6(�C��6�q)

      int iLastPage;                                                            //from where?

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      void DisplayTime(void);                                                   //��ܮɶ�
      void DisplayItem(int);                                                    //��ܽs��
      void DisplayUpAndDownPage(int);
      void DisplayActuateSegmnetNo(void);                                              //
      void DisplayActuateSegmnetCount(void);                                           //

      void LoadActuateSegtype(int);                                                    //�q�ǫi���̱o��
      int GetNowPlanOfSegtypeCount(void);                                       //�o��{�b�O�@�Ѥ����ĴX�q
      void DisplayActuateSegtypeTable(int);                                            //��ܦb�e���W
      void SaveActuateSegtype(void);                                                   //�g�^�ǫi����

      ActuateSegtypeInfo segment;

      BYTE segtypeBitmap[3840];                                                 //����
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
extern ScreenActuateSegtype screenActuateSegtype;
#endif




