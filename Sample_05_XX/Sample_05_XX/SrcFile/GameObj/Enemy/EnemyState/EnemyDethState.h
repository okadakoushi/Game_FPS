#pragma once

#include "IEnemyState.h"

class EnemyDethState : public IEnemyState
{
public:
	EnemyDethState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
};

