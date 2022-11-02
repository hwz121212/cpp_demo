#ifndef __22_WTA_ATW_20141217_LL_H__
#define __22_WTA_ATW_20141217_LL_H__
#pragma once

class C22
{
public:
	C22(const char *srcBuf)
	{
		m_isNewWchar = true;
		m_isNewchar = false;
		m_wszDest = NULL;
		m_szDest = (char *)srcBuf;
		if(srcBuf == NULL) return;
		int nNeedNum = MultiByteToWideChar(CP_ACP,0,srcBuf,-1,NULL,0);
		m_wszDest = new wchar_t[nNeedNum + 1];
		MultiByteToWideChar(CP_ACP,0,srcBuf,-1,m_wszDest,nNeedNum);
		m_wszDest[nNeedNum] = '\0';

	}

	C22(const wchar_t *srcBuf)
	{
		m_isNewWchar = false;
		m_isNewchar = true;
		m_szDest = NULL;
		m_wszDest = (wchar_t *)srcBuf;
		if(srcBuf == NULL) return;
		int nNeedNum = WideCharToMultiByte(CP_ACP,0,srcBuf,-1,NULL,0,NULL,NULL);
		m_szDest = new char[nNeedNum + 1];
		WideCharToMultiByte(CP_ACP,0,srcBuf,-1,m_szDest,nNeedNum,NULL,NULL);
		m_szDest[nNeedNum] = '\0';

	}

	operator wchar_t * ()
	{
		return m_wszDest;
	}

	operator char * ()
	{
		return m_szDest;
	}

	~C22()
	{
		if(m_isNewWchar)
		{
			delete []m_wszDest;
			m_wszDest = NULL;
		}

		if(m_isNewchar)
		{
			delete []m_szDest;
			m_szDest = NULL;
		}
	}

private:
	wchar_t *m_wszDest;
	char *m_szDest;
	bool m_isNewWchar;
	bool m_isNewchar;
};

#endif