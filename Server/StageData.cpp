#include"StageData.h"

StageData::StageData()
{
	memset(m_stage_origin, 0, sizeof(m_stage_origin));
	isLoad = false;
}

StageData::~StageData()
{
	if (isLoad)
	{
		for (int i = 0; i < STAGE_COUNT; i++)
		{
			delete m_stage_origin[i];
		}
	}
}

void StageData::SetStageDataOrigin(StageInfo * _origin[])
{
	for (int i = 0; i < STAGE_COUNT; i++)
	{
		m_stage_origin[i] = _origin[i];
	}
	isLoad = true;
}
