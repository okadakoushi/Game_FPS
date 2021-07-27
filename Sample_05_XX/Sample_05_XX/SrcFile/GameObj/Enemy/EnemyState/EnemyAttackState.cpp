#include "stdafx.h"
#include "EnemyAttackState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

void EnemyAttackState::Enter()
{
    m_enemy->m_modelRender->PlayAnimation(EnEnemyAnimation_Shoot, 0.3f);
}

void EnemyAttackState::Leave()
{
}

void EnemyAttackState::Update()
{
    //�v���C���[���������̂Ŏ��̖ړI�n�̓v���C���[�̈ʒu�ɐݒ�B
    m_enemy->m_agent.SetTargetPos(m_enemy->m_player->GetPos());
    //Player�𔭌������B
    m_enemy->m_player->GetPlayerUIs()->AddFindList(m_enemy);
    if (m_enemy->m_modelRender->GetAnimLoop() || !m_enemy->m_modelRender->isPlayAnim()) {
        //�A�j���[�V���������[�v������̂܂��́A���[�v���Ȃ����̂ŃA�j���[�V����������Ă��Ȃ��B
        //�A�j���[�V������؂�ւ��B
        m_enemy->m_modelRender->PlayAnimation(2, 0.1f);
        //�v���C���[�̕����Ɍ�����B
        m_enemy->m_rot.SetRotation(Vector3::AxisY, atan2f(m_enemy->m_toPlayerDir.x * 1.0f, m_enemy->m_toPlayerDir.z * 1.0f));
        if (m_enemy->COOLDOWN < m_enemy->m_currentTime) {
            //���C�̒����𒲐��B
            Vector3 target = m_enemy->m_toPlayerDir;
            target *= m_enemy->m_VISION;
            //�G�l�~�[�̃G�C���̐��x�𒲐��B
            target = { target.x + rand() % m_enemy->m_currentRondomAIM, target.y + rand() % m_enemy->m_currentRondomAIM, target.z };
            //�e�۔��ˁB
            //Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel);
            //bullet->SetPos(headPos);
            //bullet->SetRot(m_rot);
            //bullet->SetSpeed(110.0f);
            //���C���邽�т�Enemy��AIM�̃����_���l��������B
            m_enemy->m_currentRondomAIM--;
            //����l��ǉ������x�N�g���Ɍ������B
            //bullet->SetToTarget(target);
            //�e����Player�ɓ����邩���C�e�X�g�B
            RayTestCallBack::EnemyRayTestResult bulletRayCallBuck;
            PhysicObj().RayTest(m_enemy->m_headPos, target + m_enemy->m_headPos, bulletRayCallBuck);
            if (bulletRayCallBuck.hasHit() && bulletRayCallBuck.StaticObjectDist > bulletRayCallBuck.CharacterObjectDist) {
                //�v���C���[�ɖ����B
                m_enemy->m_player->DamageToPlayer(m_enemy->ATTACK);
                //printf("�����B");
                //���������̂ōĂ�AIM�̎����i���Ȃ��̂ɁB
                m_enemy->m_currentRondomAIM = m_enemy->MAX_RANDOM_AIM;
            }
            else {
                //printf("�O�����B");
            }
            m_enemy->m_currentTime = 0;
        }
    }
}