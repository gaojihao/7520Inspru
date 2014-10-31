// CSkinBitmap.cpp: implementation of the CSkinBitMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSkinBitmap.h"

#include "logFile.h"

#include <INITGUID.h> 
#include <imaging.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile CSkinBitMap::m_SkinIniFile;

CString CSkinBitMap::m_SkinPath = _T("\\NandFlashPartition\\menu\\Style");

CString CSkinBitMap::g_csSkin1 = _T("\\Skin1.ini");
CString CSkinBitMap::g_csSkin2 = _T("\\Skin2.ini");
CString CSkinBitMap::g_csSkin3 = _T("\\Skin3.ini");
CString CSkinBitMap::g_csSkin4 = _T("\\Skin4.ini");
CString CSkinBitMap::g_csSkin5 = _T("\\Skin5.ini");

enSkinType CSkinBitMap::g_CurSkinType = ID_SKIN1;

void CSkinBitMap::SetSkinRootPath(const CString& csSkinPath)
{
	m_SkinPath = csSkinPath;
}

CSkinBitMap::CSkinBitMap()
{
}

CSkinBitMap::~CSkinBitMap()
{
}

BOOL CSkinBitMap::LoadBitmap( const CString nNameID)
{
	CString csSection = _T("SkinBmp");

	CString csValue;
	
	if(m_SkinIniFile.GetString(csSection,nNameID,csValue))
	{
		csValue = m_SkinPath + csValue;
		return Attach(SHLoadDIBitmap(csValue));
	}
	else
		return FALSE;
}

HBITMAP CSkinBitMap::GetBmpHandle(const CString nNameID)
{
	CString csSection = _T("SkinBmp");

	CString csValue;
	HBITMAP hBitmap;
	
	m_SkinIniFile.GetString(csSection,nNameID,csValue);

	csValue = m_SkinPath + csValue;

	if(ID_SKIN1 == g_CurSkinType)
	{
		hBitmap = LoadImageFromFile(csValue.GetBuffer(csValue.GetLength()));
	}
	else if(ID_SKIN2 == g_CurSkinType)
	{
		hBitmap = SHLoadDIBitmap(csValue);
	}
	else
	{
		hBitmap = NULL;
	}
	
	return hBitmap;

}

ST_RGB CSkinBitMap::GetRGB(const CString nNameID)
{
	CString csSection = _T("RGB");

	CString csValue;
	
	m_SkinIniFile.GetString(csSection,nNameID,csValue);
	
	ST_RGB rgb;

	TCHAR* tok =  wcstok(csValue.GetBuffer(csValue.GetLength()), _T(","));
	rgb.red = _wtoi(tok);
	tok =  wcstok(NULL, _T(","));
	rgb.green = _wtoi(tok);
	tok =  wcstok(NULL, _T(","));
	rgb.blue = _wtoi(tok);
	return rgb;
}

BOOL CSkinBitMap::SetSkinType(enSkinType SkinType)
{
	BOOL bRts = FALSE;
	CString SkinName;

//	if(m_SkinIniFile.IsOpen())
//	{
//		m_SkinIniFile.Close();
//	}
	
	switch(SkinType)
	{
	case ID_SKIN1:
		{
			SkinName = m_SkinPath + g_csSkin1;
 			if(m_SkinIniFile.Load(SkinName))
			{
				g_CurSkinType = ID_SKIN1;
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}
	case ID_SKIN2:
		{
			SkinName = m_SkinPath + g_csSkin2;
 			if(m_SkinIniFile.Load(SkinName))
			{
				g_CurSkinType = ID_SKIN2;
				bRts = TRUE;
			}
			else
				bRts = FALSE;
			break;
		}	
// 	case ID_SKIN3:
// 		{
// 			SkinName = m_SkinPath + g_csSkin3;
//  			if(m_SkinIniFile.Load(SkinName))
// 			{
// 				g_CurSkinType = ID_SKIN3;	
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}	
// 	case ID_SKIN4:
// 		{
// 			SkinName = m_SkinPath + g_csSkin4;
//  			if(m_SkinIniFile.Load(SkinName))
// 			{
// 				g_CurSkinType = ID_SKIN4;		
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}	
// 	case ID_SKIN5:
// 		{
// 			SkinName = m_SkinPath + g_csSkin5;
//  			if(m_SkinIniFile.Load(SkinName))
// 			{
// 				g_CurSkinType = ID_SKIN5;
// 				bRts = TRUE;
// 			}
// 			else
// 				bRts = FALSE;
// 			break;
// 		}	
 	default:
		bRts = FALSE;
		break;
	}
	

	return bRts;
}

void CSkinBitMap::ReleaseSkin()
{
//	if(m_SkinIniFile.IsOpen())
//	{
//		m_SkinIniFile.Close();
// 	}
}

enSkinType CSkinBitMap::GetCurSkinType()
{
	return g_CurSkinType;
}

HBITMAP CSkinBitMap::LoadImageFromFile(TCHAR * pFileImage)
{
    IImagingFactory *pImgFactory = NULL;
    IImage *pImageBmp = NULL; 
    CoInitializeEx(NULL, COINIT_MULTITHREADED); 
    HBITMAP hResult = 0;
    
    if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory, NULL,
		CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void **)&pImgFactory)))
    {
		ImageInfo imageInfo;
		HRESULT hr = pImgFactory->CreateImageFromFile(pFileImage, &pImageBmp);
        if (SUCCEEDED(hr)&& SUCCEEDED(pImageBmp->GetImageInfo(&imageInfo)))
        {
			CWindowDC dc(0);
			CDC dcBitmap;

			dcBitmap.CreateCompatibleDC(&dc);
			hResult = ::CreateCompatibleBitmap(dc.GetSafeHdc(),imageInfo.Width, imageInfo.Height);
			HGDIOBJ hOldBitmap = SelectObject(dcBitmap, hResult);
			//内存不足的话，Draw操作可能会失败
			pImageBmp->Draw(dcBitmap, CRect(0, 0, imageInfo.Width, imageInfo.Height), NULL); 
			SelectObject(dcBitmap, hOldBitmap);
			DeleteDC(dcBitmap);
			pImageBmp->Release();
        }
        pImgFactory->Release();
    }
    CoUninitialize();
	
    return hResult;
}

/*
#include <imaging.h>
#include <initguid.h>
#include <imgguids.h>

#pragma comment(lib, "Imaging.lib")
*/
BOOL DrawPNG1234(HDC hdc, TCHAR *szPicString, RECT &rcDraw)
{
	BOOL br = FALSE;
	IImagingFactory *pImgFactory = NULL;
	IImage *pImage = NULL;
	//	ImageInfo sImgInfo;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//Create the imaging factory.
	if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, 
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IImagingFactory,
		(void **)&pImgFactory)))
	{
		//Load the image from the JPG file.
		if (SUCCEEDED(pImgFactory->CreateImageFromFile(szPicString, &pImage)))
		{
			pImage->Draw(hdc, &rcDraw, NULL);
			pImage->Release();
			pImage = NULL;
			br = TRUE;
		}

		pImgFactory->Release();
	}
	CoUninitialize();

	return br;
}