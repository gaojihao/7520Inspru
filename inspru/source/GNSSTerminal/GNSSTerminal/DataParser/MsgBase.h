#pragma once

#include "DataType.h"

class CMsgBase
{
public:
	CMsgBase(void);
	~CMsgBase(void);

public:
	//����У��
	UCHAR		CalcXor(UCHAR *pSrc, USHORT nLenSrc);
	//��ȡ���ַ���
	PCSTR		ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen, CHAR cSeparator = ',');
};
