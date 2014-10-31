/** Copyright (c) 2005 Unistrong, Co. Ltd.
 * @file    Serial.h
 * @brief   Class CUnisProtocol efination��
 * @version Ver1.00
 * @date    2005-11-21
 */

#pragma once

#ifndef _CDIRSREF_H
#define _CDIRSREF_H


#include "ffsco.h"

/**	Ŀ¼��Ϣ������ */

class CDirsRef : public CObject
{
public:
	virtual ~CDirsRef();
	CDirsRef();

	/**
	 * @brief    ���캯��
	 * @param  ParentDirName Ҫ��ѯ��Ŀ¼
	 * @param  recursion ȷ���Ƿ�ݹ��ѯ  �豸���
	 * @param  fext ���Ҫģ����ѯ,����Ҫģ����ѯ���ַ���,Ĭ�ϲ���Ŀ¼�������ļ�
	*/
	CDirsRef(CString ParentDirName,UINT recursion = 0,CString fext = _T(""));

	void Load(CString ParentDirName,UINT recursion = 0,CString fext = _T("")) ;

	void UnisSerialize(CArchive& ar);

	/**
	 * @brief    ��ȡ��ѯ�����ļ���
	 * @return  ��Ŀ
	 * @retval  ��ѯ�����ļ���Ŀ
	*/
	inline UINT ReadFileCnt() {return m_iFileCnt;};

	/**
	 * @brief    ��ȡ��ѯ����Ŀ¼��
	 * @return  ��Ŀ
	 * @retval  ��ѯ����Ŀ¼��Ŀ
	*/
	inline UINT ReadDirCnt() {return m_iDirCnt;}; 

	/**
	 * @brief    ��һ���ļ�����
	 * @param  pos λ��
	 * @return  �ַ���
	 * @retval  ���ļ�ȫ���ƽضϹ���·������ַ���
	*/
	CString ReadOneFileName(UINT pos) ;

	/**
	 * @brief    ��һ��Ŀ¼����
	 * @param  pos λ��
	 * @return  �ַ���
	 * @retval  ��Ŀ¼ȫ���ƽضϹ���·������ַ���
	*/
	CString ReadOneDirName(UINT pos) ;

	/**
	 * @brief    ȡĿ¼��
	 * @return  ����
	 * @retval  ��ѯ����Ŀ¼����
	*/
	int GetDirsCnt();	

	/**
	 * @brief    ȡ�ļ���
	 * @return  ����
	 * @retval  ��ѯ�����ļ�����
	*/
	int GetFilesCnt();

private:
	
	void GetFileAndDir(CString,UINT,CString fext = _T(""));

	//д��һ����Ŀ¼��
	void WriteDirs(CString DirsName);

	//д��һ�� �ļ����ļ�
	void WriteFiles(CString FileName);

private:
	UINT m_iDirCnt;
	CStringList m_vDirPath;
	UINT m_iFileCnt;
	CStringList m_vFilePath;

	DECLARE_SERIAL(CDirsRef)
};

#endif
