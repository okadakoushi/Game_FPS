#include "stdafx.h"
#include <random>
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/RayTestCallBack.h"

bool RifleEnemy::Start()
{
	//エネミーモデル初期化。
	const char* tkaFile[]{
		"Assets/animData/Enemy/Rifle/Walk.tka",
		"Assets/animData/Enemy/Rifle/Damage.tka",
        "Assets/animData/soldier/singleShot.tka"
	};
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
    //ワンショット再生として登録。
    m_modelRender->SetAnimLoop(1, false);
	m_modelRender->Init("Assets/modelData/Chara/soldierMob.tkm", tkaFile);
	m_modelRender->SetMulColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	
    //銃インスタンス化。
	m_rifle = NewGO<Rifle>(EnPriority_3DModel);
	m_rifle->SetRefBoneRender(m_modelRender);
    
    //経路探査の目的地を設定。
    m_targetPos = { 1000.0f, 0.0f, 500.0f };
    m_targetPos = { -1000.0f, 0.0f, -500.0f };
    m_collision.SetPointer(this);
    m_collision.Init(m_modelRender);
    
    //プレイヤーをFind。
    m_player = GetPlayerForUseFind();

    //頭のボーンを取得。
    int headID = m_modelRender->GetSkelton().FindBoneID(L"mixamorig:Head");
    m_head = m_modelRender->GetSkelton().GetBone(headID);
	return true;
}

void RifleEnemy::Update()
{
	//応急処置でswich-case処理が多くなったらステートパターン実装で隠蔽しよう。
	switch (m_enemyState)
	{
	case EnEnemyState_Wandering:
        Move();
        if (Search()) {
            m_modelRender->PlayAnimation(2, 0.1f);
        }
		//警戒状態。
		break;
	case EnEnemyState_Tracking:
		//トラッキング状態。
		break;
	case EnEnemyState_AttackWait:
		//攻撃待ちだぁ。
		break;
	case EnEnemyState_Attack:
		//攻撃！
		break;
    case EnEnemyState_Damage:
        Damage();
        //ダメージ。
        break;
	case EnEnemyState_Death:
		//死亡。
		break;
	}
	m_modelRender->SetPosition(m_pos);
    m_modelRender->SetRotation(m_rot);
	m_modelRender->SetScale(m_scale);
    m_collision.Update();
}
void RifleEnemy::Move()
{
    if (m_dirty == false) {
        //更新必要。
        m_nodeList = m_astar.Search(m_pos, m_targetPos, m_naviMesh->GetCellList());
        NaviMesh::Cell* parentCell = m_nodeCell;
        //NaviMeshObj()->AgentNodeRender(m_nodeList);
        if (m_pathIndex != m_paths.size() - 1) {
            m_pathIndex++;
        }
        else {
            m_pathIndex = 0;
        }
        //次の目的地はパスからランダムで選ぶ。
        m_nextTarget = m_paths[/*rand() % m_paths.size()*/m_pathIndex];
        m_dirty = true;
    }

    //ここから移動処理。
    //次のセルに向かうベクトル。
    if (m_nodeList.size() != 0) {
         m_toNextCell = m_nodeList.front()->m_CenterPos - m_pos;
        //方位化する前に、距離取っとく。
        float dist = m_toNextCell.Length();
        //方位ベクトル化。
        m_toNextCell.Normalize();
        //動かす。
        m_pos += m_toNextCell * m_spped;
        //回転計算。
        m_rot.SetRotation(Vector3::AxisY, atan2f(m_toNextCell.x * 1.0f, m_toNextCell.z * 1.0f));

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

    if (!m_modelRender->isPlayAnim()) {
        m_modelRender->PlayAnimation(0, 0.1f);
    }
}

bool RifleEnemy::Search()
{
    //まずはEnemyの頭のボーンと回転を取得。
    Vector3 headPos;
    Quaternion headRot;
    m_head->CalcWorldTRS(headPos, headRot);
    //Enemyの注視点も設定する。
    Vector3 target = m_toNextCell;
    //m_rot.Apply(target);
    target *= m_VISION;
    target += headPos;

    //コールバック関数。
    RayTestCallBack::EnemyRayTestResult rayTestCB;
    //レイテスト。
    PhysicObj().RayTest(headPos, target, rayTestCB);
    if (rayTestCB.hasHit() && rayTestCB.StaticObjectDist > rayTestCB.CharacterObjectDist) {
        //手前に障害物なしであたってる。
        return true;
    }
    return false;
}



void RifleEnemy::Tracking()
{

}

void RifleEnemy::Damage()
{
    m_modelRender->PlayAnimation(1, 0.1f);
    m_enemyState = EnEnemyState_Wandering;
}

bool RifleEnemy::IsFindPlayer()
{
    //最初はZ軸が前。
    Vector3 enemyForward = Vector3::Back;
    //回転分前方向を回す。
    m_rot.Apply(enemyForward);
    //プレイヤーとの距離測る。
    Vector3 toPlayerDir = m_player->GetPos() - m_pos;
    //距離。
    float toPlayerLen = toPlayerDir.Length();
    //方位ベクトル。
    toPlayerDir.Normalize();
    //前方向とプレイヤーに向かうベクトルの内積。
    float dot = enemyForward.Dot(toPlayerDir);
    //なす角を計算。
    float angle = acos(dot);
    //視野角内に存在するかの判定。
    if (fabsf(angle) < Math::DegToRad(m_FOV) && toPlayerLen < m_VISION) {
        //視野角の範囲内（-+)で視野内。
        //見つけた。
        return true;
    }
    //見つけてない。
    return false;
}



