// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile()
{
	m_sFileName = _T("");
	m_bFileExist = FALSE;
	m_bWrite = FALSE;
}

CIniFile::~CIniFile()
{
	if(m_bWrite)
		Flush();
	if(m_FileContainer.GetSize() > 0)
	{
		m_FileContainer.RemoveAll();
	}
}

BOOL CIniFile::Load(const CString csFileName, BOOL IsCreate)
{
	m_sFileName = csFileName;

	SetFileAttributes(m_sFileName, FILE_ATTRIBUTE_NORMAL);

	CFileException e;
	TRY
	{
		if(!IsCreate)
			m_bFileExist=m_IniFile.Open(m_sFileName, 
				CFile::modeReadWrite ,&e);
		else
			m_bFileExist=m_IniFile.Open(m_sFileName,
				CFile::modeCreate | CFile::modeReadWrite ,&e);
	}
	CATCH(CFileException,e)
	{
	//	m_IniFile.Close();
		m_bFileExist = FALSE;
		TRACE0("打开文件错误！");
		return FALSE;
	}
	END_CATCH

	if(m_bFileExist == 0)
		return FALSE;
	
	TRY
	{
		ReadIniFile();

		m_IniFile.Close();
	}
	CATCH( CException,e )
	{
		m_IniFile.Close();
		DeleteFile( csFileName );
		m_bFileExist = FALSE;
		TRACE0("打开文件错误！");
		return FALSE;
	}
	END_CATCH

	m_bFileExist = FALSE;	
	return TRUE;
}

BOOL CIniFile::IsOpen()
{
//	if(m_bFileExist)
//		return TRUE;
// 	else
		return FALSE;
}

void CIniFile::Close()
{
//	m_IniFile.Close();
// 	m_bFileExist = FALSE;
}

BOOL CIniFile::GetString(const CString& Section, const CString& Item, CString& Value)
{
	return GetFileString(Section, Item, Value);
}


	//取bool
BOOL CIniFile::GetBool(const CString& Section, const CString& Item, bool& Value)
{
	CString strtemp;
	int st =0;
	BOOL bRts = GetFileString(Section, Item, strtemp);

	if(EOF == swscanf(strtemp,_T("%d"),&st) && bRts)	
		return FALSE;
	
	if(st > 0)
		Value = true;
	else
		Value = false;
	return TRUE;
	
}

	//设置bool
BOOL CIniFile::SetBool(CString Section, CString Item, bool Value)
{
	m_bWrite = TRUE;

	CString strtemp;
	strtemp.Format(_T("%d"),(int)Value);
	//保存到m_FileContainer
	WriteFileString(Section, Item, strtemp);
	return TRUE;
}


BOOL CIniFile::GetInt(const CString& Section, const CString& Item, int& Value)
{
	CString strtemp;
	strtemp.Format(_T("%d"),Value);
	BOOL bRts = GetFileString(Section, Item, strtemp);
//	Value = _ttoi(strtemp);
	if(EOF != swscanf(strtemp,_T("%d"),&Value) && bRts)
		return TRUE;
	else
		return FALSE;
}

BOOL CIniFile::GetFloat(const CString& Section, const CString& Item, float& Value)
{
	CString strtemp;
	strtemp.Format(_T("%f"),Value);
	BOOL bRts = GetFileString(Section, Item, strtemp);	
	if(EOF != swscanf(strtemp,_T("%f"),&Value) && bRts)	
		return TRUE;
	else
		return FALSE;
}

