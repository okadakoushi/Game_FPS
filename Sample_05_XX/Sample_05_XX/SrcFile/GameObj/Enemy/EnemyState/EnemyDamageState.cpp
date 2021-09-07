#include "stdafx.h"
#include "EnemyDamageState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"
#include "EnemyTrackingState.h"
#include "SrcFile/GameObj/GamePlayer.h"

void EnemyDamageState::Enter()
{
    if (m_enmey->GetHP() > 0 && m_enemy->IsActive()) {
        m_enmey->GetHP() -= 25;
    }

    m_enemy->GetRender()->PlayAnimation(EnEnemyAnimation_Damage, 0.3f);

    //“G‚ðŒ©Ž¸‚Á‚½ó‘Ô‚É‚·‚éB
    m_enemy->SetIsMissingFlag(true);
    m_enemy->GetAgent().SetTargetPos(m_enemy->GetPlayerPtr()->GetPos());

    m_lastUpdateState = this;
}

void EnemyDamageState::Leave()
{
}

void EnemyDamageState::Update()
{
}

