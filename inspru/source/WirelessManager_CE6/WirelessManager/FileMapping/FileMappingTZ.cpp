// gsmimp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "FileMappingTZ.h"

HANDLE						g_hMemShareTianze;			//�����ڴ���
stShareMemoryTianze*		g_pstShareMemTianze;			//�����ڴ���ָ��

//�ڲ�ʹ�ú���
BOOL InitFileMappingTianze()
{
	//��ʼ���ڴ�ӳ��
	g_hMemShareTianze = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE,		
		NULL, PAGE_READWRITE, 0, sizeof(stShareMemoryTianze), FILE_MAPPING_NAME_TIANZE);	
	if(NULL == g_hMemShareTianze)
		goto FALSE_END;
	
	g_pstShareMemTianze = (stShareMemoryTianze*)MapViewOfFile(g_hMemShareTianze,
		PAGE_READWRITE, 0, 0, sizeof(stShareMemoryTianze));	
	if (NULL == g_pstShareMemTianze)
	{
		UnmapViewOfFile(g_pstShareMemTianze);
		CloseHandle(g_hMemShareTianze);
		g_hMemShareTianze = NULL;
		goto FALSE_END;
	}

	OutputDebugString(_T("InitFileMapping Success!\n"));
	return TRUE;

FALSE_END:
	OutputDebugString(_T("InitFileMapping Failure!\n"));
	return FALSE;
}

BOOL UnInitFileMappingTianze()
{
	if(NULL == g_pstShareMemTianze || NULL == g_hMemShareTianze)
		goto FALSE_END;

	UnmapViewOfFile(g_pstShareMemTianze);
	CloseHandle(g_hMemShareTianze);
	g_hMemShareTianze = NULL;

	OutputDebugString(_T("UnInitFileMapping Success!\n"));
	return TRUE;

FALSE_END:
	OutputDebugString(_T("UnInitFileMapping Failure!\n"));
	return FALSE;
}
