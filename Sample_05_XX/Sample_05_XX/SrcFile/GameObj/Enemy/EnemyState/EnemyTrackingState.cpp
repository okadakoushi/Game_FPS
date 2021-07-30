#include "stdafx.h"
#include "EnemyTrackingState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

EnemyTrackingState::~EnemyTrackingState()
{
    DeleteGO(m_missingMark);
}

void EnemyTrackingState::Enter()
{
	m_enemy->m_modelRender->PlayAnimation(EnEnemyAnimation_Run, 0.3f);
    m_lastUpdateState = this;
}

void EnemyTrackingState::Leave()
{
    //��������H�}�[�N�͏����B
    m_missingMark->SetMulColor(Vector4::Transparent);
    //�ړ����x���߂��B
    m_enemy->GetAgent().SetAgentSpeed(m_enemy->GetAgent().GetAgentSpeed() * 0.5f);
}

void EnemyTrackingState::Update()
{
    m_enemy->GetAgent().SetAgentSpeed(m_enemy->GetAgent().GetAgentSpeed() * 2);
    //�o�H��T�����Ĉړ��B
    //�o�H�ɉ����Đi�s���B
    m_enemy->Move();

    m_missingMark->SetPos({ m_enemy->GetPosition().x, m_enemy->GetPosition().y + 120.0f, m_enemy->GetPosition().z });
    //��]�B
    Quaternion markRot;
    Vector3 cameraPos = GraphicsEngineObj()->GetCamera3D().GetPosition();
    cameraPos.y = 0.0f;
    Vector3 markPos = m_enemy->GetPosition();
    markPos.y = 0.0f;
    Vector3 ToCamera = markPos - cameraPos;
    ToCamera.Normalize();
    ToCamera *= -1.0f;
    markRot.SetRotation(g_vec3Front, ToCamera);
    m_missingMark->SetRotation(markRot);
    m_missingMark->SetMulColor(Vector4::White);

    if (m_enemy->GetAgent().IsArriveTarget()) {
        m_enemy->SetFindPlayerFlag(false);
        printf("�ǐՏI��\n");
    }
}
