#include "stdafx.h"
#include "guide.h"
#include "GamePlayer.h"
#include "StageGenerator.h"

void Guide::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

bool Guide::Start()
{
	m_stageGenerator = FindGO<StageGenerator>("StageGenerator");
	//モデル
	const char* animFilePaths[] = {
		"Assets/animData/guide/idle.tka"
	};
	m_skinModelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_skinModelRender->Init("Assets/modelData/Chara/guide.tkm", animFilePaths);
	m_skinModelRender->SetScale({ 1.2f, 1.2f, 1.2f});
	m_player = FindGO<GamePlayer>("Player");
	m_collision.SetEnemyFlag(false);
	m_collision.Init(m_skinModelRender);
	return true;
}

void Guide::Update()
{
	m_skinModelRender->SetPosition(m_pos);

	//ステージ遷移処理。
	if (ClacPossibleTrans()) {
		//経過時間を加算。
		m_changeStageTimer += GameTime().GetFrameDeltaTime();
		if (m_changeStageTimer > m_CHANGESCENETIME) {
			//ステージ番号取っとく。
			int stageNum = m_stageGenerator->GetCurrentStageNumber();
			m_stageGenerator->DeleteCurrentStage();
			if (stageNum == StageGenerator::EnStageNumber_BattleStage
				|| stageNum == StageGenerator::EnStageNumber_BattleStage2
				|| stageNum == StageGenerator::EnStageNumber_BattleStage3) 
			{
				//バトルステージの場合は一旦スタンバイに戻す。
				m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_StanbyStage);
			}
			//test
			else if (stageNum == StageGenerator::EnStageNumber_StanbyStage) {
				m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_BattleStage);
			}
		}
	}
	else {
		//一回範囲からでちゃうとタイマーリセット。
		m_changeStageTimer = 0.0f;
	}
	m_collision.Update();
}

bool Guide::ClacPossibleTrans()
{
	Vector3 toGuide = m_pos - m_player->GetPos();
	float dist = toGuide.Length();
	if (dist < 150.0f) {
		return true;
	}
	else {
		return false;
	}
}
