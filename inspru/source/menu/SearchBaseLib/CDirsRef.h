/** Copyright (c) 2005 Unistrong, Co. Ltd.
 * @file    Serial.h
 * @brief   Class CUnisProtocol efination。
 * @version Ver1.00
 * @date    2005-11-21
 */

#pragma once

#ifndef _CDIRSREF_H
#define _CDIRSREF_H


#include "ffsco.h"

/**	目录信息引用类 */

class CDirsRef : public CObject
{
public:
	virtual ~CDirsRef();
	CDirsRef();

	/**
	 * @brief    构造函数
	 * @param  ParentDirName 要查询的目录
	 * @param  recursion 确定是否递归查询  设备句柄
	 * @param  fext 如果要模糊查询,传入要模糊查询的字符串,默认查找目录下所有文件
	*/
	CDirsRef(CString ParentDirName,UINT recursion = 0,CString fext = _T(""));

	void Load(CString ParentDirName,UINT recursion = 0,CString fext = _T("")) ;

	void UnisSerialize(CArchive& ar);

	/**
	 * @brief    读取查询到的文件数
	 * @return  数目
	 * @retval  查询到的文件数目
	*/
	inline UINT ReadFileCnt() {return m_iFileCnt;};

	/**
	 * @brief    读取查询到的目录数
	 * @return  数目
	 * @retval  查询到的目录数目
	*/
	inline UINT ReadDirCnt() {return m_iDirCnt;}; 

	/**
	 * @brief    读一条文件名称
	 * @param  pos 位置
	 * @return  字符串
	 * @retval  是文件全名称截断挂载路径后的字符串
	*/
	CString ReadOneFileName(UINT pos) ;

	/**
	 * @brief    读一条目录名称
	 * @param  pos 位置
	 * @return  字符串
	 * @retval  是目录全名称截断挂载路径后的字符串
	*/
	CString ReadOneDirName(UINT pos) ;

	/**
	 * @brief    取目录数
	 * @return  数量
	 * @retval  查询到的目录数量
	*/
	int GetDirsCnt();	

	/**
	 * @brief    取文件数
	 * @return  数量
	 * @retval  查询到的文件数量
	*/
	int GetFilesCnt();

private:
	
	void GetFileAndDir(CString,UINT,CString fext = _T(""));

	//写入一个子目录名
	void WriteDirs(CString DirsName);

	//写入一个 文件名文件
	void WriteFiles(CString FileName);

private:
	UINT m_iDirCnt;
	CStringList m_vDirPath;
	UINT m_iFileCnt;
	CStringList m_vFilePath;

	DECLARE_SERIAL(CDirsRef)
};

#endif