BOOL CIniFile::ReadIniFile()
{
	m_iPos = 1;
	CString strline;
	
	if(m_FileContainer.GetSize() > 0)
	{
		m_FileContainer.RemoveAll();
	}
	
	m_IniFile.SeekToBegin();
	ULONG buflen = m_IniFile.GetLength() + 1;
	char* pFileBuf = new char[buflen];
	memset(pFileBuf,0,buflen);

	buflen = m_IniFile.Read(pFileBuf,buflen);


	buflen = buflen/2;

	BOOL bReadState = TRUE;
	while(bReadState)
	{
		TCHAR OutBuf[1024];
		TCHAR* pOutBuf = OutBuf;
		UINT OutLen = 0;
		bReadState = ReadOneString((TCHAR*)pFileBuf,buflen,pOutBuf,OutLen);
		if (bReadState && OutLen!=0)
		{
			pOutBuf[OutLen] = '\0';
			CString OneStr(pOutBuf);
			m_FileContainer.Add(OneStr);
		}
	}
	
	if(pFileBuf != NULL)
	{
		delete []pFileBuf;
		pFileBuf = NULL;
	}

	return TRUE;
}

BOOL CIniFile::ReadOneString(TCHAR* pIn, UINT iInLen, TCHAR*& pOut, UINT &iOutLen)
{

	if(m_iPos >= iInLen)
		return FALSE;

	while(pIn[m_iPos] == 0x000d && pIn[m_iPos+1] == 0x000a)
		m_iPos += 2;

	iOutLen = 0;
	while(m_iPos < iInLen)
	{
		if(pIn[m_iPos] != 0x000d)
		{
			pOut[iOutLen] = pIn[m_iPos];
			iOutLen++;
			m_iPos++;
		}
		else
		{
			m_iPos = m_iPos+2;
			break;
		}
	}
	return TRUE;
}

BOOL CIniFile::GetFileString(const CString& Section,const CString& Item, CString& Value)
{
	if(m_FileContainer.GetSize() <= 0)
		return FALSE;//文件为空，返错误

	int i = 0;
	int iFileLines = m_FileContainer.GetSize();
	CString strline,str;

	while(i<iFileLines)
	{
		strline = m_FileContainer.GetAt(i++);

		strline.TrimLeft();
		if(strline.GetAt(0)=='[')//查找Section，第一个必须为[
		{
			str=strline.Left(strline.Find(_T("]")));//去掉]右边
			str=str.Right(str.GetLength()-str.Find(_T("["))-1);//去掉[左边

			str.TrimLeft();
			str.TrimRight();

			if(Section == str)//找到Section
			{
				while(i<iFileLines)
				{
					strline = m_FileContainer.GetAt(i++);
					strline.TrimLeft();

					if(strline.GetAt(0)=='[')
						return FALSE;//如果到达下一个[]，即找不到,返回错误值
					
					str = strline.Left(strline.Find(_T("=")));//去掉=右边

					str.TrimLeft();
					str.TrimRight();

					if(Item == str)//找到Item
					{
						str=strline.Right(strline.GetLength()-strline.Find(_T("="))-1);//去掉=左边

						str.TrimLeft();
						str.TrimRight();

						Value = str;
						return TRUE;
					}
				}
				return FALSE;//找不到,返回默认值
			}

		}

	}
	return FALSE;//找不到,返回默认值
}

