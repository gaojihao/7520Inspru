#include "stdafx.h"
#include "ResMng.h"
#include "CSkinResDef.h"

CResMng*  CResMng::g_ResInstance = NULL;

CResMng::CResMng()
{
	ZeroMemory(&_sfRes[0], sizeof(HFONT) * sf_count );
	DestoryAllRes();
	_tcscpy(_szFontName, _T("宋体\0"));
}

CResMng::~CResMng()
{
	DestoryAllRes();
}

CResMng*   CResMng::Instance()
{
	if ( g_ResInstance == NULL )
		g_ResInstance = new CResMng();

	return g_ResInstance;
}

//
// This is a low function, don't check the id.
// 
int   CResMng::CalBmpOffset(unsigned nBmpResID)
{
	return   ( nBmpResID - BMP_ID_STEP * (nBmpResID / BMP_ID_STEP )  -1 );
}


COLORREF   CResMng::RequestCol(unsigned nColID)
{
	COLORREF col = RGB(0,0,0);
	switch( nColID ) {
	case sc_normal:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_normal);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_select:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_select);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_disable:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_disable);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_white: col = RGB(255,255,255);
		break;
	case sc_black: col = RGB(0,0,0);
		break;
	case sc_red: col = RGB(255,0,0);
		break;
	case	SC_LIST_SELECT_TEXT:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(C_LIST_SELECT_TEXT);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case	SC_LIST_SELECT_BACK:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(C_LIST_SELECT_BACK);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case	SC_LIST_NORMAL_TEXT:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(C_LIST_NORMAL_TEXT);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case	SC_LIST_NORMAL_BACK:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(C_LIST_NORMAL_BACK);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case SC_MEDIA_MUSIC_IMAGE_RADIO_TIYILE:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(C_MEDIA_MUSIC_IMAGE_RADIO_TIYILE);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_aboutTitile :
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_aboutTitile);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_purple :
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_purple);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_about:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_about);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_lightblue:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_lightblue);
			col = RGB(rgb.red,rgb.green,rgb.blue);	
			break;
		}
	case sc_ZoneWords :
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_ZoneWords);
			col = RGB(rgb.red,rgb.green,rgb.blue);	
			break;
		}
	case sc_ZoneTransitionCol:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_ZoneTransitionCol);
			col = RGB(rgb.red,rgb.green,rgb.blue);	
			break;
		}
	case sc_Time_Titile_normal:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_Time_Titile_normal);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_Time_Titile_select:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_Time_Titile_select);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_Time_Titile_disable:
		{
			ST_RGB rgb = CSkinBitMap::GetRGB(c_Time_Titile_disable);
			col = RGB(rgb.red,rgb.green,rgb.blue);
			break;
		}
	case sc_transparent: col = RGB(255,0,255);
		break;
	default:
		col = RGB(0,0,0);
		break;
	}
	return col;
}


BOOL   CResMng::RequestUserDefinedCol(LPCTSTR strName,COLORREF& clrRequest)
{
	if(!strName)
	{
		return FALSE;
	}
	
	CString strClrName(strName);
	COLORREF col = RGB(1,2,3);
	ST_RGB st_rgb = CSkinBitMap::GetRGB( strClrName );
	col =  RGB( st_rgb.red , st_rgb.green, st_rgb.blue );
	ASSERT(col != RGB(1,2,3));//如果在这里发生ASSERT,请您重新取一下skin.ini文件,谢谢!

	clrRequest = col;
	return TRUE;
}


