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
        //更新必要。
        m_nodeCell = m_astar.Search(m_pos, m_targetPos, NaviMeshObj()->GetCellList());
        NaviMesh::Cell* parentCell = m_nodeCell;
        while (parentCell != nullptr)
        {
            //親セルに変える。
            parentCell = parentCell->m_parent;
            //親セルがなくなるまで頂点リストに格納する。
            if (parentCell != nullptr) {
                //軸直す。
                m_posList.push_back(parentCell->m_CenterPos);
                printf("次のセルの中心座標は(%f, %f, %f)です。\n", parentCell->m_CenterPos.x, parentCell->m_CenterPos.y, parentCell->m_CenterPos.z);
            }

        }
        m_dirty = true;
    }

    //ここから移動処理。
    //次のセルに向かうベクトル。
    if (m_posList.size() != 0) {
        Vector3 toNextCell = m_posList.back() - m_pos;
        //方位ベクトル化。
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

