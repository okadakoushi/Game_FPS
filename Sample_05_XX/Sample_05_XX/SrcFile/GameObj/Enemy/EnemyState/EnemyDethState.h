#pragma once

#include "IEnemyState.h"

class EnemyDethState : public IEnemyState
{
public:
	EnemyDethState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
		m_deathSE = NewGO<SoundSource>(0);
		m_deathSE->Init(L"Assets/Audio/Death.wav");
	}

	~EnemyDethState()
	{
		DeleteGO(m_deathSE);
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
private:
	SoundSource* m_deathSE = nullptr;
};

