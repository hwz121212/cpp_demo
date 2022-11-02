#pragma once
#define APP_NAME L"APP_NAME"
#define FILE_NAME L"config.ini"
class Config
{
public:	
	Config(void);
	~Config(void);
	static void WriteConfig(const TCHAR* appname,const TCHAR* keyname,const TCHAR* value,const TCHAR* file);
	static void WriteConfig(const TCHAR* keyname,const TCHAR* value);
	static void ReadConfig(const TCHAR* appname,const TCHAR* keyname,	TCHAR* value,unsigned int size,const TCHAR* file);
	static void ReadConfig(const TCHAR* keyname,TCHAR* value);
};