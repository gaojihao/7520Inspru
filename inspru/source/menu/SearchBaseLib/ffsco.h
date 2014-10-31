

#ifndef	__FFSCO__H

#define __FFSCO__H

#include <string>
#include <vector>

using namespace std;
#pragma warning(disable:4786)

const int max_limit = 0x100000;	//--1048576
const int def_limit = 0x10000;	//--65536

#ifdef _UNICODE

const TCHAR c_pathSeparator = '\\';

const wstring s_pathSeparator = _T("\\");

//--find files collection
class ffsco  
{
public:
	typedef vector<wstring>	typeT;
	virtual int match(wstring fext_, wstring file_);	//--�ļ�ƥ����
	
public:
//		void test();	//--example
	int find(wstring path_, wstring fext_ = _T(""));
	
public:
	wstring get(int i = 0) { return _co.at(i); }
	int count() { return _co_dir.size() + _co_file.size(); }
	//int count() { return _co.size(); }
	typeT co() { return _co; }
	typeT co_dir() { return _co_dir; }
	typeT co_file() { return _co_file; }
	
public:
	int limit() { return _limit; }
	//void limit(int limit_) { _limit = limit_; }
	void limit(int limit_);	//--���ý���ļ���Ŀ����
	
public:
	int dirs() { return _dirs; }	//get
	void dirs(int dirs) { _dirs = dirs; }	//--set
	int dir(wstring s);	//--�ж�s�Ƿ�ΪĿ¼
	
private:
	int _limit;	//--limit
	int _dirs;	//--dirs-�Ƿ������Ŀ¼(Ĭ��0-������)
	typeT _co_file;
	typeT _co_dir;
	typeT _co;
	
public:
	ffsco();
	virtual ~ffsco();
	
private:
	inline int _limit_() { return count() == limit(); }	//--limit test
	inline wstring _uppercase_(wstring s);	//--ת��s����д��ĸ
};

#else

const char c_pathSeparator = '\\';
const string s_pathSeparator = "\\";

//--find files collection
class ffsco  
{
public:
	typedef vector<string>	typeT;
	virtual int match(string fext_, string file_);	//--�ļ�ƥ����
	
public:
//		void test();	//--example
	int find(string path_, string fext_ = "");
	
public:
	string get(int i = 0) { return _co.at(i); }
	int count() { return _co_dir.size() + _co_file.size(); }
	//int count() { return _co.size(); }
	typeT co() { return _co; }
	typeT co_dir() { return _co_dir; }
	typeT co_file() { return _co_file; }
	
public:
	int limit() { return _limit; }
	//void limit(int limit_) { _limit = limit_; }
	void limit(int limit_);	//--���ý���ļ���Ŀ����
	
public:
	int dirs() { return _dirs; }	//get
	void dirs(int dirs) { _dirs = dirs; }	//--set
	int dir(string s);	//--�ж�s�Ƿ�ΪĿ¼
	
private:
	int _limit;	//--limit
	int _dirs;	//--dirs-�Ƿ������Ŀ¼(Ĭ��0-������)
	typeT _co_file;
	typeT _co_dir;
	typeT _co;
	
public:
	ffsco();
	virtual ~ffsco();
	
private:
	inline int _limit_() { return count() == limit(); }	//--limit test
	inline string _uppercase_(string s);	//--ת��s����д��ĸ
};

#endif
#endif	
