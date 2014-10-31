#include "StdAfx.h"
#include "MsgBase.h"

CMsgBase::CMsgBase(void)
{
}

CMsgBase::~CMsgBase(void)
{
}


/************************************************************************
* ������:	CalcXor
* ��  ��:	����У���
* ��  ��:	1. pSrc:		Э����Ϣ�ַ���
			2. nLenSrc:	Э����Ϣ�ַ����ĳ���
* ��  ��: 
* ��  ��:	�����У���
* ��  ע:	�����ַ���pSrcÿ���ֽڵ����ͣ�������
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
* ������:	ScanField
* ��  ��:	��ȡ�ַ����е�һ��cSeparator�����ַ����ĵ�ַ
* ��  ��:	1. pcBuffer:	Ҫ�����Ķ���Э���ַ�����ȥ��'*','#'
*			2. nFieldNO:	�������ַ����Ľ�(����','֮����ַ���)��0����ǰ�ڣ�1����һ�ڣ�2,3...
*			3. cSeparator:	��֮��ļ����
* ��  ��:	1. wLen:		�ڵĳ���
* ��  ��:	PCSTR:		ָ��ڵ��ַ��������ΪNULL�����ȡ���ַ���ʧ��
* ��  ע: 
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
	//�����Ľ�Ϊ��
	if(NULL == pcFieldBegin ||NULL == *pcFieldBegin)
	{
		wLen = 0;
		return NULL;
	}

	//�ڵ����һ���ַ�ָ��ĺ�һλ
	PCSTR pcFieldEnd = pcFieldBegin;
	while( pcFieldEnd && *pcFieldEnd != cSeparator && *pcFieldEnd != NULL )
	{
		pcFieldEnd++;
	}

	//����ڵĳ���
	wLen = pcFieldEnd - pcFieldBegin;

	return pcFieldBegin;
}