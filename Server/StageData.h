#ifndef _STAGEDATA_H_
#define _STAGEDATA_H_

#include "MonsterData.h"
#include "StageInfo.h"
#include "Global.h"

class StageData
{
private:
	StageInfo* m_stage_origin[STAGE_COUNT];
	bool isLoad;
public:
	StageData();
	~StageData();

	void SetStageDataOrigin(StageInfo* _origin[]);

	const StageInfo* GetFirstStage() { return m_stage_origin[0]; }
	const StageInfo* GetSecondStage() { return m_stage_origin[1]; }
	const StageInfo* GetThirdStage() { return m_stage_origin[2]; }
};

#endif
