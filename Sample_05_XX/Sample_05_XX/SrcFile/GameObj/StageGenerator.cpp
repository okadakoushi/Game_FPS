#include "stdafx.h"
#include "StageGenerator.h"
#include "Stages/Stage.h"
#include "Stages/BattleStage1.h"
#include "SrcFile/Fade.h"
#include "GamePlayer.h"
#include "SrcFile/nature/SkyBox.h"

StageGenerator::StageGenerator()
{
}

void StageGenerator::OnDestroy()
{
	DeleteCurrentStage();
	DeleteGO(m_windSE);
}

bool StageGenerator::Start()
{
	//風のSE。
	m_windSE = NewGO<SoundSource>(0);
	m_windSE->Init(L"Assets/Audio/wind.wav");
	m_windSE->SetVolume(0.1f);

	//フェード。
	m_fade = NewGO<Fade>(EnPriority_UI);

	m_currentStageNum = EnStageNumber_BattleStage1;

	return true;
}

void StageGenerator::Update()
{
	m_windSE->Play(true);

	if (!m_isStageDeleteCall) {
		//ステージ削除した後でない。
		return;
	}
	if (m_currentStageNum == EnStageNumber_BattleStage1) {
		if (m_stanbyStage->IsStart()) {
			//レベルの初期化が終了。
			FadeProcess(false);
			m_isStageDeleteCall = false;
		}
	}
	else if (m_currentStageNum == EnStageNumber_BattleStage2) {
		if (m_battleStage->IsStart()) {
			//レベルの初期化が終了。
			FadeProcess(false);
			m_isStageDeleteCall = false;
		}
	}
}

bool StageGenerator::FadeProcess(const bool& fadeIn)
{
	if (m_firstFadeCall) {
		m_fade->StartFade(fadeIn);
		m_firstFadeCall = false;
	}

	if (m_fade->IsFinishFade()) {
		printf("Fadeが終了した\n");
		//フェード終了
		m_firstFadeCall = true;
		return true;
	}

	if (!fadeIn) {
		//フェードアウトの場合は即初期化。
		m_firstFadeCall = true;
	}

	printf("false\n");
	return false;
}

void StageGenerator::CreateStage(const StageNumber& stageNum)
{
	if (stageNum == EnStageNumber_BattleStage1) {

		m_stanbyStage = NewGO<Stage>(EnPriority_3DModel);

#ifdef GAME
		if (m_sky != nullptr) {
			DeleteGO(m_sky);
		}
		//skyも変更。
		m_sky = NewGO<SkyBox>(EnPriority_3DModel, "Sky");
		m_sky->SetSkyCubeTexturePath(L"Assets/modelData/nature/skyCubeMapNight_Toon_02.dds");
#endif
		GraphicsEngineObj()->GetDefferd().GetDefferdSprite().SetIBLItensity(4.0f);
		m_stanbyStage->SetPlayer(m_player);
		m_currentStageNum = EnStageNumber_BattleStage1;
	}
	else if (stageNum == EnStageNumber_BattleStage2) {
		m_battleStage = NewGO<BattleStage1>(EnPriority_3DModel);
#ifdef GAME
		DeleteGO(m_sky);
		m_sky = NewGO<SkyBox>(EnPriority_3DModel, "Sky");
		//skyも変更。
		m_sky->SetSkyCubeTexturePath(L"Assets/modelData/nature/skyCubeMapSpace_Toon_2.dds");
#endif
		GraphicsEngineObj()->GetDefferd().GetDefferdSprite().SetIBLItensity(1.0f);
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
	m_isStageDeleteCall = true;
	return;
	//番号初期化。
	//m_currentStageNum = EnstageNumber_None;
}

void StageGenerator::SetPlayerRespawnPos()
{
	m_player->SetPos(m_stanbyStage->GetPlayerPos());
}
