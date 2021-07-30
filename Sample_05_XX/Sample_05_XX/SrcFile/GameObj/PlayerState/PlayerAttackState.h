#pragma once

#include "IPlayerState.h"

class PlayerAttackState : public IPlayerState
{
public:
	PlayerAttackState(GamePlayer* player) :
		IPlayerState(player)
	{
		//SE初期化。
		m_shootSE = NewGO<SoundSource>(0);
		m_shootSE->Init(L"Assets/Audio/AK47Shoot.wav");
		//エフェクト生成。
		m_effect = NewGO<myEngine::Effect>(EnPriority_3DModel);
		m_effect->SetScale({ 3.0f, 3.0f, 3.0f });
	}
	void Enter() override;
	void Update() override;
	void Leave() override;
private:
	const float RAY_RANGE = 8000.0f;
	SoundSource* m_shootSE = nullptr;			//銃の発射音。
	myEngine::Effect* m_effect = nullptr;		//エフェクト。
};