HFONT  CResMng::RequestFont(unsigned nFontID)
{
	HFONT hFont = NULL;

	switch( nFontID )
	{
	case sf_default:
		if ( _sfRes[sf_default] == NULL )
			_sfRes[sf_default] = CreateFontFromSize(10,6);
		hFont = _sfRes[sf_default];
		break;
	case sf_12:
		if ( _sfRes[sf_12] == NULL )
			_sfRes[sf_12] = CreateFontFromSize(12,6);
		hFont = _sfRes[sf_12];
		break;
	case sf_14:
		if ( _sfRes[sf_14] == NULL )
			_sfRes[sf_14] = CreateFontFromSize(14,6);
		hFont = _sfRes[sf_14] ;
		break;
	case sf_15:
		if ( _sfRes[sf_15] == NULL )
			_sfRes[sf_15] = CreateFontFromSize(15,6);
		hFont = _sfRes[sf_15] ;
		break;
	case sf_16:
		if ( _sfRes[sf_16] == NULL )
			_sfRes[sf_16] = CreateFontFromSize(16,6);
		hFont = _sfRes[sf_16];
		break;
	case sf_18:
		if ( _sfRes[sf_18] == NULL )
			_sfRes[sf_18] = CreateFontFromSize(18,6);
		hFont = _sfRes[sf_18] ;
		break;
	case sf_20:
		if ( _sfRes[sf_20] == NULL )
			_sfRes[sf_20] = CreateFontFromSize(20,7);
		hFont = _sfRes[sf_20] ;
		break;
	case sf_24:
		if ( _sfRes[sf_24] == NULL )
			_sfRes[sf_24] = CreateFontFromSize(24,8);
		hFont = _sfRes[sf_24] ;
		break;
	case sf_28:
		if ( _sfRes[sf_28] == NULL )
			_sfRes[sf_28] = CreateFontFromSize(28,9);
		hFont = _sfRes[sf_28] ;
		break;
	case sf_32:
		if ( _sfRes[sf_32] == NULL )
			_sfRes[sf_32] = CreateFontFromSize(32,10);
		hFont = _sfRes[sf_32] ;		
		break;
	case SF_BUTTON_BIG:
	case SF_BUTTON_NORMAL:
	case SF_BUTTON_SMALL:
	case SF_BUTTON_BIG_CHINESE:
	case SF_BUTTON_NORMAL_CHINESE:
	case SF_BUTTON_SMALL_CHINESE:
		if ( _sfRes[nFontID] == NULL )
		{
			_sfRes[nFontID] = CreateSpecialFont(skin_font_e(nFontID));
			ASSERT(_sfRes[nFontID]);
		}
		hFont = _sfRes[nFontID] ;
		break;
	default:
		ASSERT(FALSE);//应用不可以请求其它字体
		hFont = NULL;
		break;
	}
	
	return hFont;
}

/*

typedef struct tagLOGFONT { 
  LONG lfHeight; 
	以逻辑单位指定字体的字符单元或字符的高度.此字符高度值是字符单元
	高度值减去间距值的差.字体映射把此值根据如下规则进行解释:
		>0 字体映射变换此值到设备单位并且匹配有效字体的单元高度.
		=0 字体映射在查找一个匹配时使用缺省的高度值.
		<0 字体映射变换此值到设备单位并且根据有效字体的字符高度来
		匹配它的绝对值
	对于所有的高度比较,字体映射查找不超过所要求的大小的最大的字体.
	这个映射发生在字体被第一次使用的时候.
	对于MM_TEXT映射模式,你可以使用下面这个公式来以一个给定点的大小
	为一种字体指定一个高度
	lfHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	(译者注.GetDeviceCaps(hDC, LOGPIXELSY)返回目标DC在y方向上每个逻辑单位(英寸)的像素数,
	高度 = 此高度以点来描述的尺寸 * 每英寸的点数 / 72.
  LONG lfWidth; 
	指定平均宽度,以字体的字符的逻辑单位.如果此值为0,那么
	
  LONG lfEscapement; 
  以1/10度来指定出口向量与设备x轴的夹角.出口向量与一行文本的基线平行.
  LONG lfOrientation; 
		以1/10度来指定每个字符的基线与设备x轴的夹角
  LONG lfWeight; 
  指定字体的磅值,范围为0-1000,例如,400是正常而700就是粗体.
  如果此值为0,那么使用缺省的磅值.
  BYTE lfItalic; 
	如果要使用斜体那么此值要设置为TRUE.
  BYTE lfUnderline; 
	如果要加下划线那么此值要设置为TRUE.
  BYTE lfStrikeOut; 
  ???????????
  BYTE lfCharSet; 
	指定字符集,预定义了以下字符集
	ANSI_CHARSET BALTIC_CHARSET 
CHINESEBIG5_CHARSET DEFAULT_CHARSET 
EASTEUROPE_CHARSET GB2312_CHARSET 
GREEK_CHARSET HANGUL_CHARSET 
MAC_CHARSET OEM_CHARSET 
RUSSIAN_CHARSET SHIFTJIS_CHARSET 
SYMBOL_CHARSET TURKISH_CHARSET 

OEM_CHARSET所指定的字符集与操作系统相关.
DEFAULT_CHARSET由本地化的操作系统所决定,例如如果是美国英文版操作系统,则此宏
实际为ANSI_CHARSET.
其它字符集的字库也可能已经存在于操作系统中.如果一个应用程序使用了一个
未知字符集的字体,那么它应该试图去转换或解释要用该字体渲染的字符串.
	这个成员在字体映射过程中非常重要.要确保得到一致的结果,指定一种特殊的字
	符集.如果你指定一种打印机字体到此成员,确保该值所匹配的字符集与lfFaceName相一致.

  BYTE lfOutPrecision; 
  指定输出精度,输入精度定义了如果
  BYTE lfClipPrecision; 
  BYTE lfQuality; 
  BYTE lfPitchAndFamily; 
  TCHAR lfFaceName[LF_FACESIZE]; 
} LOGFONT; 

*/
HFONT   CResMng::CreateFontFromSize(int nSize,int nWidth)
{
	LOGFONT lgFont;
	lgFont.lfHeight = nSize; 
	lgFont.lfWidth = nWidth;
	lgFont.lfEscapement = 0;
	lgFont.lfOrientation = 0;
	lgFont.lfWeight = FW_SEMIBOLD ;
	

	lgFont.lfItalic = FALSE;
	lgFont.lfUnderline = FALSE;
	lgFont.lfStrikeOut = 0;
	lgFont.lfCharSet = ANSI_CHARSET;
	lgFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lgFont.lfQuality = DEFAULT_QUALITY;
	lgFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	_tcscpy(lgFont.lfFaceName, &_szFontName[0]);

	return CreateFontIndirect(&lgFont);
}

