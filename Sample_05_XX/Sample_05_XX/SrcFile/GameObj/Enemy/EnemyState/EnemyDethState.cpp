#include "stdafx.h"
#include "EnemyDethState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

void EnemyDethState::Enter()
{
    m_enemy->GetRender()->PlayAnimation(3, 1.0f);
    m_enemy->GetCollision().Release();

    m_enemy->GetRender()->PlayAnimation(EnEnemyAnimation_Death, 0.7f);
    m_lastUpdateState = this;
}

void EnemyDethState::Leave()
{
}

void EnemyDethState::Update()
{
}
