#include "stdafx.h"
#include "StageGenerator.h"
#include "Stages/Stage.h"
#include "Stages/BattleStage1.h"
#include "SrcFile/Fade.h"
#include "GamePlayer.h"

StageGenerator::StageGenerator()
{
}

void StageGenerator::OnDestroy()
{
	DeleteCurrentStage();
}

bool StageGenerator::Start()
{
	return true;
}

void StageGenerator::Update()
{
	//if (m_currentStageNum == EnStageNumber_StanbyStage) {
	//	if (m_stanbyStage->IsStart()) {
	//		//始まったらフェードを開ける。
	//	}
	//}
	//else if (m_currentStageNum == EnStageNumber_BattleStage) {
	//	if (m_stanbyStage->IsStart()) {
	//		//始まったらフェードを開ける。
	//	}
	//}
}

void StageGenerator::CreateStage(const StageNumber& stageNum)
{
	if (stageNum == EnStageNumber_BattleStage1) {
		m_stanbyStage = NewGO<Stage>(EnPriority_3DModel);
		m_stanbyStage->SetPlayer(m_player);
		m_currentStageNum = EnStageNumber_BattleStage1;
	}
	else if (stageNum == EnStageNumber_BattleStage2) {
		m_battleStage = NewGO<BattleStage1>(EnPriority_3DModel);
		m_battleStage->SetPlayer(m_player);
		m_currentStageNum = EnStageNumber_BattleStage2;
	}
	else if (stageNum == EnStageNumber_BattleStage3) {

	}
	else if (stageNum == EnStageNumber_BattleStage4) {

	}
	else {
		//printf("StageNum = %d", stageNum);
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
	if (m_currentStageNum == EnStageNumber_BattleStage1) {
		DeleteGO(m_stanbyStage);
		m_player->Init();
	}
	else if (m_currentStageNum == EnStageNumber_BattleStage2) {
		DeleteGO(m_battleStage);
		m_player->Init();
	}
	else if (m_currentStageNum == EnStageNumber_BattleStage3) {

	}
	else if (m_currentStageNum == EnStageNumber_BattleStage4) {

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
	return;
	//番号初期化。
	//m_currentStageNum = EnstageNumber_None;
}
