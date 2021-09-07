#pragma once

#include "IEnemyState.h"

class EnemyDamageState : public IEnemyState
{
public:
	EnemyDamageState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
		m_enmey = enemy;
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
private:
	RifleEnemy* m_enmey = nullptr;	//エネミー。
};

