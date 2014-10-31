// E920EventProtocol.h: interface for the E920EventProtocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_E920EVENTPROTOCOL_H__90E786B2_2C3C_45A2_9AC7_A7C2A6D15AE6__INCLUDED_)
#define AFX_E920EVENTPROTOCOL_H__90E786B2_2C3C_45A2_9AC7_A7C2A6D15AE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSerial.h"
#include "CE920Event.h"


class CE920EventProtocol
{
public:
	CE920EventProtocol();
	virtual ~CE920EventProtocol();

	//解码
	virtual BOOL Decode(char* lpInBuf,UINT iInBufLen, CE920Event*& lpOutData);

	//编码
	virtual BOOL Encode(CE920Event* lpInData,char* lpOutBuf,UINT& iBufLen);

private:

	//缓冲区读出一桢
	BOOL ReadOneFrame(char* lpInBuf,UINT iInBufLen,char lpOutBuf[],UINT& iOutLen);
};

#endif // !defined(AFX_E920EVENTPROTOCOL_H__90E786B2_2C3C_45A2_9AC7_A7C2A6D15AE6__INCLUDED_)
