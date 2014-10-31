#ifndef _E710ERROR_H_
#define _E710ERROR_H_

int Open_Error_File(TCHAR *filename, FILE *fp_override = NULL);
int Close_Error_File(void);
int Write_Error(TCHAR *string, ...);

#endif

