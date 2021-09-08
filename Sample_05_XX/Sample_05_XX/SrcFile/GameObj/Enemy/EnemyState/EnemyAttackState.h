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
	RifleEnemy* m_enemy = nullptr;				//enemyPtr�B
	SoundSource* m_shootSE = nullptr;			//���C���B
	float m_weponRaitTimer = 0.0f;				//�A�˃^�C�}�[�B
	const int MAX_RANDOM_AIM = 45;				//�G�l�~�[��AIM�̍ő嗐��l�B
	int m_currentRondomAIM = MAX_RANDOM_AIM;	//�G�l�~�[�̌��݂�AIM�̗���l�B
	const int ATTACK = 10;						//�U���́B
};

