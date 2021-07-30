#include "stdafx.h"
#include "EnemyDethState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

void EnemyDethState::Enter()
{
    printf("シニマース。\n");
    m_enemy->m_modelRender->PlayAnimation(3, 1.0f);
    m_enemy->m_collision.Release();

    m_enemy->m_modelRender->PlayAnimation(EnEnemyAnimation_Death, 0.7f);
    m_lastUpdateState = this;
}

void EnemyDethState::Leave()
{
}

void EnemyDethState::Update()
{
}
