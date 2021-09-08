#include "stdafx.h"
#include "StageGenerator.h"
#include "Stage.h"
#include "BattleStage1.h"
#include "SrcFile/Fade.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/nature/SkyBox.h"

StageGenerator::StageGenerator()
{
	//各ステージのライティングパラメーターを設定。
	//Startでやってもいいな。

	//s1
	{
		m_ligParams[EnStageNumber_BattleStage1].DirectionLightColor = { 1.4f, 1.4f, 1.4f, 1.0f };
		m_ligParams[EnStageNumber_BattleStage1].DirectionLightDir = { 1.0f, -1.0f, 0.3f };
		m_ligParams[EnStageNumber_BattleStage1].SkySelfLuminous = 1.0f;
		m_ligParams[EnStageNumber_BattleStage1].IBLItensity = 4.0f;
		m_ligParams[EnStageNumber_BattleStage1].MiddleGray = 0.1f;
	}

	//s2
	{
		m_ligParams[EnStageNumber_BattleStage2].DirectionLightColor = { 1.4f, 1.4f, 1.4f, 1.0f };
		m_ligParams[EnStageNumber_BattleStage2].DirectionLightDir = { 1.0f, -1.0f, 0.3f };
		m_ligParams[EnStageNumber_BattleStage2].SkySelfLuminous = 1.0f;
		m_ligParams[EnStageNumber_BattleStage2].IBLItensity = 1.0f;
		m_ligParams[EnStageNumber_BattleStage2].MiddleGray = 0.15f;
	}

	//s3
	{
		m_ligParams[EnStageNumber_BattleStage3].DirectionLightColor = { 15.4f, 15.4f, 15.4f, 1.0f };
		m_ligParams[EnStageNumber_BattleStage3].DirectionLightDir = { 1.0f, -1.0f, 0.3f };
		m_ligParams[EnStageNumber_BattleStage3].SkySelfLuminous = 10.0f;
		m_ligParams[EnStageNumber_BattleStage3].IBLItensity = 1.0f;
		m_ligParams[EnStageNumber_BattleStage3].MiddleGray = 0.18f;
	}

	//s4 没予定。
	{
		m_ligParams[EnStageNumber_BattleStage4].DirectionLightColor = { 1.4f, 1.4f, 1.4f, 1.0f };
		m_ligParams[EnStageNumber_BattleStage4].DirectionLightDir = { 1.0f, -1.0f, 0.3f };
		m_ligParams[EnStageNumber_BattleStage4].SkySelfLuminous = 3.0f;
		m_ligParams[EnStageNumber_BattleStage4].IBLItensity = 2.0f;
		m_ligParams[EnStageNumber_BattleStage4].MiddleGray = 0.23f;
	}
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
	if (m_currentStageNum == EnStageNumber_BattleStage1 || m_currentStageNum == EnStageNumber_BattleStage3) {
		if (m_stage->IsStart()) {
			//レベルの初期化が終了。
			FadeProcess(false);
			m_isStageDeleteCall = false;
		}
	}
	else if (m_currentStageNum == EnStageNumber_BattleStage2 || m_currentStageNum == EnStageNumber_BattleStage4) {
		if (m_stage->IsStart()) {
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
		//フェード終了
		m_firstFadeCall = true;
		return true;
	}

	if (!fadeIn) {
		//フェードアウトの場合は即初期化。
		m_firstFadeCall = true;
	}

	return false;
}

void StageGenerator::CreateStage(const StageNumber& stageNum)
{
	//引数で渡されたステージ番号に応じてステージを生成する。
	if (stageNum == EnStageNumber_BattleStage1) {
		m_stage = NewGO<Stage>(EnPriority_3DModel);
	}
	else if (stageNum == EnStageNumber_BattleStage2) {
		m_stage = NewGO<BattleStage1>(EnPriority_3DModel);
	}
	else if (stageNum == EnStageNumber_BattleStage3) {
		m_stage = NewGO<Stage>(EnPriority_3DModel);
	}
	else if (stageNum == EnStageNumber_BattleStage4) {
		m_stage = NewGO<BattleStage1>(EnPriority_3DModel);
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


	//player設定。
	m_stage->SetPlayer(m_player);
	//ジェネレーター設定。
	m_stage->SetStageGenerator(this);

	//ステージ番号に合わせてパラメーターを設定していく。
	//Dirライトのパラメーターを設定。
	g_light.directionalLight[0].color = m_ligParams[stageNum].DirectionLightColor;
	g_light.directionalLight[0].direction = m_ligParams[stageNum].DirectionLightDir;
	//SkyBoxの解放。
	if (m_sky != nullptr) {
		DeleteGO(m_sky);
	}
	//SkyBoxをインスタンス化。
	m_sky = NewGO<SkyBox>(EnPriority_3DModel, "SkyBox");
	//Sky用のパラメーターを設定。
	m_sky->SetSkyCubeTexturePath(m_skyBoxFilePaths[stageNum]);
	m_sky->SetSelfLuminous(m_ligParams[stageNum].SkySelfLuminous);
	//IBL用の輝度を設定。
	GraphicsEngineObj()->GetDefferd().GetDefferdSprite().SetIBLItensity(m_ligParams[stageNum].IBLItensity);
	//トーンマップ用ミドルグレイを設定。
	GraphicsEngineObj()->GetRenderingEngine().GetToneMap().SetMiddleGray(m_ligParams[stageNum].MiddleGray);
	//現在のステージ番号を更新する。
	m_currentStageNum = stageNum;
}

void StageGenerator::DeleteCurrentStage()
{
	DeleteGO(m_stage);
	m_player->Init();
	m_isStageDeleteCall = true;
	return;
}
