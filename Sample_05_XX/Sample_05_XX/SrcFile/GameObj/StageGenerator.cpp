#include "stdafx.h"
#include "StageGenerator.h"
#include "Stages/Stage.h"
#include "Stages/BattleStage1.h"

void StageGenerator::OnDestroy()
{
}

bool StageGenerator::Start()
{
	return true;
}

void StageGenerator::Update()
{
}

void StageGenerator::CreateStage(const StageNumber& stageNum)
{
	if (stageNum == EnStageNumber_StanbyStage) {
		m_stanbyStage = NewGO<Stage>(EnPriority_3DModel);
		m_stanbyStage->SetPlayer(m_player);
		m_currentStageNum = EnStageNumber_StanbyStage;
	}
	else if (stageNum == EnStageNumber_BattleStage) {
		m_battleStage = NewGO<BattleStage1>(EnPriority_3DModel);
		m_battleStage->SetPlayer(m_player);
		m_currentStageNum = EnStageNumber_BattleStage;
	}
	else if (stageNum == EnStageNumber_BattleStage2) {

	}
	else if (stageNum == EnStageNumber_BattleStage3) {

	}
	else {
		//エラー。
		MessageBoxA(
			nullptr, 
			"ステージが削除済みまたは、引数で渡されたステージは存在しません。",
			"StageGenerator", 
			MB_OK
		);
	}
}

void StageGenerator::DeleteCurrentStage()
{
	if (m_currentStageNum == EnStageNumber_StanbyStage) {
		DeleteGO(m_stanbyStage);
	}
	else if (m_currentStageNum == EnStageNumber_BattleStage) {
		DeleteGO(m_battleStage);
	}
	else if (m_currentStageNum == EnStageNumber_BattleStage2) {

	}
	else if (m_currentStageNum == EnStageNumber_BattleStage3) {

	}
	else {
		//エラー。
		MessageBoxA(
			nullptr,
			"ステージが削除済みまたは、引数で渡されたステージは存在しません。",
			"StageGenerator",
			MB_OK
		);
	}
	//番号初期化。
	m_currentStageNum = EnstageNumber_None;
}
