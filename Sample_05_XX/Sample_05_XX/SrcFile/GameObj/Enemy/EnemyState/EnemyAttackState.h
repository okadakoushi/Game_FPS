#pragma once

#include "IEnemyState.h"

class PlayerUIs;

class EnemyAttackState : public IEnemyState
{
public:
	EnemyAttackState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
		m_shootSE = NewGO<SoundSource>(0);
		m_shootSE->Init(L"Assets/Audio/AK47Shoot.wav", true);
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
private:
	SoundSource* m_shootSE = nullptr;	//î≠ñCâπÅB
};

