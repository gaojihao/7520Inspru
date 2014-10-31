
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"	
#include "ffsco.h"
#include <windows.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _UNICODE

ffsco::ffsco()
	: _limit(def_limit), _dirs(0)
{
	
}

ffsco::~ffsco()
{
	
}

//////////////////////////////////////////////////////////////////////
wstring ffsco::_uppercase_(wstring s)
{
	const char aazz = 'z' - 'Z';
	wstring rs;
	for (wstring::iterator it = s.begin();
	s.end() != it; it++)
	{
		if ('a' <= *it && *it <= 'z') rs.append(1, *it - aazz);
		else rs.append(1, *it);
	}
	return rs;
}
//////////////////////////////////////////////////////////////////////

int ffsco::match(wstring fext_, wstring file_)
{
	wstring fext = _uppercase_(fext_);
	wstring file = _uppercase_(file_);
	
	int pos = file.find_last_of('.');
	if (wstring::npos != pos) file = file.substr(pos);
	
	return (wstring::npos != fext.find(file));
}

void ffsco::limit(int limit_)
{
	if (limit_ < 1) _limit = def_limit;
	if (limit_ < max_limit) _limit = limit_;
}

int ffsco::dir(wstring s)
{
	return (FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(LPCTSTR(s.c_str())));
}

int ffsco::find(wstring path_, wstring fext_)
{
	_co_file.clear();
	_co_dir.clear();
	_co.clear();
	
	wstring path = path_;
	if (path.size() > 0 && c_pathSeparator != path[path.size() - 1])
		path.append(s_pathSeparator);
	
	_co_dir.push_back( path );
	
	wstring fext = fext_;
	if (0 == fext.compare(_T("*"))
		|| 0 == fext.compare(_T("*.*"))
		) fext = _T("");

	//wstring file = fext;
	wstring file = _T("*");
	
	wstring s = path + file;
	
	WIN32_FIND_DATA fileinfo = {0};
	HANDLE handle = FindFirstFile( LPCTSTR(s.c_str()), &fileinfo );
	
	if (NULL != handle && INVALID_HANDLE_VALUE != handle)
	{
		
		do {
			
			if (_limit_()) break;	//--limit test
			
			if ('.' != fileinfo.cFileName[0])	//--skip./..
				if ((FILE_ATTRIBUTE_DIRECTORY & fileinfo.dwFileAttributes)
					== FILE_ATTRIBUTE_DIRECTORY)	//--Ŀ¼
				{
					if (!_limit_()	//--limit test
						//&& 0 == fext.size()
						)
					{
						wstring str1 = (TCHAR*)fileinfo.cFileName;
						_co_dir.push_back( path + str1 + s_pathSeparator );
					}
					
					if (0 != dirs())
					{
						ffsco o;
						o.dirs(dirs());
						wstring str2 = (TCHAR*)fileinfo.cFileName;
						o.find( path + str2, fext);
						//--dir
						typeT o_dir = o.co_dir();
						for (typeT::iterator it_dir = o_dir.begin();
						o_dir.end() != it_dir && !_limit_();	//--limit test
						it_dir ++) _co_dir.push_back(*it_dir);
						//--file
						typeT o_file = o.co_file();
						for (typeT::iterator it_file = o_file.begin();
						o_file.end() != it_file && !_limit_();	//--limit test
						it_file ++) _co_file.push_back(*it_file);
					}
				}
				else
				{
					if (!_limit_()	//--limit test
						&& (0 == fext.size() || match(fext, (TCHAR*)fileinfo.cFileName))
						)
					{
						wstring str3 = (TCHAR*)fileinfo.cFileName;
						_co_file.push_back( path + str3 );
					}
				}
				
		} while (FindNextFile( handle, &fileinfo ));
		
		FindClose(handle);
	}
	
	//--dir
	for (typeT::iterator it_dir = _co_dir.begin();
	_co_dir.end() != it_dir;
	it_dir ++) _co.push_back(*it_dir);
	//--file
	for (typeT::iterator it_file = _co_file.begin();
	_co_file.end() != it_file;
	it_file ++) _co.push_back(*it_file);
	
	return count();
}

		
#else


