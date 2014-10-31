// CResString.cpp: implementation of the CResString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGDICommon.h"

CGDICommon CGDICommon::m_gdicommon;

CGDICommon::CGDICommon()
{
	m_bigbigfont.CreateFont(
		40,					    // nHeight
		16,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("ºÚÌå\0")); 
	
	m_bigfont.CreateFont(
		32,					    // nHeight
		12,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("ºÚÌå\0")); 
	
	m_smallfont.CreateFont(
		-18,					    // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("ºÚÌå\0"));             // lpszFacename
}

CGDICommon::~CGDICommon()
{
	m_bigbigfont.DeleteObject();
	m_bigfont.DeleteObject();
	m_smallfont.DeleteObject();
}

CGDICommon* CGDICommon::Instance()
{
	return &m_gdicommon;
}

CFont& CGDICommon::bigbigfont()
{
	return m_bigbigfont;
}

CFont& CGDICommon::bigfont()
{
	return m_bigfont;
}

CFont& CGDICommon::smallfont()
{
	return m_smallfont;
}

COLORREF CGDICommon::color()
{
	return RGB(198,203,210);
}

UINT CGDICommon::format()
{
	return DT_CENTER|DT_BOTTOM;
}