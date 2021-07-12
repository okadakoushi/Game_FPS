#include "stdafx.h"
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"

bool RifleEnemy::Start()
{
	//�G�l�~�[���f���������B
	const char* tkaFile[]{
		"Assets/animData/Enemy/Rifle/Walk.tka"
	};
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_modelRender->Init("Assets/modelData/Chara/soldierMob.tkm", tkaFile);
	m_modelRender->SetMulColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	//�e�C���X�^���X���B
	m_rifle = NewGO<Rifle>(EnPriority_3DModel);
	m_rifle->SetRefBoneRender(m_modelRender);
    //�o�H�T���̖ړI�n��ݒ�B
    m_targetPos = { 1000.0f, 0.0f, 500.0f };
    m_targetPos = { -1000.0f, 0.0f, -500.0f };
	return true;
}

void RifleEnemy::Update()
{
	//���}���u��swich-case�����������Ȃ�����X�e�[�g�p�^�[�������ŉB�����悤�B
	switch (m_enemyState)
	{
	case Enemy::EnEnemyState_Wandering:
        Move();
		//�x����ԁB
		break;
	case Enemy::EnEnemyState_Tracking:
		//�g���b�L���O��ԁB
		break;
	case Enemy::EnEnemyState_AttackWait:
		//�U���҂������B
		break;
	case Enemy::EnEnemyState_Attack:
		//�U���I
		break;
	case Enemy::EnEnemyState_Death:
		//���S�B
		break;
	}
	m_modelRender->SetPosition(m_pos);
    m_modelRender->SetRotation(m_rot);
	m_modelRender->SetScale(m_scale);
}

void RifleEnemy::Move()
{
    if (m_dirty == false) {
        //�X�V�K�v�B
        m_nodeList = m_astar.Search(m_pos, m_targetPos, m_naviMesh->GetCellList());
        NaviMesh::Cell* parentCell = m_nodeCell;
        //NaviMeshObj()->AgentNodeRender(m_nodeList);
        //���̖ړI�n�͍��̏ꏊ�B
        m_nextTarget = m_pos;
        m_dirty = true;
    }

    //��������ړ������B
    //���̃Z���Ɍ������x�N�g���B
    if (m_nodeList.size() != 0) {
        Vector3 toNextCell = m_nodeList.front()->m_CenterPos - m_pos;
        //���ʉ�����O�ɁA��������Ƃ��B
        float dist = toNextCell.Length();
        //���ʃx�N�g�����B
        toNextCell.Normalize();
        //�������B
        m_pos += toNextCell * m_spped;
        //��]�v�Z�B
        m_rot.SetRotation(Vector3::AxisY, atan2f(toNextCell.x * 1.0f, toNextCell.z * 1.0f));

        //�m�[�h�ɓ��B����������B
        if (dist < 5) {
            m_nodeList.erase(m_nodeList.begin());
        }
        //�����Ă�B
        //m_modelRender->PlayAnimation(0, 0.5f);
    }
    else {
        //�����ĂȂ��B
        //m_modelRender->PlayAnimation(1, 0.5f);
        //�X�V�B
        m_targetPos = m_nextTarget;
        m_dirty = false;
    }
}

