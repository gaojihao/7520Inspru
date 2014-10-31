/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $Ex\ex_imp\ex_file.h

 $Reversion History:

        (1) Created Liuhl@2007-12-250 13:37

 $Notes:
		
--*/

#ifndef  EX_FILE_H
#define  EX_FILE_H

#include "ex_basics.h"

class cex_file
{
public:
	cex_file();
	~cex_file();

	static  bool rename(const char* old_name, const char* new_name );
	static  bool remove(const char* file_name);
	static  bool create(const char* file_path);
	static  bool copyfile(const char* src_file, const char* new_file, bool is_overwrite);
	static  bool is_exist(const char* file_path );

	enum { file_null = -1 };
	enum { begin=0, current=1, end=2 };
	enum { m_read=0, m_write=1, m_readwrite=2};  // for open 'flag'


public:
	bool       open(const char* file_path, unsigned flag );
	unsigned   read(void* p_buf, unsigned n_count );
	bool       write( const void* p_buf, unsigned n_size );
	long	   get_file_size();
	
	long       seek( unsigned off_set, unsigned from );
	long       seek2end();
	long       seek2begin();

	void       close();
	void       flush();
	unsigned   size();
	
protected:
	unsigned int m_hfile;

};

#endif // EX_FILE_H