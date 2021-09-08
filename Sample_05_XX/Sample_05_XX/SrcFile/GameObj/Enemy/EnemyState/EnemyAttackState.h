#pragma once

#include "IEnemyState.h"

class PlayerUIs;

class EnemyAttackState : public IEnemyState
{
public:
	EnemyAttackState(RifleEnemy* enemy) :
		IEnemyState::IEnemyState(enemy) {
		m_enemy = enemy;
		m_shootSE = NewGO<SoundSource>(0);
		m_shootSE->Init(L"Assets/Audio/AK47Shoot.wav", true);
	}

	~EnemyAttackState()
	{
		DeleteGO(m_shootSE);
	}
	void Enter() override;
	void Leave() override;
	void Update() override;
private:
	RifleEnemy* m_enemy = nullptr;				//enemyPtr。
	SoundSource* m_shootSE = nullptr;			//発砲音。
	float m_weponRaitTimer = 0.0f;				//連射タイマー。
	const int MAX_RANDOM_AIM = 45;				//エネミーのAIMの最大乱れ値。
	int m_currentRondomAIM = MAX_RANDOM_AIM;	//エネミーの現在のAIMの乱れ値。
	const int ATTACK = 10;						//攻撃力。
};

