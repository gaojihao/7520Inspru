// CE920EventProtocol.cpp: implementation of the CE920EventProtocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CE920EventProtocol.h"
#include "CE920Event.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CE920EventProtocol::CE920EventProtocol()
{

}

CE920EventProtocol::~CE920EventProtocol()
{

}

BOOL CE920EventProtocol::Decode(char* lpInBuf,UINT iInBufLen,CE920Event*& lpOutData)
{ 
	char FrameBuf[E920BufSize];
	memset(FrameBuf,0,E920BufSize);


	UINT iOneFrameLen = 0;
	
	if(ReadOneFrame(lpInBuf,iInBufLen,FrameBuf,iOneFrameLen))
	{
		//����lpOutData����δʵ��
		lpOutData->bCmd = FrameBuf[2];
		UINT iDataLen = FrameBuf[3];
		for(UINT i=0; i<iDataLen; i++)
		{
			lpOutData->bpData[i] = FrameBuf[4+i];
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL CE920EventProtocol::Encode(CE920Event* lpInData,char* lpOutBuf,UINT& iBufLen)
{
	iBufLen = 0;
	lpOutBuf[iBufLen] = DLE;
	iBufLen++;
	lpOutBuf[iBufLen] = char(lpInData->bDataLen + 2);
	iBufLen++;
	lpOutBuf[iBufLen] = lpInData->bCmd;
	iBufLen++;
	lpOutBuf[iBufLen] = lpInData->bDataLen;
	iBufLen++;
	for(UINT i=0; i<UINT(lpInData->bDataLen); i++)
	{
		lpOutBuf[iBufLen] = lpInData->bpData[i];
		iBufLen++;
	}
	lpOutBuf[iBufLen] = ETX;
	iBufLen++;
	return TRUE;
}

BOOL CE920EventProtocol::ReadOneFrame(char* lpInBuf,UINT iInBufLen,char lpOutBuf[],UINT& iOutLen)
{
	char* pDataHead = (char*)lpInBuf;

	//�ж���ͷ�Ƿ���ȷ
	if(pDataHead[0] != DLE)
		return FALSE;
	
	//��һ��
	for(UINT i=0; i<iInBufLen;i++)
	{
		if(pDataHead[i] != ETX)
		{
			lpOutBuf[i] = pDataHead[i];
			iOutLen++;
		}
		else
		{
			//�ѵ�����
			lpOutBuf[i] = pDataHead[i];
			iOutLen++;
			return TRUE;
		}
	}
	//δ�ѵ���β����
	return FALSE;
}
