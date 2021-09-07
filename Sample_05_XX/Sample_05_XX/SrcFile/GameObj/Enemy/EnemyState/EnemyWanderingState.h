#pragma once

#include "IEnemyState.h"

class EnemyWanderingState : public IEnemyState
{
public:
	EnemyWanderingState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
		m_enemy = enemy;
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
private:
	RifleEnemy* m_enemy = nullptr;
};

