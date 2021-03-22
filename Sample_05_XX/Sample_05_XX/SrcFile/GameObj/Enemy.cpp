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
        //更新必要。
        m_nodeList = m_astar.Search(m_pos, m_targetPos, NaviMeshObj()->GetCellList());
        NaviMesh::Cell* parentCell = m_nodeCell;
        //NaviMeshObj()->AgentNodeRender(m_nodeList);
        //次の目的地は今の場所。
        m_nextTarget = m_pos;
        m_dirty = true;
    }

    //ここから移動処理。
    //次のセルに向かうベクトル。
    if (m_nodeList.size() != 0) {
        Vector3 toNextCell = m_nodeList.front()->m_CenterPos - m_pos;
        //方位ベクトル化。
        toNextCell.Normalize();
        m_pos += toNextCell * m_spped;

        //todo:ここから昔書いたコード持ってきただけだから後でリファクタリング。
        //初期化
        Quaternion qRot = Quaternion::Identity;
        //角度を決める
        float kakudo1 = toNextCell.Dot(Vector3::AxisZ);
        //1~-1
        float kakudo = toNextCell.Dot(Vector3::AxisX);
        //角度の計算
        kakudo1 = acos(kakudo1);
        int seifu = 1;
        //ここで角度の正負の決定
        if (kakudo < 0)
        {
            seifu = -1;
        }
        //最終的な角度  角度を動かす 角度に正負をかける 
        m_kakudo = kakudo1 * seifu;
        qRot.SetRotation(Vector3::AxisY, m_kakudo - 3.14);
        //回転を加算
        m_rot.Multiply(qRot);
        //モデルを回転させる
        m_modelRender->SetRotation(qRot);

        toNextCell = m_nodeList.front()->m_CenterPos - m_pos;
        float dist = toNextCell.Length();
        if (dist < 5) {
            m_nodeList.erase(m_nodeList.begin());
        }    
        //動いてる。
        m_modelRender->PlayAnimation(0, 0.5f);
    }
    else {
        //動いてない。
        m_modelRender->PlayAnimation(1, 0.5f);
        //更新。
        m_targetPos = m_nextTarget;
        m_dirty = false;
    }
    m_modelRender->SetPosition(m_pos);
}

void Enemy::ForwardRender()
{
}

