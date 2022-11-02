#include "StdAfx.h"
#include "Config.h"


Config::Config(void)
{
}


Config::~Config(void)
{

}
void Config::WriteConfig(const TCHAR* appname,const TCHAR* keyname,const TCHAR* value,const TCHAR* file)
{
	WritePrivateProfileString(appname,keyname,value,file);
}
void Config::WriteConfig(const TCHAR* keyname,const TCHAR* value)
{
	///////////////获取当前文件夹路径//////////////////////
	TCHAR filename[1024];
	Util::GetFileDirectory(filename);
	wcscat(filename,L"\\");  
	wcscat(filename,FILE_NAME);  
	WriteConfig(APP_NAME,keyname,value,filename);
}
void Config::ReadConfig(const TCHAR* appname,const TCHAR* keyname,TCHAR* value,unsigned int size,const TCHAR* file)
{
	GetPrivateProfileString(APP_NAME,keyname,L"",value,size,file);
}
void Config::ReadConfig(const TCHAR* keyname,TCHAR* value)
{
	///////////////获取当前文件夹路径//////////////////////
	TCHAR filename[1024];
	Util::GetFileDirectory(filename);
	wcscat(filename,L"\\");  
	wcscat(filename,FILE_NAME);  

	ReadConfig(APP_NAME,keyname,value,1024,filename);	
}