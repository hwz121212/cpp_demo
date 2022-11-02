/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	Exception.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration/Defn. file for class PEException.
*
* NOTES:		Error handling and fault tolerence of the application is done
*				by custom defined exception classes.
*
******************************************************************************/
#pragma once

//Base class for all other exception classes.
class PEException
{
protected:
	char *m_szErrMsg;//Stores the error message
public:
	PEException(){}
	virtual ~PEException(){delete []m_szErrMsg;}

	virtual void Report(){AfxMessageBox(m_szErrMsg);}


};
//File read write related exceptions
class PEFileException:public PEException
{
public:
	PEFileException(char *szErrMsg = NULL)
	{
		m_szErrMsg = szErrMsg;
		if(m_szErrMsg == NULL)
		{
			m_szErrMsg = new char[1024];
			strcpy(m_szErrMsg,"PEFileException: Unknown File Operation Error");
		}
	}
};
//Error while parsing the resources
class PEFormatException:public PEException
{
public:
	PEFormatException(char *szErrMsg = NULL)
	{
		m_szErrMsg = szErrMsg;
		if(m_szErrMsg == NULL)
		{
			m_szErrMsg = new char[1024];
			strcpy(m_szErrMsg,"PEFormatException: Invalid Header format");
		}
	}
};
//Errow while parsing the resources
class PEResParseException:public PEException
{
public:
	PEResParseException(char *szErrMsg = NULL)
	{
		m_szErrMsg = szErrMsg;
		if(m_szErrMsg == NULL)
		{
			m_szErrMsg = new char[1024];
			strcpy(m_szErrMsg,"PEResParseException: Resource Parsing Error. Unexpected Tree found while parsing.");
		}
	}
};
//User interface relate error occured
class PEUIException:public PEException
{
public:
	PEUIException(char *szErrMsg = NULL)
	{
		m_szErrMsg = szErrMsg;
		if(m_szErrMsg == NULL)
		{
			m_szErrMsg = new char[1024];
			strcpy(m_szErrMsg,"PEUIException: Error while Creating User Interface Objects.");
		}
	}
};