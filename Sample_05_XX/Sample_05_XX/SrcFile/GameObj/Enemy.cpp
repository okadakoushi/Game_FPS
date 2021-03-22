#include "stdafx.h"
#include "Enemy.h"

bool Enemy::Start()
{
    m_modelRender = NewGO<SkinModelRender>(EnPriority_3DRender);
    const char* tkaFilePaths[]{
        "Assets/animData/soldierMob/walk.tka",
        "Assets/animData/soldierMob/idle.tka",
    };
    m_modelRender->Init("Assets/modelData/Chara/soldierMob.tkm", tkaFilePaths);
    m_modelRender->SetRenderMode(enRenderMode_Skin);
    m_modelRender->SetShadwoCaster(true);
    m_modelRender->SetShadowReciever(true);
    Vector3 scale = { 1.5f, 1.5f, 1.5f };
    m_modelRender->SetScale(scale);
    return true;
}

void Enemy::Update()
{
    if (m_dirty == false) {
        //�X�V�K�v�B
        m_nodeList = m_astar.Search(m_pos, m_targetPos, NaviMeshObj()->GetCellList());
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
        //���ʃx�N�g�����B
        toNextCell.Normalize();
        m_pos += toNextCell * m_spped;

        //todo:��������̏������R�[�h�����Ă��������������Ń��t�@�N�^�����O�B
        //������
        Quaternion qRot = Quaternion::Identity;
        //�p�x�����߂�
        float kakudo1 = toNextCell.Dot(Vector3::AxisZ);
        //1~-1
        float kakudo = toNextCell.Dot(Vector3::AxisX);
        //�p�x�̌v�Z
        kakudo1 = acos(kakudo1);
        int seifu = 1;
        //�����Ŋp�x�̐����̌���
        if (kakudo < 0)
        {
            seifu = -1;
        }
        //�ŏI�I�Ȋp�x  �p�x�𓮂��� �p�x�ɐ����������� 
        m_kakudo = kakudo1 * seifu;
        qRot.SetRotation(Vector3::AxisY, m_kakudo - 3.14);
        //��]�����Z
        m_rot.Multiply(qRot);
        //���f������]������
        m_modelRender->SetRotation(qRot);

        toNextCell = m_nodeList.front()->m_CenterPos - m_pos;
        float dist = toNextCell.Length();
        if (dist < 5) {
            m_nodeList.erase(m_nodeList.begin());
        }    
        //�����Ă�B
        m_modelRender->PlayAnimation(0, 0.5f);
    }
    else {
        //�����ĂȂ��B
        m_modelRender->PlayAnimation(1, 0.5f);
        //�X�V�B
        m_targetPos = m_nextTarget;
        m_dirty = false;
    }
    m_modelRender->SetPosition(m_pos);
}

void Enemy::ForwardRender()
{
}

