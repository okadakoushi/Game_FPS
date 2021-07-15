#include "stdafx.h"
#include <random>
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/RayTestCallBack.h"
#include "SrcFile/GameObj/Bullet.h"

bool RifleEnemy::Start()
{
	//エネミーモデル初期化。
	const char* tkaFile[]{
		"Assets/animData/Enemy/Rifle/Walk.tka",
		"Assets/animData/Enemy/Rifle/Damage.tka",
        "Assets/animData/Enemy/Rifle/Shoot.tka",
        "Assets/animData/Enemy/Rifle/Death.tka"
	};
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
    //ワンショット再生として登録。ここtodo:
    m_modelRender->SetAnimLoop(0, true);
    m_modelRender->SetAnimLoop(1, false);
    m_modelRender->SetAnimLoop(2, true);
    m_modelRender->SetAnimLoop(3, false);
	m_modelRender->Init("Assets/modelData/Chara/Enemy.tkm", tkaFile);
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
        if (IsFindPlayer()) {
            m_enemyState = EnEnemyState_Attack;
            //m_enemyState = EnEnemyState_Tracking;
        }
		//警戒状態。
		break;
	case EnEnemyState_Tracking:
		//トラッキング状態。
		break;
	case EnEnemyState_AttackWait:
		//リロードとかの予定だが...
		break;
	case EnEnemyState_Attack:
        Attack();
		//攻撃！
		break;
    case EnEnemyState_Damage:
        Damage();
        //ダメージ。
        break;
	case EnEnemyState_Death:
		//死亡。
        Death();
		break;
	}
	m_modelRender->SetPosition(m_pos);
    m_modelRender->SetRotation(m_rot);
	m_modelRender->SetScale(m_scale);
    m_collision.Update();
    m_currentTime += GameTime().GetFrameDeltaTime();
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

void RifleEnemy::Attack()
{
    //printf("AttackMode\n");

    //まずはEnemyの頭のボーンと回転を取得。
    Vector3 headPos;
    Quaternion headRot;
    m_head->CalcWorldTRS(headPos, headRot);

    //プレイヤーへ伸びる方位ベクトル。
    Vector3 toPlayerDir = m_player->GetPos() - m_pos;
    //プレイヤーに向かうエネミーのエイム。
    //Vector3 EnemyAIM_to_Player = { toPlayerDir.x + rand() % m_currentRondomAIM, toPlayerDir.y + rand() % m_currentRondomAIM, toPlayerDir.z };
    toPlayerDir.Normalize();
    //EnemyAIM_to_Player.Normalize();
    //今回のターゲットはプレイヤーに伸びるベクトル。
    Vector3 target = toPlayerDir;
    target *= m_VISION;

    
    //視野判定用コールバック関数。
    RayTestCallBack::EnemyRayTestResult visionCallBuck;
    //レイテスト。
    PhysicObj().RayTest(headPos, target + headPos, visionCallBuck);
    if (visionCallBuck.hasHit() && visionCallBuck.StaticObjectDist > visionCallBuck.CharacterObjectDist) {
        //手前に障害物なし！あたってる。
        if (m_modelRender->GetAnimLoop() || !m_modelRender->isPlayAnim()) {
            //アニメーションを切り替え。
            m_modelRender->PlayAnimation(2, 0.1f);
            //プレイヤーの方向に向ける。
            m_rot.SetRotation(Vector3::AxisY, atan2f(toPlayerDir.x * 1.0f, toPlayerDir.z * 1.0f));
            if (COOLDOWN < m_currentTime) {
                //エネミーのエイムの精度を調整。
                target = { target.x + rand() % m_currentRondomAIM, target.y + rand() % m_currentRondomAIM, target.z };
                //弾丸発射。
                Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel);
                bullet->SetPos(headPos);
                bullet->SetRot(m_rot);
                bullet->SetSpeed(110.0f);
                //発砲するたびにEnemyのAIMのランダム値を下げる。
                m_currentRondomAIM--;
                //乱れ値を追加したベクトルに向かう。
                bullet->SetToTarget(target);
                //弾道がPlayerに当たるかレイテスト。
                RayTestCallBack::EnemyRayTestResult bulletRayCallBuck;
                PhysicObj().RayTest(headPos, target + headPos, bulletRayCallBuck);
                if (bulletRayCallBuck.hasHit() && bulletRayCallBuck.StaticObjectDist > bulletRayCallBuck.CharacterObjectDist) {
                    //プレイヤーに命中。
                    printf("命中。");
                    //命中したので再びAIMの質を低品質なものに。
                    m_currentRondomAIM = MAX_RANDOM_AIM;
                }
                else {
                    printf("外した。");
                }
                m_currentTime = 0;
            }
        }
    }
    else {
        //手前にオブジェクトがある or 何にもあたっていない。
        m_enemyState = EnEnemyState_Wandering;
        m_modelRender->PlayAnimation(0, 0.5f);
    }
}

void RifleEnemy::Tracking()
{
    //まずはEnemyの頭のボーンと回転を取得。
    Vector3 headPos;
    Quaternion headRot;
    m_head->CalcWorldTRS(headPos, headRot);
    //Enemyの注視点も計算する。
    Vector3 target = m_toNextCell;
    //m_rot.Apply(target);
    target *= m_VISION;
    target += headPos;

    ////コールバック関数。
    //RayTestCallBack::EnemyRayTestResult rayTestCB;
    ////レイテスト。
    //PhysicObj().RayTest(headPos, target, rayTestCB);
    //if (rayTestCB.hasHit() && rayTestCB.StaticObjectDist > rayTestCB.CharacterObjectDist) {
    //    //手前に障害物なしであたってる。
    //    return true;
    //}
}

void RifleEnemy::Damage()
{
    //printf("hp = %d", m_hp);
    if (m_hp <= 0 && this->IsActive()) {
        printf("死亡！！\n");
        m_enemyState = EnEnemyState_Death;
    }
    else {
        m_modelRender->PlayAnimation(1, 0.1f);
        m_enemyState = EnEnemyState_Attack;
        //とりあえず固定値25。
        m_hp -= 25;
    }
}

void RifleEnemy::Death()
{
    printf("シニマース。\n");
    m_modelRender->PlayAnimation(3, 1.0f);
    if (!m_modelRender->isPlayAnim()) {
        //更新止め、コリジョン解放。
        this->SetActive(false);
        m_collision.Release();
    }
}

bool RifleEnemy::IsFindPlayer()
{
    //プレイヤーとの距離測る。
    Vector3 toPlayerDir = m_player->GetPos() - m_pos;
    //距離。
    float toPlayerLen = toPlayerDir.Length();
    //方位ベクトル。
    toPlayerDir.Normalize();
    //前方向とプレイヤーに向かうベクトルの内積。
    float dot = m_toNextCell.Dot(toPlayerDir);
    //なす角を計算。
    float angle = acos(dot);
    //視野角内に存在するかの判定。
    if (fabsf(angle) < Math::DegToRad(m_FOV) && toPlayerLen < m_VISION) {
        //視野角の範囲内（-+)で視野内。
        return true;
    }
    //見つけてない。
    return false;
}



