#pragma once

#include "IEnemyState.h"

class EnemyAttackState : public IEnemyState
{
public:
	EnemyAttackState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
};

