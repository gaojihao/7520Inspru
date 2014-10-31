
#ifndef   _SKINPUB_H
#define   _SKINPUB_H

#include <windows.h>
//#include <WinSock2.h>
//
// The draw style.
//

//            ��������    ǰ������   ǰ��ͼƬ
// 00000000---00000000---00000000---00000000
//
#define  KDS_TEXT_HLEFT         0x00000001	 //����ˮƽ�����
#define  KDS_TEXT_HRIGHT        0x00000002	 //����ˮƽ�Ҷ���
#define  KDS_TEXT_HCENTER       0x00000004   //����ˮƽ����
#define  KDS_TEXT_VTOP          0x00000010   //���ִ�ֱ�϶���
#define  KDS_TEXT_VBOTTOM       0x00000020   //���ִ�ֱ�¶��� 
#define  KDS_TEXT_VCENTER       0x00000040   //���ִ�ֱ���ж���
#define  KDS_TEXT_USERDEF       0x00000080   //������ָ��λ�û����ڶԻ����У�

#define  KDS_ICON_HEFT          0x00000001	 //ͼ��ˮƽ�����
#define  KDS_ICON_HRIGHT        0x00000002	 //ͼ��ˮƽ�Ҷ���
#define  KDS_ICON_HCENTER       0x00000004   //ͼ��ˮƽ����
#define  KDS_ICON_VTOP          0x00000010   //ͼ�괹ֱ�϶���
#define  KDS_ICON_VBOTTOM       0x00000020   //ͼ�괹ֱ�¶��� 
#define  KDS_ICON_VCENTER       0x00000040   //ͼ�괹ֱ���ж���

#define  KDS_CTRL_BKGBMP        0x00010000   //������ͼ��
#define  KDS_CTRL_BKGRGB        0x00020000	 //��������ɫ
#define  KDS_CTRL_FKTEXT        0x00040000	 //ǰ��������
#define  KDS_CTRL_FKICON        0x00080000	 //ǰ����ͼ��

#define  KDS_AUTOSIZE           0x01000000   //�ؼ��Զ���СΪͼƬ��С
#define  KDS_TRANSPARENT        0x02000000   //�ؼ�����͸��


#define  KDS_BTN_AUTOBMP       ( KDS_CTRL_BKGBMP | KDS_AUTOSIZE)
#define  KDS_BTN_DFTTEXT       ( KDS_TEXT_VBOTTOM | KDS_TEXT_HCENTER | KDS_CTRL_FKTEXT )


#define  STC_TEXT_SINGLE        (0x01)    //�����ı������ж���
#define  STC_TEXT_SINGLE_LEFT   (0x02)    //�����ı��������
#define  STC_TEXT_SINGLE_RIGHT  (0x03)    //�����ı����Ҷ���
#define  STC_TEXT_MULTIPLE      (0x04)    //�����ı�
#define  STC_BMP                (0x05)    //ֻ��ͼƬ(����Ϊ͸�������߲�͸��)
#define  STC_BMP_BATTERY        (0x06)    //���ͼ����ʾ
#define  STC_DIGITAL_TIME       (0x07)    //ʱ�䣬�����
#define  STC_DIGITAL_NUM        (0x08)    //1-90,��λ���������
#define  STC_DIGITAL_CH         (0x09)    //����ܣ�Ƶ��
#define  STC_DIGITAL_FM         (0x10)    //����ܣ�FM
#define  STC_MAP_ZONE           (0x11)    //������ʾ
#define  STC_SLIDER             (0x12)    //������
#define  STC_TEXT_SINGLE_ELIDE  (0x13)    //�����ı��������ؼ����ȱ��ضϣ��õ��ʾ
#define  STC_TEXT_SINGLE_ROLL   (0x14)    //���й����ı�


#define  SKIN_TRANS_COLOR      ( RGB(255,0,255) ) 
#define  SKIN_ID_NONE          (0xEFFFFFFF)

typedef struct _STC_DRAW_ITEM{
	DWORD     stSkinID;
	DWORD     stPntSkinID;
	int       nNumber;
	BYTE      nFontID;
	BYTE      nFontCol;
	BYTE      stcStyle;
	BYTE      nBkCol;
}STC_DRAW_ITEM;