HFONT   CResMng::CreateSpecialFont(skin_font_e eFont)
{
	LOGFONT lgFont;
	switch(eFont)
	{
	case SF_BUTTON_BIG:
		{
			lgFont.lfHeight = 36; 
			lgFont.lfWidth = 12;
			lgFont.lfEscapement = 0;
			lgFont.lfOrientation = 0;
			lgFont.lfWeight = FW_NORMAL ;
			lgFont.lfItalic = FALSE;
			lgFont.lfUnderline = FALSE;
			lgFont.lfStrikeOut = 0;
			lgFont.lfCharSet = ANSI_CHARSET;
			lgFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			lgFont.lfQuality = DEFAULT_QUALITY;
			lgFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
			_tcscpy(lgFont.lfFaceName, _T("Tahoma\0"));
		}
		break;
	case SF_BUTTON_NORMAL:
		{
			lgFont.lfHeight = 18; 
			lgFont.lfWidth = 8;
			lgFont.lfEscapement = 0;
			lgFont.lfOrientation = 0;
			lgFont.lfWeight = FW_NORMAL ;
			lgFont.lfItalic = FALSE;
			lgFont.lfUnderline = FALSE;
			lgFont.lfStrikeOut = 0;
			lgFont.lfCharSet = ANSI_CHARSET;
			lgFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			lgFont.lfQuality = DEFAULT_QUALITY;
			lgFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
			_tcscpy(lgFont.lfFaceName, _T("Tahoma\0"));
		} 
		break;
	case SF_BUTTON_SMALL:
		{
			lgFont.lfHeight = 16; 
			lgFont.lfWidth = 6;
			lgFont.lfEscapement = 0;
			lgFont.lfOrientation = 0;
			lgFont.lfWeight = FW_NORMAL ;
			lgFont.lfItalic = FALSE;
			lgFont.lfUnderline = FALSE;
			lgFont.lfStrikeOut = 0;
			lgFont.lfCharSet = ANSI_CHARSET;
			lgFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			lgFont.lfQuality = DEFAULT_QUALITY;
			lgFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
			_tcscpy(lgFont.lfFaceName, _T("Tahoma\0"));
		}
		break;
	case SF_BUTTON_BIG_CHINESE:
		{
			lgFont.lfHeight = 32; 
			lgFont.lfWidth = 13;
			lgFont.lfEscapement = 0;
			lgFont.lfOrientation = 0;
			lgFont.lfWeight = FW_SEMIBOLD ;
			lgFont.lfItalic = FALSE;
			lgFont.lfUnderline = FALSE;
			lgFont.lfStrikeOut = 0;
			lgFont.lfCharSet = ANSI_CHARSET;
			lgFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			lgFont.lfQuality = DEFAULT_QUALITY;
			lgFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
			_tcscpy(lgFont.lfFaceName, _T("仿宋_GB2312\0"));
			//_tcscpy(lgFont.lfFaceName, _T("Verdana"));
		}
		break;
	case SF_BUTTON_NORMAL_CHINESE:
		{
			lgFont.lfHeight = 23; 
			lgFont.lfWidth = 8;
			lgFont.lfEscapement = 0;
			lgFont.lfOrientation = 0;
			lgFont.lfWeight = FW_SEMIBOLD ;
			lgFont.lfItalic = FALSE;
			lgFont.lfUnderline = FALSE;
			lgFont.lfStrikeOut = 0;
			lgFont.lfCharSet = ANSI_CHARSET;
			lgFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			lgFont.lfQuality = DEFAULT_QUALITY;
			lgFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
			//_tcscpy(lgFont.lfFaceName, _T("宋体\0"));
			_tcscpy(lgFont.lfFaceName, _T("Verdana"));
			
		} 
		break;
	case SF_BUTTON_SMALL_CHINESE:
		{
			lgFont.lfHeight = 20; 
			lgFont.lfWidth = 7;
			lgFont.lfEscapement = 0;
			lgFont.lfOrientation = 0;
			lgFont.lfWeight = FW_NORMAL ;
			lgFont.lfItalic = FALSE;
			lgFont.lfUnderline = FALSE;
			lgFont.lfStrikeOut = 0;
			lgFont.lfCharSet = ANSI_CHARSET;
			lgFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			lgFont.lfQuality = DEFAULT_QUALITY;
			lgFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
			_tcscpy(lgFont.lfFaceName, _T("宋体\0"));
		}
		break;
	default:
		break;
	}


	return CreateFontIndirect(&lgFont);
}//////////////////////////////////////////////////////////////////////////
		
