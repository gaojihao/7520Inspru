// gsmimp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "FileMapping.h"

HANDLE				g_hMemShare;			//共享内存句柄
stShareMemory*		g_pstShareMem;			//共享内存区指针

//内部使用函数
BOOL InitFileMapping()
{
	//初始化内存映射
	g_hMemShare = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE,		
		NULL, PAGE_READWRITE, 0, sizeof(stShareMemory), FILE_MAPPING_NAME);	
	if(NULL == g_hMemShare)
		goto FALSE_END;
	
	g_pstShareMem = (stShareMemory*)MapViewOfFile(g_hMemShare,
		PAGE_READWRITE, 0, 0, sizeof(stShareMemory));	
	if (NULL == g_pstShareMem)
	{
		UnmapViewOfFile(g_pstShareMem);
		CloseHandle(g_hMemShare);
		g_hMemShare = NULL;
		goto FALSE_END;
	}

	OutputDebugString(_T("InitFileMapping Success!\n"));
	return TRUE;

FALSE_END:
	OutputDebugString(_T("InitFileMapping Failure!\n"));
	return FALSE;
}

BOOL UnInitFileMapping()
{
	if(NULL == g_pstShareMem || NULL == g_hMemShare)
		goto FALSE_END;

	UnmapViewOfFile(g_pstShareMem);
	CloseHandle(g_hMemShare);
	g_hMemShare = NULL;

	OutputDebugString(_T("UnInitFileMapping Success!\n"));
	return TRUE;

FALSE_END:
	OutputDebugString(_T("UnInitFileMapping Failure!\n"));
	return FALSE;
}