typedef struct  _BTN_DRAW_ITEM{

	DWORD   dwSkinID;
	DWORD   dwStyle;
	union{
		struct { BYTE TxNorCol, TxtSelCol, TxtDisCol, FontID; } s_col;
		DWORD   dwFgCol;
	};
}BTN_DRAW_ITEM;

//////////////////////////////////////////////////////////////////////////

typedef struct  _DLG_DRAW_ITEM{
	DWORD   dwSkinID;
	DWORD   dwStyle;
	union{
		struct { unsigned short FontID, TitleCol; } s_Text;
		DWORD   dwTitle;
	};
	POINT   TitlePt;
}DLG_DRAW_ITEM;

//////////////////////////////////////////////////////////////////////////

typedef struct _SKINBMP_ST{

	HBITMAP    hBitmap;
	unsigned   nWidthPerCell;
	unsigned   nHeightPerCell;
	UINT       nReferenceCount;

}SKINBMP_ST, *PSKINBMP_ST;


//////////////////////////////////////////////////////////////////////////


//
// define the attributes for all skins.
//

//
// define all the fonts for skin support.
//

enum  skin_font_e{
	sf_default = 0,
	sf_12,
	sf_14,
	sf_15,
	sf_16,
	sf_18,
	sf_20,
	sf_24,
	sf_28,
	sf_32,
//////////////////////////////////////////////////////////////////////////

	SF_TITLE,
	SF_BUTTON_SMALL,
	SF_BUTTON_NORMAL,
	SF_BUTTON_BIG,

	SF_TITLE_CHINESE,
	SF_BUTTON_SMALL_CHINESE,
	SF_BUTTON_NORMAL_CHINESE,
	SF_BUTTON_BIG_CHINESE,
	
	sf_count
};


// define all colors for skin support.
enum  skin_color_e{
	sc_normal = 0,
	sc_select,
	sc_disable,
	sc_white,
	sc_black,
	sc_red,
	SC_LIST_SELECT_TEXT,
	SC_LIST_SELECT_BACK,
	SC_LIST_NORMAL_TEXT,
	SC_LIST_NORMAL_BACK,
	SC_MEDIA_MUSIC_IMAGE_RADIO_TIYILE,
	sc_aboutTitile,
	sc_purple,
	sc_about,
	sc_lightblue,
	sc_ZoneWords,
	sc_ZoneTransitionCol,
	sc_Time_Titile_normal,
	sc_Time_Titile_select,
	sc_Time_Titile_disable,
	sc_transparent,
	sc_count
};

const CString	c_normal	= _T("sc_normal");
const CString	c_select	= _T("sc_select");
const CString	c_disable	= _T("sc_disable");

const CString	c_Time_Titile_normal =	_T("sc_Time_Titile_normal");
const CString	c_Time_Titile_select =	_T("sc_Time_Titile_select");
const CString	c_Time_Titile_disable =	_T("sc_Time_Titile_disable");

const CString	C_LIST_SELECT_TEXT	= _T("SC_LIST_SELECT_TEXT");
const CString	C_LIST_SELECT_BACK	= _T("SC_LIST_SELECT_BACK");
const CString	C_LIST_NORMAL_TEXT	= _T("SC_LIST_NORMAL_TEXT");
const CString	C_LIST_NORMAL_BACK	= _T("SC_LIST_NORMAL_BACK");
const CString  C_MEDIA_MUSIC_IMAGE_RADIO_TIYILE = _T("SC_MEDIA_MUSIC_IMAGE_RADIO_TIYILE");
const CString c_aboutTitile   =	_T("sc_aboutTitile");
const CString c_purple		  =	_T("sc_purple");
const CString c_about		  =	_T("sc_about");
const CString c_lightblue	  =	_T("sc_lightblue");
const CString c_ZoneWords	  =	_T("sc_ZoneWords");
const CString c_ZoneTransitionCol = _T("sc_ZoneTransitionCol");

#endif //_SKINPUB_H