PSKINBMP_ST  CResMng::RequestBmp(unsigned nBmpResID,bool bBgk)
{
	PSKINBMP_ST pSkin = NULL;

	unsigned nBmpFile = nBmpResID;

	if ( !bBgk )
		nBmpFile = nBmpResID / BMP_ID_STEP;

	if ( nBmpFile >= bmp_count )
		return NULL;

//	if ( _sbRes[nBmpFile].hBitmap == NULL  )
//	{
		if ( !LoadBmpFromFile( nBmpFile ) )
			return NULL;
//	}

	pSkin = &_sbRes[nBmpFile];

	return pSkin;		

}

//////////////////////////////////////////////////////////////////////////

BOOL  CResMng::LoadBmpFromFile(unsigned nFileID)
{
//	if ( _sbRes[nFileID].hBitmap == NULL )
//	{
		_sbRes[nFileID].nWidthPerCell  = g_stBmpInfo[nFileID].nBmpSize.x;
		_sbRes[nFileID].nHeightPerCell = g_stBmpInfo[nFileID].nBmpSize.y;
		_sbRes[nFileID].hBitmap = CSkinBitMap::GetBmpHandle(g_stBmpInfo[nFileID].szBmpPath);  
//	}

	return ( _sbRes[nFileID].hBitmap != NULL );

}


void  CResMng::DestoryAllRes()
{
	ReleaseAllBmp();
	ReleaseAllFont();
}

void CResMng::ReleaseAllBmp ()
{
	for(int i = 0; i< bmp_count;i++)
	{
		if ( _sbRes[i].hBitmap != NULL )
			DeleteObject( _sbRes[i].hBitmap );

		_sbRes[i].hBitmap = NULL;
		_sbRes[i].nHeightPerCell =	0;
		_sbRes[i].nWidthPerCell  =	0;
		_sbRes[i].nReferenceCount = 0;
	}
}

void  CResMng::ReleaseAllFont()
{
	for(int i= 0;i < sf_count ;i++)
	{
		if ( _sfRes[i] != NULL )
			DeleteObject(_sfRes[i] );
		_sfRes[i] = NULL;
	}
}

void  CResMng::ReleaseBmp(unsigned nBmpResID)
{
	unsigned nBmpFile = nBmpResID;// / BMP_ID_STEP;

	if ( nBmpFile >= bmp_count )
		return ;

	if ( _sbRes[nBmpFile].hBitmap != NULL  )
	{
		DeleteObject(_sbRes[nBmpFile].hBitmap);
		_sbRes[nBmpFile].hBitmap = NULL;
		_sbRes[nBmpFile].nHeightPerCell = 
		_sbRes[nBmpFile].nWidthPerCell  = 
		_sbRes[nBmpFile].nReferenceCount = 0;
	}

	return;
}