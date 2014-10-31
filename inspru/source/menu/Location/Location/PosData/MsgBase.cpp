#include "StdAfx.h"
#include "MsgBase.h"

CMsgBase::CMsgBase(void)
{
}

CMsgBase::~CMsgBase(void)
{
}


/************************************************************************
* 函数名:	CalcXor
* 描  述:	计算校验和
* 入  参:	1. pSrc:		协议消息字符串
			2. nLenSrc:	协议消息字符串的长度
* 出  参: 
* 返  回:	计算的校验和
* 备  注:	计算字符串pSrc每个字节的异或和，并返回
************************************************************************/
UCHAR CMsgBase::CalcXor(UCHAR *pSrc, USHORT nLenSrc)
{
	if (NULL == pSrc || 0 == nLenSrc)
	{
		return -1;
	}
	char chTemp = pSrc[0];
	for(int i=1; i<nLenSrc; i++)
	{
		chTemp ^= pSrc[i];
	}
	return chTemp;
}

/************************************************************************
* 函数名:	ScanField
* 描  述:	获取字符串中第一个cSeparator后面字符串的地址
* 入  参:	1. pcBuffer:	要解析的短信协议字符串，去掉'*','#'
*			2. nFieldNO:	解析的字符串的节(两个','之间的字符串)，0：当前节，1：下一节，2,3...
*			3. cSeparator:	节之间的间隔符
* 出  参:	1. wLen:		节的长度
* 返  回:	PCSTR:		指向节的字符串，如果为NULL，则获取节字符串失败
* 备  注: 
************************************************************************/
PCSTR CMsgBase::ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen, CHAR cSeparator/* = ','*/)
{
	if (NULL == pcBuffer)
	{
		return NULL;
	}
	PCSTR pcFieldBegin = pcBuffer;

	while( nFieldNO-- > 0 )
	{
		while ( pcFieldBegin && *pcFieldBegin++ != cSeparator && *pcFieldBegin != NULL )
			;
	}
	//搜索的节为空
	if(NULL == pcFieldBegin ||NULL == *pcFieldBegin)
	{
		wLen = 0;
		return NULL;
	}

	//节的最后一个字符指针的后一位
	PCSTR pcFieldEnd = pcFieldBegin;
	while( pcFieldEnd && *pcFieldEnd != cSeparator && *pcFieldEnd != NULL )
	{
		pcFieldEnd++;
	}

	//计算节的长度
	wLen = pcFieldEnd - pcFieldBegin;

	return pcFieldBegin;
}