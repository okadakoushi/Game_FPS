#pragma once

#include "IEnemyState.h"

class EnemyWanderingState : public IEnemyState
{
public:
	EnemyWanderingState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
};

