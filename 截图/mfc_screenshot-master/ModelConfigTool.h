#pragma once
#include "ModelConfig.h"
#include "Config.h"
class CModelConfigTool
{
public:
	CModelConfigTool(void);
	~CModelConfigTool(void);
	static CModelConfig* Load();
	static void Save(const CModelConfig *model);
};

