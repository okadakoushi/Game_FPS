#include "stdafx.h"
#include "EnemyDamageState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"
#include "EnemyTrackingState.h"
#include "SrcFile/GameObj/GamePlayer.h"

void EnemyDamageState::Enter()
{
    if (m_enemy->m_hp > 0 && m_enemy->IsActive()) {
        m_enemy->m_hp -= 25;
    }

    printf("今のHPは%d\n", m_enemy->m_hp);
    m_enemy->m_modelRender->PlayAnimation(EnEnemyAnimation_Damage, 0.3f);

    //敵を見失った状態にする。
    m_enemy->m_isMissingPlayer = true;
    m_enemy->GetAgent().SetTargetPos(m_enemy->m_player->GetPos());

    m_lastUpdateState = this;
}

void EnemyDamageState::Leave()
{
}

void EnemyDamageState::Update()
{
}

