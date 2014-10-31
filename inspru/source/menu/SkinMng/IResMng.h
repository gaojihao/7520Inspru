
#ifndef   _IRESMNG_N
#define   _IRESMNG_N

#include <windows.h>
#include "SkinPub.h"

class  IResMng 
{
public:
	virtual  ~IResMng(){};
	
	virtual  PSKINBMP_ST       RequestBmp(unsigned nBmpResID, bool bBgk) = 0;
	virtual  void              ReleaseBmp(unsigned nBmpResID) = 0;
	virtual  int               CalBmpOffset(unsigned nBmpID) = 0;

	virtual  COLORREF          RequestCol(unsigned nColID) = 0;
	virtual  HFONT             RequestFont(unsigned nFontID) = 0;
	virtual  void              DestoryAllRes()=0;
protected:
	IResMng(){};

};


#endif