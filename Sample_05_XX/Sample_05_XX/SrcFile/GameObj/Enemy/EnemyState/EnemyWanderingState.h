#pragma once

#include "IEnemyState.h"

class EnemyWanderingState : public IEnemyState
{
	void Enter() override;
	void Leave() override;
	void Update() override;
};

