
#ifndef _E9101N_HARD_H_
#define _E9101N_HARD_H_

//��Ϊ�����ӿ�����Cʵ�ֵģ�����C++��ʹ��
#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
//���岼������
typedef int BOOL;
#define TRUE	1
#define FALSE	0
#endif

//���������Ϣ
#define WM_SOUND_SWITCH 0xc022

// �����루���к������˷���Ҫ��ȡ�Ĳ����⣬�������Ҫ���ش����룩
typedef int ERRCODE;
#define HARD_ERR_OK				0	//û�д���
#define HARD_ERR_FAILED			-1	//һ����ʧ��
#define HARD_ERR_OSD_NOT_SHOW	-2	//OSD���滹û��ʾ

//��Դ��������KEYUP��KEYDOWN��Ϣ��
#define LC_KEY_PWR_SHORT				51  //�̰�
#define LC_KEY_PWR_LONG                                 52  //����

//	����
//��Ƶ���API��Windows��׼�ӿ�һ����������
BOOL audio_initdev();
#define SoundInit	audio_initdev

#define SOUND_TRACK_LEFT		0	       //��
#define SOUND_TRACK_RIGHT		1	       //��
#define SOUND_TRACK_STEREO		2	       //����
#define SOUND_TRACK_MUTEON		3    	   //����
ERRCODE	SoundSetTrack(int nTrack);	       //��������
int		SoundGetTrack();			       //ȡ�õ�ǰ����


//��Ƶ�л�
#define AUDIO_TRACK_GPS         0           //������Ƶͨ��ΪGPS���
#define AUDIO_TRACK_FM          1           //������Ƶͨ��ΪFM���
#define AUDIO_TRACK_DVBT        2           //������Ƶͨ��ΪDVBT���
ERRCODE	SoundSetAudioSwitch(int nState);   
int  	SoundGetAudioSwitch();			   //ȡ�õ�ǰ��Ƶͨ��
				
//�����WINCE��ϵͳ��������
BOOL    ioc_audmute(DWORD nmute);			//���ؾ���
DWORD   ioc_audamplify(DWORD dwswi);	    //���ع���

DWORD   ioc_get_audamplify();					
DWORD   ioc_get_audmute();					//ȡ�þ���


//��Ļ��Դ
#define LCD_BLIGHT_LOW              0
#define LCD_BLIGHT_MIDDLE           1
#define LCD_BLIGHT_HIGHT            2
ERRCODE LcdBackLightCtrl(int nLight);              //����LCD����
ERRCODE	LcdSetPower(BOOL bPower);	               //������Ļ��Դ
BOOL	LcdGetPower();			      	           //ȡ�õ�ǰ��Ļ��Դ����


// ��Դ����
BOOL	 PowerShutDown();			   //��ػ�
//DWORD ioc_systempwr(DWORD);
int	 PowerGetBatteryRemainPercent();           //ȡ�õ�ص�ǰʣ������ٷֱ�0-100
BOOL	 PowerIsUseBattery();			   //����Ƿ��ڷŵ�״̬
typedef enum _POWERSTAT_E
{
   BATTERY_FAILED = 0,
   BATTERY_ONLINE,
   AC_ONLINE,
   AC_OFFLINE, 
   BATTERY_CHARGING,
   BATTERY_FULL,
   BATTERY_CHANGE_DISABLE
}POWERSTAT_E;
POWERSTAT_E     BatteryStatus();


//ָʾ�ƹ���
//////////////////////////

#define  LED_LEFT_BON      0x01     // ���������
#define  LED_LEFT_BOFF     0x02     // ��������
#define  LED_LEFT_RON      0x04     // ��ƺ����
#define  LED_LEFT_ROFF     0x08     // ��ƺ���
#define  LED_RIGHT_BON     0x10     // �ҵ�������
#define  LED_RIGHT_BOFF    0x20     // �ҵ������
#define  LED_RIGHT_RON     0x40     // �ҵƺ����
#define  LED_RIGHT_ROFF    0x80     // �ҵƺ���
ERRCODE  LEDControl(int nState);
 
//FM���µ�
DWORD ioc_fmdata(DWORD dwfmdata);
/////////////////////////////////////////////

//GPS��Դ
DWORD ioc_gpspwr(DWORD dwgpspwr);
DWORD ioc_get_gpspwr();
DWORD ioc_get_fmdata();

//������
int SoundGetKeySound();
ERRCODE	SoundSetKeySound(BOOL bOpen);

//��Ϣ����
//#define WM_LCD_CLOSE 			WM_USER + 18			//��ҪLCD�ر�ʱ�����������̷߳���
//#define WM_CHILD_EXIT  			WM_USER + 11			//�˳�ʱ�����������̷߳���
//#define WM_POWER_OFF			WM_USER + 12			//��Ҫ��Դ�ر�ʱ�����������̷߳���
// #define WM_LANGUAGE_CHANGE		WM_USER + 14		    //���Ըı���Ϣ	
#define WM_HBCPSTATE			WM_USER + 0x1111        //����״̬�ı���Ϣ
#define WM_HBCPCALLIN			WM_USER + 0x1112        //�����绰������Ϣ
#define WM_HBCPCALLER_NUMBER    WM_USER + 0x1113        //�����绰����绰����
#define WM_HIDEMAINWND          WM_USER + 0x1114        //�ӽ��̷��������̣���������������
#define WM_HBCPRINGIND          WM_USER + 0x1115        //���绰����ʱ��������Ϣ
#define WM_HBCPOFFHOOK          WM_USER + 0x1116        //����ͨ�绰ʱ�Է��Ҷ���Ϣ
#define WM_HBCPOUTGOINGCALL     WM_USER + 0x1117        //���ֻ����Ⲧ��绰ʱ���͵���Ϣ
#define WM_HBCPCONNECTED        WM_USER + 0x1118        //��ģ��ͬ�ֻ���������ʱ
#define WM_VOL                  WM_USER + 0x1119 
#define WM_HBCPACTIVECALL		WM_USER + 0x1120
#define WM_HBCPPAIRLISTCOMPLETE		WM_USER + 0x1121
#define WM_REFRESH_PAGE			WM_USER + 0x1122		//���о�Ԯ������ڶ���������ڵ�ʱˢ��ҳ��

//	SD��
BOOL	SDCardIsInsert();		           //SD���Ƿ���

//SD����Ϣֵ����
/* internal definition */
#define WM_STORAGE_EVENT        	0x219

/* expose to user */
#define STORAGE_EVENT_SD_PLUGIN    	0x8000			//SD�����룬����WPARAM����16λֵ	DBT_DEVICEARRIVAL
#define STORAGE_EVENT_SD_PULLOUT   	0x8004			//SD���γ�������WPARAM����16λֵ	DBT_DEVICEREMOVECOMPLETE

#define WM_CREATE_SET	0xC021//0xC020	//����������Ϣ

//typedef struct savtofile_s  {
//	int vol_field;
//	int bklite_field;
//	int pad[5];
//} savtofile_t;

#ifdef __cplusplus
}
#endif

#endif	//_E9101N_HARD_H_