// gsmimp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "FileMapping.h"
#include "../Location.h"

PPOS_INFO			g_pPosInfo;				//共享内存区指针

//内部使用函数
HANDLE InitFileMapping(PPOS_INFO* ppMapMem, DWORD dwDesiredAccess /*= FILE_MAP_ALL_ACCESS*/)
{
	//初始化内存映射
	HANDLE hMemShare = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE,
		NULL, PAGE_READWRITE /*PAGE_READONLY*/, 0, sizeof(POS_INFO), FILE_MAPPING_NAME);
	if(NULL == hMemShare)
		goto FALSE_END;
	
	*ppMapMem = (PPOS_INFO)MapViewOfFile(hMemShare,
		FILE_MAP_ALL_ACCESS, 0, 0, sizeof(POS_INFO));
	int i = GetLastError();
	if (NULL == ppMapMem || NULL == *ppMapMem)
	{
		UnInitFileMapping(hMemShare, ppMapMem);
		goto FALSE_END;
	}

	OutputDebugString(_T("InitFileMapping Success!\n"));
	return hMemShare;

FALSE_END:
	OutputDebugString(_T("InitFileMapping Failure!\n"));
	return hMemShare;
}

BOOL UnInitFileMapping(HANDLE hMemShare, PPOS_INFO* ppMapMem)
{
	if(NULL == ppMapMem || NULL == hMemShare)
		goto FALSE_END;

	UnmapViewOfFile(*ppMapMem);
	CloseHandle(hMemShare);
	hMemShare = NULL;
	*ppMapMem = NULL;

	OutputDebugString(_T("UnInitFileMapping Success!\n"));
	return TRUE;

FALSE_END:
	OutputDebugString(_T("UnInitFileMapping Failure!\n"));
	return FALSE;
}
