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
    DeleteGO(m_findMark); 
    
    delete m_attackState;
    delete m_damageState;
    delete m_deathState;
    delete m_trackingState;
    delete m_wanderingState;
}

bool RifleEnemy::Start()
{
	//エネミーモデル初期化。
	const char* tkaFile[]{
		"Assets/animData/Enemy/Rifle/Walk.tka",
        "Assets/animData/Enemy/Rifle/Run.tka",
		"Assets/animData/Enemy/Rifle/Damage.tka",
        "Assets/animData/Enemy/Rifle/Shoot.tka",
        "Assets/animData/Enemy/Rifle/Death.tka"
	};
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
    //ワンショット再生として登録。ここtodo:
    m_modelRender->SetAnimLoop(EnEnemyAnimation_Walk, true);
    m_modelRender->SetAnimLoop(EnEnemyAnimation_Run, true);
    m_modelRender->SetAnimLoop(EnEnemyAnimation_Damage, false);
    m_modelRender->SetAnimLoop(EnEnemyAnimation_Shoot, true);
    m_modelRender->SetAnimLoop(EnEnemyAnimation_Death, false);
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

    //SE初期化。
    m_findSE = NewGO<SoundSource>(0);
    m_findSE->Init(L"Assets/Audio/find.wav");
    m_footStep = NewGO<SoundSource>(0);
    m_footStep->Init(L"Assets/Audio/footstep.wav", true);
    m_footStep->SetPosition(m_pos);
    m_footStep->Play(true);

    //マーク初期化。
    SpriteInitData initData;
    initData.m_ddsFilePath[0] = "Assets/sprite/findSprite.dds";
    initData.m_fxFilePath = "Assets/shader/sprite.fx";
    initData.m_width = 50.0f;
    initData.m_height = 50.0f;
    initData.renderTargetMask = D3D12_COLOR_WRITE_ENABLE_RED
                                | D3D12_COLOR_WRITE_ENABLE_GREEN
                                | D3D12_COLOR_WRITE_ENABLE_BLUE;

    m_findMark = NewGO<SpriteRender>(EnPriority_UI);
    m_findMark->Init(initData, true);
    m_findMark->SetPos({ 0, 100, 0 });
    m_findMark->SetMulColor(Vector4::Transparent);

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
        //更新止め、コリジョン解放。
        this->SetActive(false);
        m_findMark->SetMulColor(Vector4::Transparent);
        m_footStep->Stop();
        //もう死んでるので更新は行わない。
        return;
    }

    //後々使うのでヘッドのいちを計算しておく。
    Quaternion headRot;
    m_head->CalcWorldTRS(m_headPos, headRot);

    if (IsFindPlayer()) {
        //攻撃。
        m_footStep->Stop();
        ChangeState(m_attackState);
        if (!m_isMissingPlayer) {
            //初めてプレイヤーを見つけた。
            m_findSE->Play(false);
            m_findMark->SetMulColor(Vector4::White);
            m_isMissingPlayer = true;
        }

    }
    else if (m_isMissingPlayer) {
        //警戒状態。プレイヤーがいそうな地点に移動。
        ChangeState(m_trackingState);
        m_FOV = 90.0f;
        m_vision = 1100.0f;
        m_findMark->SetMulColor(Vector4::Transparent);
    }
    else {
        //非警戒状態。
        m_FOV = 40.0f;
        m_vision = 700.0f;
        ChangeState(m_wanderingState);
    }

    if (m_hp <= 0) {
        //死んでる。
        ChangeState(m_deathState);
    }

    m_enemyState->Update();

    //レンダーに適用。
	m_modelRender->SetPosition(m_pos);
    m_modelRender->SetRotation(m_rot);
	m_modelRender->SetScale(m_scale);

    //マークのtransform設定。
    m_findMark->SetPos({ m_pos.x, m_pos.y + 120.0f, m_pos.z });
    //回転。
    Quaternion markRot;
    Vector3 cameraPos = GraphicsEngineObj()->GetCamera3D().GetPosition();
    cameraPos.y = 0.0f;
    Vector3 markPos = m_pos;
    markPos.y = 0.0f;
    Vector3 ToCamera = markPos - cameraPos;
    ToCamera.Normalize();
    ToCamera *= -1.0f;
    markRot.SetRotation(g_vec3Front, ToCamera);
    m_findMark->SetRotation(markRot);

    m_collision.Update();
    m_currentTime += GameTime().GetFrameDeltaTime();
}

void RifleEnemy::Move()
{
    m_modelRender->SetPosition(m_pos);
    m_agent.Move();
    m_pos = m_agent.GetAgentPos();
    m_rot = m_agent.GetAgentRot();
    m_footStep->SetPosition(m_pos);
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
    float dot = m_agent.GetMoveDirection().Dot(m_toPlayerDir);
    //なす角を計算。
    float angle = acos(dot);
    //視野角内に存在するかの判定。
    if (fabsf(angle) < Math::DegToRad(m_FOV) && toPlayerLen < m_vision) {
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
    target *= m_vision;
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
    //if (m_modelRender->GetAnimLoop() || !m_modelRender->isPlayAnim()) {
        //ループしないアニメーションはしっかり流してから変えさせる。
        if (m_enemyState != nullptr) {
            m_enemyState->Leave();
        }
        m_enemyState = state;
        m_enemyState->Enter();
    //}
}

void RifleEnemy::GetDamage()
{
    ChangeState(m_damageState);
}

GamePlayer* RifleEnemy::GetPlayerForUseFind() const
{
    return FindGO<GamePlayer>("Player");
}



