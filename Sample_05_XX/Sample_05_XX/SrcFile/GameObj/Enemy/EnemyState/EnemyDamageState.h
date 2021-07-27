#pragma once

#include "IEnemyState.h"

class EnemyDamageState : public IEnemyState
{
public:
	EnemyDamageState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
};

