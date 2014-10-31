#pragma once

class CMsgBase
{
public:
	CMsgBase(void);
	~CMsgBase(void);

protected:
	//计算校验
	UCHAR		CalcXor(UCHAR *pSrc, USHORT nLenSrc);
	//获取节字符串
	PCSTR		ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen, CHAR cSeparator = ',');
};
