#pragma once

class CMsgBase
{
public:
	CMsgBase(void);
	~CMsgBase(void);

protected:
	//����У��
	UCHAR		CalcXor(UCHAR *pSrc, USHORT nLenSrc);
	//��ȡ���ַ���
	PCSTR		ScanField( PCSTR pcBuffer, INT nFieldNO, WORD& wLen, CHAR cSeparator = ',');
};
