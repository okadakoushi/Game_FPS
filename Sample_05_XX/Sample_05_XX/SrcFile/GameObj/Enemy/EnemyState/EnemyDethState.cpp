#include "stdafx.h"
#include "EnemyDethState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

void EnemyDethState::Enter()
{
    printf("�V�j�}�[�X�B\n");
    m_enemy->m_modelRender->PlayAnimation(3, 1.0f);
    if (!m_enemy->m_modelRender->isPlayAnim()) {
        //�X�V�~�߁A�R���W��������B
        m_enemy->SetActive(false);
        m_enemy->m_collision.Release();
    }

    m_enemy->m_modelRender->PlayAnimation(EnEnemyAnimation_Death, 0.7f);
}

void EnemyDethState::Leave()
{
}

void EnemyDethState::Update()
{
}
