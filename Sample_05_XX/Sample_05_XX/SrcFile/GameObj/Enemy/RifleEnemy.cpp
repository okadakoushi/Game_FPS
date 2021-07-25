#include "stdafx.h"
#include <random>
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/RayTestCallBack.h"
#include "SrcFile/GameObj/Bullet.h"

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
	return true;
}

void RifleEnemy::Update()
{
    //後々使うのでヘッドのいちを計算しておく。
    Quaternion headRot;
    m_head->CalcWorldTRS(m_headPos, headRot);

	//応急処置でswich-case。処理多くなったらステートパターン実装で隠蔽しよう。
	switch (m_enemyState)
	{
	case EnEnemyState_Wandering:
        //if (m_isFindPlayer) {
        //    printf("トラッキング状態に遷移\n");
        //    m_enemyState = EnEnemyState_Tracking;
        //}
        //else {
        Move();
        //}
        //if (IsFindPlayer()) {
        //    printf("敵を発見。\n");
        //    m_enemyState = EnEnemyState_Attack;
        //}
        //else {
        //    if (m_isFindPlayer) {
        //        m_enemyState = EnEnemyState_Tracking;
        //        //トラッキングアニメーション。
        //    }
        //    else {
        //        //普通に戻す。
        //        m_enemyState = EnEnemyState_Wandering;
        //        m_modelRender->PlayAnimation(0, 0.5f);
        //    }

        //}
		//警戒状態。
		break;
	case EnEnemyState_Tracking:
		//トラッキング状態。
        Tracking();
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
    m_modelRender->SetPosition(m_pos);
    m_agent.Move();
    m_pos = m_agent.GetAgentPos();
    m_rot = m_agent.GetAgentRot();
}

void RifleEnemy::Attack()
{
    //printf("AttackMode\n");

    //プレイヤーを見つけたので次の目的地はプレイヤーの位置に設定。
    m_isFindPlayer = true;
    //m_nextTarget = m_player->GetPos();
    //経路の更新が必要。
    //m_dirty = false;
    //Playerを発見した。
    m_player->GetPlayerUIs()->AddFindList(this);
    if (m_modelRender->GetAnimLoop() || !m_modelRender->isPlayAnim()) {
        //アニメーションがループするものまたは、ループしないものでアニメーションが流れていない。
        //アニメーションを切り替え。
        m_modelRender->PlayAnimation(2, 0.1f);
        //プレイヤーの方向に向ける。
        m_rot.SetRotation(Vector3::AxisY, atan2f(m_toPlayerDir.x * 1.0f, m_toPlayerDir.z * 1.0f));
        if (COOLDOWN < m_currentTime) {
            //レイの長さを調整。
            Vector3 target = m_toPlayerDir;
            target *= m_VISION;
            //エネミーのエイムの精度を調整。
            target = { target.x + rand() % m_currentRondomAIM, target.y + rand() % m_currentRondomAIM, target.z };
            //弾丸発射。
            //Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel);
            //bullet->SetPos(headPos);
            //bullet->SetRot(m_rot);
            //bullet->SetSpeed(110.0f);
            //発砲するたびにEnemyのAIMのランダム値を下げる。
            m_currentRondomAIM--;
            //乱れ値を追加したベクトルに向かう。
            //bullet->SetToTarget(target);
            //弾道がPlayerに当たるかレイテスト。
            RayTestCallBack::EnemyRayTestResult bulletRayCallBuck;
            PhysicObj().RayTest(m_headPos, target + m_headPos, bulletRayCallBuck);
            if (bulletRayCallBuck.hasHit() && bulletRayCallBuck.StaticObjectDist > bulletRayCallBuck.CharacterObjectDist) {
                //プレイヤーに命中。
                m_player->DamageToPlayer(ATTACK);
                //printf("命中。");
                //命中したので再びAIMの質を低品質なものに。
                m_currentRondomAIM = MAX_RANDOM_AIM;
            }
            else {
                //printf("外した。");
            }
            m_currentTime = 0;
        }
    }
}

void RifleEnemy::Tracking()
{
    printf("トラッキング中！！\n");
    //経路を探査して移動。
    //経路に沿って進行中。
    Move();
    //if(m_dirty == true){
    //    //指定地点に到着。
    //    m_enemyState = EnEnemyState_Wandering;
    //    m_isFindPlayer = false;
    //    //もとの経路探査に戻る。
    //    m_nextTarget = m_paths[m_pathIndex];
    //}

}

void RifleEnemy::Damage()
{
    if (m_hp <= 0 && this->IsActive()) {
        //printf("死亡！！\n");
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
    //printf("シニマース。\n");
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
        printf("FindPlayer!\n");
        return true;
    }
    //見つけてない。
    printf("CantFindPlayer\n");
    return false;
}



