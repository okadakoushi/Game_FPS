#include "stdafx.h"
#include <random>
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/RayTestCallBack.h"
#include "SrcFile/GameObj/Bullet.h"

#include "EnemyState/IEnemyState.h"
#include "EnemyState/EnemyAttackState.h"
#include "EnemyState/EnemyDamageState.h"
#include "EnemyState/EnemyDethState.h"
#include "EnemyState/EnemyTrackingState.h"
#include "EnemyState/EnemyWanderingState.h"

RifleEnemy::~RifleEnemy()
{
    DeleteGO(m_modelRender);
    DeleteGO(m_rifle);
}

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
	m_rifle = NewGO<Rifle>(EnPriority_3DModel, "Rifle");
	m_rifle->SetRefBoneRender(m_modelRender);
    
    //エージェント初期化。
    m_agent.Init(m_modelRender, m_naviMesh, true);

    m_collision.SetPointer(this);
    m_collision.Init(m_modelRender);
    
    //プレイヤーをFind。
    m_player = GetPlayerForUseFind();

    //頭のボーンを取得。
    int headID = m_modelRender->GetSkelton().FindBoneID(L"mixamorig:Head");
    m_head = m_modelRender->GetSkelton().GetBone(headID);

    //ステートを生成。
    m_attackState = new EnemyAttackState(this);
    m_damageState = new EnemyDamageState(this);
    m_deathState = new EnemyDethState(this);
    m_trackingState = new EnemyTrackingState(this);
    m_wanderingState = new EnemyWanderingState(this);

	return true;
}

void RifleEnemy::Update()
{
    if (m_enemyState == m_deathState) {
        //もう死んでるので更新は行わない。
        return;
    }

    //後々使うのでヘッドのいちを計算しておく。
    Quaternion headRot;
    m_head->CalcWorldTRS(m_headPos, headRot);

    ChangeState(m_wanderingState);
    if (IsFindPlayer()) {
        //攻撃。
        ChangeState(m_attackState);
        //プレイヤーを見失った。
        m_isMissingPlayer = true;
    }
    else if (m_isMissingPlayer) {
        //見失ったので、プレイヤーがいたであろう地点まで追跡する。
        ChangeState(m_trackingState);
    }

    if (m_hp <= 0) {
        //死んでる。
        ChangeState(m_deathState);
    }

    m_enemyState->Update();

	m_modelRender->SetPosition(m_pos);
    m_modelRender->SetRotation(m_rot);
	m_modelRender->SetScale(m_scale);
    m_collision.Update();
    m_currentTime += GameTime().GetFrameDeltaTime();
}

void RifleEnemy::Move()
{
    m_modelRender->SetPosition(m_pos);
    m_agent.Move();
    m_pos = m_agent.GetAgentPos();
    m_rot = m_agent.GetAgentRot();
}

bool RifleEnemy::IsFindPlayer()
{
    //プレイヤーとの距離測る。
    m_toPlayerDir = m_player->GetPos() - m_pos;
    //距離。
    float toPlayerLen = m_toPlayerDir.Length();
    //方位ベクトル。
    m_toPlayerDir.Normalize();
    //前方向とプレイヤーに向かうベクトルの内積。
    float dot = m_agent.MoveDirection().Dot(m_toPlayerDir);
    //なす角を計算。
    float angle = acos(dot);
    //視野角内に存在するかの判定。
    if (fabsf(angle) < Math::DegToRad(m_FOV) && toPlayerLen < m_VISION) {
        //視野角の範囲内（-+)で視野内。
    }
    else {
        //見つけてない。
        return false;
    }

    //手前にオブジェクトが存在しないかを判定。
    //視野判定用コールバック関数。
    RayTestCallBack::EnemyRayTestResult visionCallBuck;

    //プレイヤーへ伸びる方位ベクトル。
    m_toPlayerDir = m_player->GetPos() - m_pos;
    //プレイヤーに向かうエネミーのエイム。
    //Vector3 EnemyAIM_to_Player = { toPlayerDir.x + rand() % m_currentRondomAIM, toPlayerDir.y + rand() % m_currentRondomAIM, toPlayerDir.z };
    m_toPlayerDir.Normalize();
    //EnemyAIM_to_Player.Normalize();
    //今回のターゲットはプレイヤーに伸びるベクトル。
    Vector3 target = m_toPlayerDir;
    target *= m_VISION;
    //レイテスト。
    PhysicObj().RayTest(m_headPos, target + m_headPos, visionCallBuck);
    if (visionCallBuck.hasHit() && visionCallBuck.StaticObjectDist > visionCallBuck.CharacterObjectDist) {
        //あたっている。
        //printf("FindPlayer!\n");
        return true;
    }
    //見つけてない。
    //printf("CantFindPlayer\n");
    return false;
}

void RifleEnemy::ChangeState(IEnemyState* state)
{
    if (m_modelRender->GetAnimLoop() || !m_modelRender->isPlayAnim()) {
        //ループしないアニメーションはしっかり流してから変えさせる。
        if (m_enemyState != nullptr) {
            m_enemyState->Leave();
        }
        m_enemyState = state;
        m_enemyState->Enter();
    }
}

void RifleEnemy::GetDamage()
{
    ChangeState(m_damageState);
}

GamePlayer* RifleEnemy::GetPlayerForUseFind() const
{
    return FindGO<GamePlayer>("Player");
}



