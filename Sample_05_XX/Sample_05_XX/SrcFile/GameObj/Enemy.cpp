#include "stdafx.h"
#include "Enemy.h"

bool Enemy::Start()
{
    m_modelRender = NewGO<SkinModelRender>(EnPriority_3DRender);
    m_modelRender->Init("Assets/modelData/unityChan.tkm");
    m_pos = { 0,0,0 };
    return true;
}

void Enemy::Update()
{
    if (m_dirty == false) {
        //�X�V�K�v�B
        m_nodeCell = m_astar.Search(m_pos, m_targetPos, NaviMeshObj()->GetCellList());
        NaviMesh::Cell* parentCell = m_nodeCell;
        while (parentCell != nullptr)
        {
            //�e�Z���ɕς���B
            parentCell = parentCell->m_parent;
            //�e�Z�����Ȃ��Ȃ�܂Œ��_���X�g�Ɋi�[����B
            if (parentCell != nullptr) {
                //�������B
                m_posList.push_back(parentCell->m_CenterPos);
                printf("���̃Z���̒��S���W��(%f, %f, %f)�ł��B\n", parentCell->m_CenterPos.x, parentCell->m_CenterPos.y, parentCell->m_CenterPos.z);
            }

        }
        m_dirty = true;
    }

    //��������ړ������B
    //���̃Z���Ɍ������x�N�g���B
    if (m_posList.size() != 0) {
        Vector3 toNextCell = m_posList.back() - m_pos;
        //���ʃx�N�g�����B
        toNextCell.Normalize();
        m_pos += toNextCell * m_spped;

        toNextCell = m_posList.back() - m_pos;
        float dist = toNextCell.Length();
        if (dist < 5) {
            m_posList.erase(m_posList.end() - 1);
        }
    }
    else {
        int i = 0;
    }

    m_modelRender->SetRenderMode(enRenderMode_NonSkin);
    m_modelRender->SetPosition(m_pos);
}