ffsco::ffsco()
	: _limit(def_limit), _dirs(0)
{
	
}

ffsco::~ffsco()
{
	
}

//////////////////////////////////////////////////////////////////////
string ffsco::_uppercase_(string s)
{
	const char aazz = 'z' - 'Z';
	string rs;
	for (string::iterator it = s.begin();
	s.end() != it; it++)
	{
		if ('a' <= *it && *it <= 'z') rs.append(1, *it - aazz);
		else rs.append(1, *it);
	}
	return rs;
}
//////////////////////////////////////////////////////////////////////

int ffsco::match(string fext_, string file_)
{
	string fext = _uppercase_(fext_);
	string file = _uppercase_(file_);
	
	int pos = file.find_last_of('.');
	if (string::npos != pos) file = file.substr(pos);
	
	return (string::npos != fext.find(file));
}

void ffsco::limit(int limit_)
{
	if (limit_ < 1) _limit = def_limit;
	if (limit_ < max_limit) _limit = limit_;
}

int ffsco::dir(string s)
{
	return (FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(LPCTSTR(s.c_str())));
}

int ffsco::find(string path_, string fext_)
{
	_co_file.clear();
	_co_dir.clear();
	_co.clear();
	
	string path = path_;
	if (path.size() > 0 && c_pathSeparator != path[path.size() - 1])
		path.append(s_pathSeparator);
	
	_co_dir.push_back( path );
	
	string fext = fext_;
	if (0 == fext.compare("*")
		|| 0 == fext.compare("*.*")
		) fext = "";

	//string file = fext;
	string file = "*";
	
	string s = path + file;
	
	WIN32_FIND_DATA fileinfo = {0};
	HANDLE handle = FindFirstFile( LPCTSTR(s.c_str()), &fileinfo );
	
	if (NULL != handle && INVALID_HANDLE_VALUE != handle)
	{
		
		do {
			
			if (_limit_()) break;	//--limit test
			
			if ('.' != fileinfo.cFileName[0])	//--skip./..
				if ((FILE_ATTRIBUTE_DIRECTORY & fileinfo.dwFileAttributes)
					== FILE_ATTRIBUTE_DIRECTORY)	//--Ŀ¼
				{
					if (!_limit_()	//--limit test
						//&& 0 == fext.size()
						)
					{
						string str1 = (const char*)fileinfo.cFileName;
						_co_dir.push_back( path + str1 + s_pathSeparator );
					}
					
					if (0 != dirs())
					{
						ffsco o;
						o.dirs(dirs());
						string str2 = (const char*)fileinfo.cFileName;
						o.find( path + str2, fext);
						//--dir
						typeT o_dir = o.co_dir();
						for (typeT::iterator it_dir = o_dir.begin();
						o_dir.end() != it_dir && !_limit_();	//--limit test
						it_dir ++) _co_dir.push_back(*it_dir);
						//--file
						typeT o_file = o.co_file();
						for (typeT::iterator it_file = o_file.begin();
						o_file.end() != it_file && !_limit_();	//--limit test
						it_file ++) _co_file.push_back(*it_file);
					}
				}
				else
				{
					if (!_limit_()	//--limit test
						&& (0 == fext.size() || match(fext, (const char*)fileinfo.cFileName))
						)
					{
						string str3 = (const char*)fileinfo.cFileName;
						_co_file.push_back( path + str3 );
					}
				}
				
		} while (FindNextFile( handle, &fileinfo ));
		
		FindClose(handle);
	}
	
	//--dir
	for (typeT::iterator it_dir = _co_dir.begin();
	_co_dir.end() != it_dir;
	it_dir ++) _co.push_back(*it_dir);
	//--file
	for (typeT::iterator it_file = _co_file.begin();
	_co_file.end() != it_file;
	it_file ++) _co.push_back(*it_file);
	
	return count();
}

#endif
