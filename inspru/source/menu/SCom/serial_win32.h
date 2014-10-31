/*++
  ====+ (C) Copyright 2007-2008.  TargetCrop.  All right reserved. ====

               PROPRIETARY AND TRADE SECRET              

            Published only in a limited, copyright sense.   
  ===================================================================

 $Module Name:

        $src_code\gps\serial_win32.h

 $Reversion History:

        (1) Created   Liuhl@2008-02-32 11:14 AM

 $Notes:
		
	      This class included by this file must be rewrited.
--*/

#ifndef  SERIAL_WIN32_H
#define  SERIAL_WIN32_H

#include <windows.h>

class serial_win32
{
public:
	serial_win32();
	~serial_win32();

	enum  dev_state_e{ dev_close = 0, dev_open, dev_error };
	bool       open(int port, long baud, char parity=NOPARITY, char databits=8, char stopbit=ONESTOPBIT);
	bool       close();
	bool       is_open() { return (m_state == dev_open); };
	bool	   change_baud(long baud);

	// write the data to the serial 
	bool       write_data( const char* p_data, int n_count);
	int        read_data( char* p_buf, int n_count );
	
	// get the serial state: see dev_state_e
	unsigned   get_state();

	// empty the input buffer
	bool       clear_in_buffer();

	void       set_event(unsigned n_event);
	unsigned long  wait_event();

protected:
	void*      m_device;
	unsigned   m_state;
	long	   m_baud;

};


#endif //SERIAL_WIN32_H