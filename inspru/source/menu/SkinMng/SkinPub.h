
#ifndef   _SKINPUB_H
#define   _SKINPUB_H

#include <windows.h>
//#include <WinSock2.h>
//
// The draw style.
//

//            背景控制    前景文字   前景图片
// 00000000---00000000---00000000---00000000
//
#define  KDS_TEXT_HLEFT         0x00000001	 //文字水平左对齐
#define  KDS_TEXT_HRIGHT        0x00000002	 //文字水平右对齐
#define  KDS_TEXT_HCENTER       0x00000004   //文字水平居中
#define  KDS_TEXT_VTOP          0x00000010   //文字垂直上对齐
#define  KDS_TEXT_VBOTTOM       0x00000020   //文字垂直下对齐 
#define  KDS_TEXT_VCENTER       0x00000040   //文字垂直居中对齐
#define  KDS_TEXT_USERDEF       0x00000080   //文字在指定位置画（在对话框中）

#define  KDS_ICON_HEFT          0x00000001	 //图标水平左对齐
#define  KDS_ICON_HRIGHT        0x00000002	 //图标水平右对齐
#define  KDS_ICON_HCENTER       0x00000004   //图标水平居中
#define  KDS_ICON_VTOP          0x00000010   //图标垂直上对齐
#define  KDS_ICON_VBOTTOM       0x00000020   //图标垂直下对齐 
#define  KDS_ICON_VCENTER       0x00000040   //图标垂直居中对齐

#define  KDS_CTRL_BKGBMP        0x00010000   //背景是图标
#define  KDS_CTRL_BKGRGB        0x00020000	 //背景是颜色
#define  KDS_CTRL_FKTEXT        0x00040000	 //前景有文字
#define  KDS_CTRL_FKICON        0x00080000	 //前景有图标

#define  KDS_AUTOSIZE           0x01000000   //控件自动大小为图片大小
#define  KDS_TRANSPARENT        0x02000000   //控件背景透明


#define  KDS_BTN_AUTOBMP       ( KDS_CTRL_BKGBMP | KDS_AUTOSIZE)
#define  KDS_BTN_DFTTEXT       ( KDS_TEXT_VBOTTOM | KDS_TEXT_HCENTER | KDS_CTRL_FKTEXT )


#define  STC_TEXT_SINGLE        (0x01)    //单行文本，居中对齐
#define  STC_TEXT_SINGLE_LEFT   (0x02)    //单行文本，左对齐
#define  STC_TEXT_SINGLE_RIGHT  (0x03)    //单行文本，右对齐
#define  STC_TEXT_MULTIPLE      (0x04)    //多行文本
#define  STC_BMP                (0x05)    //只有图片(可以为透明，或者不透明)
#define  STC_BMP_BATTERY        (0x06)    //电池图标显示
#define  STC_DIGITAL_TIME       (0x07)    //时间，数码管
#define  STC_DIGITAL_NUM        (0x08)    //1-90,两位数的数码管
#define  STC_DIGITAL_CH         (0x09)    //数码管，频道
#define  STC_DIGITAL_FM         (0x10)    //数码管，FM
#define  STC_MAP_ZONE           (0x11)    //区域显示
#define  STC_SLIDER             (0x12)    //滑动条
#define  STC_TEXT_SINGLE_ELIDE  (0x13)    //单行文本，超过控件长度被截断，用点表示
#define  STC_TEXT_SINGLE_ROLL   (0x14)    //单行滚动文本


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