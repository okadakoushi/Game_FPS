#pragma once

#include "IEnemyState.h"

class EnemyTrackingState : public IEnemyState
{
public:
	EnemyTrackingState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
		//マーク初期化。
		SpriteInitData initData;
		initData.m_ddsFilePath[0] = "Assets/sprite/missing.dds";
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		initData.m_width = 50.0f;
		initData.m_height = 50.0f;
		m_missingMark = NewGO<SpriteRender>(EnPriority_UI);
		m_missingMark->Init(initData, true);
		m_missingMark->SetPos({ 0, 100, 0 });
		m_missingMark->SetMulColor(Vector4::Transparent);
	}
	~EnemyTrackingState();
	void Enter() override;
	void Leave() override;
	void Update() override;
private:
	SpriteRender* m_missingMark = nullptr;		//？マーク。
};

