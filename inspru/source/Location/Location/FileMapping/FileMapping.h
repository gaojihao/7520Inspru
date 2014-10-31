#ifndef __FILEMAPPING__
#define __FILEMAPPING__

#include "../Location.h"

#define	FILE_MAPPING_NAME					_T("FILE_MAPPING_BD2_GPS_POS_DATA")

HANDLE InitFileMapping(PPOS_INFO* ppMapMem, DWORD dwDesiredAccess = FILE_MAP_ALL_ACCESS);
BOOL UnInitFileMapping(HANDLE hMemShare, PPOS_INFO* ppMapMem);

#endif//__FILEMAPPING__
