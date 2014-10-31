#include "stdafx.h"
#include "ResMng.h"
#include "CSkinResDef.h"

CResMng*  CResMng::g_ResInstance = NULL;

CResMng::CResMng()
{
	ZeroMemory(&_sfRes[0], sizeof(HFONT) * sf_count );
	DestoryAllRes();
	_tcscpy(_szFontName, _T("����\0"));
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
	ASSERT(col != RGB(1,2,3));//��������﷢��ASSERT,��������ȡһ��skin.ini�ļ�,лл!

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
		ASSERT(FALSE);//Ӧ�ò�����������������
		hFont = NULL;
		break;
	}
	
	return hFont;
}

/*

typedef struct tagLOGFONT { 
  LONG lfHeight; 
	���߼���λָ��������ַ���Ԫ���ַ��ĸ߶�.���ַ��߶�ֵ���ַ���Ԫ
	�߶�ֵ��ȥ���ֵ�Ĳ�.����ӳ��Ѵ�ֵ�������¹�����н���:
		>0 ����ӳ��任��ֵ���豸��λ����ƥ����Ч����ĵ�Ԫ�߶�.
		=0 ����ӳ���ڲ���һ��ƥ��ʱʹ��ȱʡ�ĸ߶�ֵ.
		<0 ����ӳ��任��ֵ���豸��λ���Ҹ�����Ч������ַ��߶���
		ƥ�����ľ���ֵ
	�������еĸ߶ȱȽ�,����ӳ����Ҳ�������Ҫ��Ĵ�С����������.
	���ӳ�䷢�������屻��һ��ʹ�õ�ʱ��.
	����MM_TEXTӳ��ģʽ,�����ʹ�����������ʽ����һ��������Ĵ�С
	Ϊһ������ָ��һ���߶�
	lfHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	(����ע.GetDeviceCaps(hDC, LOGPIXELSY)����Ŀ��DC��y������ÿ���߼���λ(Ӣ��)��������,
	�߶� = �˸߶��Ե��������ĳߴ� * ÿӢ��ĵ��� / 72.
  LONG lfWidth; 
	ָ��ƽ�����,��������ַ����߼���λ.�����ֵΪ0,��ô
	
  LONG lfEscapement; 
  ��1/10����ָ�������������豸x��ļн�.����������һ���ı��Ļ���ƽ��.
  LONG lfOrientation; 
		��1/10����ָ��ÿ���ַ��Ļ������豸x��ļн�
  LONG lfWeight; 
  ָ������İ�ֵ,��ΧΪ0-1000,����,400��������700���Ǵ���.
  �����ֵΪ0,��ôʹ��ȱʡ�İ�ֵ.
  BYTE lfItalic; 
	���Ҫʹ��б����ô��ֵҪ����ΪTRUE.
  BYTE lfUnderline; 
	���Ҫ���»�����ô��ֵҪ����ΪTRUE.
  BYTE lfStrikeOut; 
  ???????????
  BYTE lfCharSet; 
	ָ���ַ���,Ԥ�����������ַ���
	ANSI_CHARSET BALTIC_CHARSET 
CHINESEBIG5_CHARSET DEFAULT_CHARSET 
EASTEUROPE_CHARSET GB2312_CHARSET 
GREEK_CHARSET HANGUL_CHARSET 
MAC_CHARSET OEM_CHARSET 
RUSSIAN_CHARSET SHIFTJIS_CHARSET 
SYMBOL_CHARSET TURKISH_CHARSET 

OEM_CHARSET��ָ�����ַ��������ϵͳ���.
DEFAULT_CHARSET�ɱ��ػ��Ĳ���ϵͳ������,�������������Ӣ�İ����ϵͳ,��˺�
ʵ��ΪANSI_CHARSET.
�����ַ������ֿ�Ҳ�����Ѿ������ڲ���ϵͳ��.���һ��Ӧ�ó���ʹ����һ��
δ֪�ַ���������,��ô��Ӧ����ͼȥת�������Ҫ�ø�������Ⱦ���ַ���.
	�����Ա������ӳ������зǳ���Ҫ.Ҫȷ���õ�һ�µĽ��,ָ��һ���������
	����.�����ָ��һ�ִ�ӡ�����嵽�˳�Ա,ȷ����ֵ��ƥ����ַ�����lfFaceName��һ��.

  BYTE lfOutPrecision; 
  ָ���������,���뾫�ȶ��������
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
			_tcscpy(lgFont.lfFaceName, _T("����_GB2312\0"));
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
			//_tcscpy(lgFont.lfFaceName, _T("����\0"));
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
			_tcscpy(lgFont.lfFaceName, _T("����\0"));
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