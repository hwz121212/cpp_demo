#include "StdAfx.h"
#include "ModelConfigTool.h"


CModelConfigTool::CModelConfigTool(void)
{
}


CModelConfigTool::~CModelConfigTool(void)
{
}

CModelConfig* CModelConfigTool::Load()
{
	CModelConfig* model = new CModelConfig;
	
	TCHAR tmp[100];
	Config::ReadConfig(L"m_cbx_max_height",tmp);
	model->m_cbx_max_height = _wtoi(tmp);

	Config::ReadConfig(L"m_cbx_max_width",tmp);
	model->m_cbx_max_width = _wtoi(tmp);

	Config::ReadConfig(L"m_edit_max_height",tmp);
	model->m_edit_max_height.Format(L"%s",tmp);

	Config::ReadConfig(L"m_edit_max_width",tmp);
	model->m_edit_max_width.Format(L"%s",tmp);

	Config::ReadConfig(L"m_edit_key",tmp);
	if(wcscmp(tmp,L"")==0)
	{
		model->m_edit_key = L"X";
	}
	else
	{
		model->m_edit_key.Format(L"%s",tmp);
	}
	return model;
}
void CModelConfigTool::Save(const CModelConfig *model)
{
	CString tmp;
	tmp.Format(L"%d",model->m_cbx_max_height);
	Config::WriteConfig(L"m_cbx_max_height",tmp);

	tmp.Format(L"%d",model->m_cbx_max_width);
	Config::WriteConfig(L"m_cbx_max_width",tmp);

	Config::WriteConfig(L"m_edit_max_height",model->m_edit_max_height);

	Config::WriteConfig(L"m_edit_max_width",model->m_edit_max_width);

	Config::WriteConfig(L"m_edit_key",model->m_edit_key);
}