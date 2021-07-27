#pragma once

#include "IEnemyState.h"

class EnemyTrackingState : public IEnemyState
{
public:
	EnemyTrackingState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
};

