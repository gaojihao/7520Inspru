
#ifndef  _RESMNG_H
#define  _RESMNG_H

#include "./IResMng.h"
#include "CSkinResDef.h"
#include "CSkinBitmap.h"

class  CResMng  : public IResMng
{
public:
	~CResMng();
static	CResMng*   Instance();

public:
	PSKINBMP_ST   RequestBmp(unsigned nBmpResID,bool bBgk);
	void          ReleaseBmp(unsigned nBmpResID);
	int           CalBmpOffset(unsigned nBmpID);

	COLORREF	  RequestCol(unsigned nColID);
	HFONT         RequestFont(unsigned nFontID);
	
	void          DestoryAllRes();
//////////////////////////////////////////////////////////////////////////

    virtual  BOOL          RequestUserDefinedCol( LPCTSTR strName,COLORREF& clrRequest );
//////////////////////////////////////////////////////////////////////////

protected:
	CResMng();
	void   ReleaseAllBmp();
	void   ReleaseAllFont();
	BOOL   LoadBmpFromFile(unsigned nFileID);
	HFONT  CreateFontFromSize(int nSize,int nWidth);
	HFONT	CreateSpecialFont(skin_font_e eFont);

private:
	static CResMng*   g_ResInstance;
	SKINBMP_ST         _sbRes[bmp_count];
	HFONT              _sfRes[sf_count];
	TCHAR              _szFontName[LF_FACESIZE];
}; 


#endif