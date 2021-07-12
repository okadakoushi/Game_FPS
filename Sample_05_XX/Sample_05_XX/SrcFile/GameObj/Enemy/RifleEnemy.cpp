#include "stdafx.h"
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"

bool RifleEnemy::Start()
{
	//エネミーモデル初期化。
	const char* tkaFile[]{
		"Assets/animData/Enemy/Rifle/Walk.tka"
	};
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_modelRender->Init("Assets/modelData/Chara/soldierMob.tkm", tkaFile);
	m_modelRender->SetMulColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	//銃インスタンス化。
	m_rifle = NewGO<Rifle>(EnPriority_3DModel);
	m_rifle->SetRefBoneRender(m_modelRender);
    //経路探査の目的地を設定。
    m_targetPos = { 1000.0f, 0.0f, 500.0f };
    m_targetPos = { -1000.0f, 0.0f, -500.0f };
	return true;
}

void RifleEnemy::Update()
{
	//応急処置でswich-case処理が多くなったらステートパターン実装で隠蔽しよう。
	switch (m_enemyState)
	{
	case Enemy::EnEnemyState_Wandering:
        Move();
		//警戒状態。
		break;
	case Enemy::EnEnemyState_Tracking:
		//トラッキング状態。
		break;
	case Enemy::EnEnemyState_AttackWait:
		//攻撃待ちだぁ。
		break;
	case Enemy::EnEnemyState_Attack:
		//攻撃！
		break;
	case Enemy::EnEnemyState_Death:
		//死亡。
		break;
	}
	m_modelRender->SetPosition(m_pos);
    m_modelRender->SetRotation(m_rot);
	m_modelRender->SetScale(m_scale);
}

void RifleEnemy::Move()
{
    if (m_dirty == false) {
        //更新必要。
        m_nodeList = m_astar.Search(m_pos, m_targetPos, m_naviMesh->GetCellList());
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
        //方位化する前に、距離取っとく。
        float dist = toNextCell.Length();
        //方位ベクトル化。
        toNextCell.Normalize();
        //動かす。
        m_pos += toNextCell * m_spped;
        //回転計算。
        m_rot.SetRotation(Vector3::AxisY, atan2f(toNextCell.x * 1.0f, toNextCell.z * 1.0f));

        //ノードに到達したか判定。
        if (dist < 5) {
            m_nodeList.erase(m_nodeList.begin());
        }
        //動いてる。
        //m_modelRender->PlayAnimation(0, 0.5f);
    }
    else {
        //動いてない。
        //m_modelRender->PlayAnimation(1, 0.5f);
        //更新。
        m_targetPos = m_nextTarget;
        m_dirty = false;
    }
}

