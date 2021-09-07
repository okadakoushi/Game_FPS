#include "stdafx.h"
#include "EnemyAttackState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"

void EnemyAttackState::Enter()
{
    m_enemy->GetRender()->PlayAnimation(EnEnemyAnimation_Shoot, 0.3f);
    m_lastUpdateState = this;
}

void EnemyAttackState::Leave()
{
}

void EnemyAttackState::Update()
{
    //�G�l�~�[������v���C���[�B
    Vector3 Head2PlayerDir = m_enemy->GetPlayerPtr()->GetPos() - m_enemy->GetHeadPos();
    Head2PlayerDir.Normalize();
   
    //�^�C�}�[���Z�B
    m_weponRaitTimer += GameTime().GetFrameDeltaTime();

    //�v���C���[���������̂Ŏ��̖ړI�n�̓v���C���[�̈ʒu�ɐݒ�B
    m_enemy->GetAgent().SetTargetPos(m_enemy->GetPlayerPtr()->GetPos());
    //Player�𔭌������B
    m_enemy->GetPlayerPtr()->GetPlayerUIs()->AddFindList(m_enemy);
    if (m_enemy->GetRender()->GetAnimLoop() || !m_enemy->GetRender()->isPlayAnim()) {
        //�A�j���[�V���������[�v������̂܂��́A���[�v���Ȃ����̂ŃA�j���[�V����������Ă��Ȃ��B
        //�A�j���[�V������؂�ւ��B
        m_enemy->GetRender()->PlayAnimation(EnEnemyAnimation_Shoot, 0.1f);
        //�v���C���[�̕����Ɍ�����B
        m_enemy->GetRotation().SetRotation(Vector3::AxisY, atan2f(Head2PlayerDir.x * 1.0f, Head2PlayerDir.z * 1.0f));
        if (m_enemy->GerRifle()->GetWeponRaito() * 1.5f < m_weponRaitTimer) {
            m_shootSE->SetPosition(m_enemy->GetPosition());
            m_shootSE->Stop();
            m_shootSE->Play();
            //���C�̒����𒲐��B
            Vector3 target = Head2PlayerDir;
            target *= m_enemy->GerRifle()->GetWeponEnableRange();
            //�G�l�~�[�̃G�C���̐��x�𒲐��B
            target = { target.x + rand() % m_currentRondomAIM, target.y + rand() % m_currentRondomAIM, target.z };
            //���C���邽�т�Enemy��AIM�̃����_���l��������B
            if (m_currentRondomAIM > 1) {
                m_currentRondomAIM--;
            }
            //����l��ǉ������x�N�g���Ɍ������B
            //bullet->SetToTarget(target);
            //�e����Player�ɓ����邩���C�e�X�g�B
            RayTestCallBack::EnemyRayTestResult bulletRayCallBuck;
            PhysicObj().RayTest(m_enemy->GetHeadPos(), target + m_enemy->GetHeadPos(), bulletRayCallBuck);
            if (bulletRayCallBuck.hasHit() && bulletRayCallBuck.StaticObjectDist > bulletRayCallBuck.CharacterObjectDist) {
                //�v���C���[�ɖ����B
                m_enemy->GetPlayerPtr()->DamageToPlayer(ATTACK);
                printf("�����B");
                //���������̂ōĂ�AIM�̎����i���Ȃ��̂ɁB
                m_currentRondomAIM = MAX_RANDOM_AIM;
            }
            else {
                //printf("�O�����B");
            }
            //�^�C�}�[���Z�b�g�B
            m_weponRaitTimer = 0;
        }
    }
}