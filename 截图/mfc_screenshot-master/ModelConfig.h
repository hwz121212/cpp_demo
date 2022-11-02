#pragma once
class CModelConfig
{
public:
	CModelConfig(void);
	~CModelConfig(void);
	BOOL m_cbx_max_width;
	BOOL m_cbx_max_height;
	CString m_edit_max_width;
	CString m_edit_max_height;
	CString m_edit_key;
};

