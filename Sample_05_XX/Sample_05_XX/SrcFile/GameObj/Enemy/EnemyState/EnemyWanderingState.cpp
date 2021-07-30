#include "stdafx.h"
#include "EnemyWanderingState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

void EnemyWanderingState::Enter()
{
	m_enemy->m_modelRender->PlayAnimation(EnEnemyAnimation_Walk, 0.3f);
	m_lastUpdateState = this;
}

void EnemyWanderingState::Leave()
{
}

void EnemyWanderingState::Update()
{
	m_enemy->Move();
}