BOOL CIniFile::WriteIniFile()
{
	if(IsOpen())
		m_IniFile.Close();

	SetFileAttributes(m_sFileName, FILE_ATTRIBUTE_NORMAL);
	
	CFileException e;
	TRY
	{
		m_bFileExist=m_IniFile.Open(m_sFileName,
			CFile::modeCreate | CFile::modeReadWrite,&e);
		if(!m_bFileExist)
			return FALSE;

		char UniHead[2];
		UniHead[0]	= (char)0xff;
		UniHead[1]	= (char)0xfe;
		m_IniFile.Write(&UniHead,2);

		CString strIni;
		//将m_FileContainer写到m_IniFile文件
		for(int i = 0; i< m_FileContainer.GetSize(); i++)
		{
			CString strline = m_FileContainer[i];
			strIni += strline + _T("\r\n");

	// 		UINT nSize = strline.GetLength();
	//		char* pNew=NULL;
	//		pNew=new char[nSize];
	// 		memset(pNew,0,nSize);
			//<把CString数据从Unicode转换到Ansi
	//		WideCharToMultiByte(  //转换Unicode到Ansi
	//			CP_ACP,
	//			WC_COMPOSITECHECK | WC_DEFAULTCHAR,
	//			strline.GetBuffer(strline.GetLength()),
	//			strline.GetLength(),
	//			pNew,  //转换到缓冲区中
	//			nSize,  //最多字节数
	//			0,
	//			0
	// 			);

	//		nSize = nSize * 2;
	//		m_IniFile.Write((char*)strline.GetBuffer(strline.GetLength()),nSize);

	//		if(pNew != NULL)
	//		{
	//			delete pNew;
	//			pNew = NULL;
	//  	}
 		}
		UINT nSize = strIni.GetLength() * 2;
		m_IniFile.Write((char*)strIni.GetBuffer(strIni.GetLength()),nSize);
		
	}
	CATCH(CFileException , e)
	{
		m_IniFile.Close();
		return FALSE;
	}
	END_CATCH


	m_IniFile.Close();
	m_bFileExist = FALSE;

	return TRUE;
}

BOOL CIniFile::SetInt(CString Section, CString Item, int Value)
{
	m_bWrite = TRUE;
	CString strtemp;
	strtemp.Format(_T("%d"),Value);
	//保存到m_FileContainer
	WriteFileString(Section, Item, strtemp);
	return TRUE;
}

BOOL CIniFile::SetString(CString Section, CString Item, CString Value)
{
	m_bWrite = TRUE;
	//保存到m_FileContainer
	WriteFileString(Section, Item, Value);
	return TRUE;
}

BOOL CIniFile::SetFloat(CString Section, CString Item, float Value)
{
	m_bWrite = TRUE;
	CString strtemp;
	strtemp.Format(_T("%f"),Value);
	//保存到m_FileContainer
	WriteFileString(Section, Item, strtemp);
	return TRUE;
}

void CIniFile::WriteFileString(CString Section, CString Item, CString Value)
{
	int i = 0;
	int iFileLines = m_FileContainer.GetSize();
	CString strline,str;

	while(i<iFileLines)
	{
		strline = m_FileContainer.GetAt(i++);

		strline.TrimLeft();
		if(strline.GetAt(0)=='[')//查找Section，第一个必须为[
		{
			str=strline.Left(strline.Find(_T("]")));//去掉]右边
			str=str.Right(str.GetLength()-str.Find(_T("["))-1);//去掉[左边

			str.TrimLeft();
			str.TrimRight();

			if(Section == str)//找到Section
			{
				while(i<iFileLines)
				{
					strline = m_FileContainer.GetAt(i++);
					strline.TrimLeft();

					if(strline.GetAt(0)=='[')//如果到达下一个[]，即找不到Item
					{
						break;
					}
					
					str = strline.Left(strline.Find(_T("=")));//去掉=右边

					str.TrimLeft();
					str.TrimRight();

					if(Item == str)//找到Item
					{
						strline = Item + _T("=");
						strline += Value;
						m_FileContainer[i-1] = strline;
						return;
					}
				}
				//找不到Item
				strline = Item + _T("=");
				strline += Value;
				m_FileContainer.InsertAt(i-1, strline);
				return;

			}

		}

	}
	//找不到Section
	//直接在最后加入Section，Item，Value
	//str = _T("[") + Section + _T("]");
	m_FileContainer.Add(_T("[") + Section + _T("]"));
	m_FileContainer.Add(Item + _T("=") + Value);
	return;
}

BOOL CIniFile::Flush()
{
	//保存到ini文件
	return WriteIniFile();
}

void CIniFile::Release()
{
	if(m_FileContainer.GetSize() > 0)
	{
		m_FileContainer.RemoveAll();
	}
}