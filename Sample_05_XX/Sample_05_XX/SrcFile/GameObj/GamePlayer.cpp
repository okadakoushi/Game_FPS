#include "stdafx.h"
#include "GamePlayer.h"
#include "GameCamera.h"
#include "Bullet.h"
#include "Rifle.h"
#include "Enemy/RifleEnemy.h"
#include "Rifle.h"

const float	GamePlayer::BASE_PARAM::PLAYER_MAX_HP = 150.0f;
const int	GamePlayer::BASE_PARAM::REGENE_VALUE_SECOND = 50;
const float GamePlayer::BASE_PARAM::REGENE_COOL_TIME = 5.0f;
const float GamePlayer::BASE_PARAM::JUMPFORSE = 520.0f;
const float GamePlayer::BASE_PARAM::GRAVITY = 20.0f;


GamePlayer::~GamePlayer()
{
	DeleteGO(m_reticle);
	DeleteGO(m_unityChan);
}

void GamePlayer::Regene()
{
	if (m_hp < BASE_PARAM::PLAYER_MAX_HP) {
		//現在HPがマックスでない。
		m_currentRegeneTime += GameTime().GetFrameDeltaTime();
		if (m_currentRegeneTime >= BASE_PARAM::REGENE_COOL_TIME) {
			//回復開始。
			m_hp += BASE_PARAM::REGENE_VALUE_SECOND * GameTime().GetFrameDeltaTime();
		}
	}
}

void GamePlayer::Init()
{
	m_playerState = EnPlayerState_Idle;
	m_hp = BASE_PARAM::PLAYER_MAX_HP;
	m_wepon->Init();
}

bool GamePlayer::Start()
{	
	//unitychan初期化。
	m_unityChan = NewGO<SkinModelRender>(EnPriority_3DModel, "Unity");
	const char* tkaFilePaths[] = {
		"Assets/animData/Enemy/Rifle/Idle.tka",
		"Assets/animData/Enemy/Rifle/Walk.tka",
		"Assets/animData/Enemy/Rifle/WalkShoot.tka",
		"Assets/animData/Enemy/Rifle/Death.tka"
	};
	m_unityChan->SetAnimLoop(EnPlayerState_Deth, false);
	m_unityChan->Init("Assets/modelData/Chara/Enemy.tkm", tkaFilePaths);
	//m_unityChan->Init("Assets/modeldata/unityChan.tkm", "Assets/animData/unityChan/test.tka");
	//シャドウキャスター。
	m_unityChan->SetShadwoCaster(true);
	m_unityChan->SetShadowReciever(true);
	m_unityChan->SetScale({ 1.5f, 1.5f, 1.5f });

	//SkinModelRenderのアニメーション再生が終わったタイミングで呼ばれる処理を設定。
	m_unityChan->SetPostAnimationProgressFunc([&] {	OnPostAnimationProgress(); });
	//位置初期化。
	m_pos = m_unityChan->GetPosition();
	//キャラコン初期化。
	m_cCon.Init(25.0f, 60.0f, m_pos);
	//腰のボーン取得。
	int spineBoneNo = m_unityChan->GetSkelton().FindBoneID(L"mixamorig:Spine");
	m_spineBone = m_unityChan->GetSkelton().GetBone(spineBoneNo);
	//頭のボーン取得。
	int headNo = m_unityChan->GetSkelton().FindBoneID(L"mixamorig:Head");;
	m_headBone = m_unityChan->GetSkelton().GetBone(headNo);
	//武器。
	m_wepon = NewGO<Rifle>(EnPriority_3DModel, "Wepon");
	m_wepon->SetRefBoneRender(m_unityChan);	

	//カメラFind。
	m_camera = FindGO<GameCamera>("GameCamera");

	//UIs。
	m_playerUIs = NewGO<PlayerUIs>(EnPriority_UI);

	//エフェクト生成。
	m_effect = NewGO<myEngine::Effect>(EnPriority_3DModel);
	m_effect->SetScale({ 3.0f, 3.0f, 3.0f });

	//サウンド初期化。
	m_shootSE.Init(L"Assets/Audio/AK47Shoot.wav");
	m_footStepSE.Init(L"Assets/Audio/footstep.wav");

	m_pos = { 0.0f, 0.0f, 0.0f };

	return true;
}

void GamePlayer::Update()
{
	if (m_playerState != EnPlayerState_Deth) {
		//死んでない。
		//移動。
		Move();
		//回転。
		//Rotation();
		//射撃。
		Shot();
		//リジェネ。
		Regene();
		m_unityChan->SetPosition(m_pos);
	}

	switch (m_playerState)
	{
	case EnPlayerState_Idle:
		m_unityChan->PlayAnimation(EnPlayerState_Idle, 0.3f);
		break;
	case EnPlayerState_Walk:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Run:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Buck:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Shot:
		m_unityChan->PlayAnimation(EnPlayerState_Shot, 0.3f);
		break;
	case EnPlayerState_Reload:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Deth:
		m_unityChan->PlayAnimation(EnPlayerState_Deth, 0.5f);
		//m_camera->SetActive(false);
		break;
	case EnPlayerState_Num:
		break;
	}
	
	if (m_hp <= 0) {
		m_playerState = EnPlayerState_Deth;
	}
}

Vector3& GamePlayer::CalcHeadPos()
{
	Quaternion a;
	m_headBone->CalcWorldTRS(m_headPos, a);
	m_headPos.y += 4.0f;
	m_camera->SetEyePos(m_headPos);
	return m_headPos;
}

void GamePlayer::OnPostAnimationProgress()
{
	Quaternion qRot;
	//X軸周りの回転を計算。
	Vector3 to = GraphicsEngineObj()->GetCamera3D().GetForward();
	to.Normalize();
	//腰への回転は少し強めに取る。
	qRot.SetRotation(g_vec3AxisX, atan2f(to.y * -1.2f, g_vec3Front.z));
	Matrix local = m_spineBone->GetLocalMatrix();

	Matrix trans;
	trans.MakeRotationFromQuaternion(qRot);
	local *= trans;
	//腰のボーンに回転を適用させる。
	m_spineBone->SetLocalMatrixFromUser(local);
}
void GamePlayer::Rotation()
{	
	Vector3 toPos = GraphicsEngineObj()->GetCamera3D().GetTarget() - GraphicsEngineObj()->GetCamera3D().GetPosition();
	toPos.Normalize();
	float angle = atan2(toPos.x, toPos.z);
	m_rot.SetRotation(g_vec3AxisY, angle);
	m_unityChan->SetRotation(m_rot);
}

void GamePlayer::Shot()
{
	//レイのコールバック。
	RayTestCallBack::PlayerRayTestResult rayCallBack;
	//レイテストを行う。
	Vector3 toDir = GraphicsEngineObj()->GetCamera3D().GetTarget() - m_headPos;
	toDir.Normalize();
	toDir *= RAY_RANGE;
	PhysicObj().RayTest(m_headPos, toDir + m_headPos, rayCallBack);

	m_flame++;
	if (GetAsyncKeyState(MK_LBUTTON)) {
		m_playerState = EnPlayerState_Shot;
		if (m_flame >= 20) {
			if (m_wepon->GetRifleEvent() == Rifle::EnRifleEvent_None) {
				m_shootSE.Stop();
				m_shootSE.Play(false);
				m_wepon->ReduseAmo();
				//Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel, "Bullet");
				//bullet->SetPos(m_wepon->GetPos());
				//bullet->SetRot(m_rot);
				//bullet->SetToTarget(toDir);
				m_flame = 0;
				//printf("StaticObjectDist = %f\n", rayCallBack.StaticObjectDist);
				//printf("CharacterObjectDist = %f\n", rayCallBack.CharacterObjectDist);
				if (rayCallBack.hasHit() && rayCallBack.StaticObjectDist > rayCallBack.CharacterObjectDist) {
					//敵にレイが命中。
					//生ポインタから敵に強制キャスト。
					RifleEnemy* enemy = reinterpret_cast<RifleEnemy*>(rayCallBack.m_collisionObject->getUserPointer());
					if (enemy != nullptr) {
						enemy->GetDamage();
					}
				}
				else if (rayCallBack.hasHit() && rayCallBack.StaticObjectDist < rayCallBack.CharacterObjectDist) {
					//なんかのメッシュに命中。
					//命中した地点からエフェクトを再生。
					Vector3 effectPos = m_headPos + toDir * rayCallBack.StaticObjectDist;
					Quaternion effectRot;
					effectRot.SetRotation(g_vec3AxisY, atan2f(g_vec3Front.z, toDir.x));
					m_effect->SetPosition(effectPos);
					m_effect->SetRotation(effectRot);
					//Y成分はいらない。
					//printf("%f\n", rayCallBack.StaticObjectDist);
					//printf("%f, %f, %f\n", toDir.x /* rayCallBack.StaticObjectDist*/, toDir.y /* rayCallBack.StaticObjectDist*/, toDir.z /* rayCallBack.StaticObjectDist*/);
					m_effect->Play(L"Assets/effect/aaaa.efk");
				}
				else if (!rayCallBack.hasHit()) {
					//printf("%f\n", rayCallBack.StaticObjectDist);
				}
			}
			else {
				m_playerState = EnPlayerState_Reload;
			}
		}
	}
}

void GamePlayer::Reload()
{
	if (m_wepon->GetRifleEvent() == Rifle::EnRifleEvent_Reloading) {
		m_wepon->AddReloadTime();
	}

}

void GamePlayer::Move()
{
	float deltaTime = GameTime().GetFrameDeltaTime();
	//カメラの向き情報。
	Vector3 camForward = GraphicsEngineObj()->GetCamera3D().GetForward();
	Vector3 camRight = GraphicsEngineObj()->GetCamera3D().GetRight();
	camForward.y = 0.0f;
	camForward.Normalize();
	camRight.y = 0.0f;
	camRight.Normalize();
	
	//加速度。
	Vector3 acc;

	//移動処理。
	//todo:Pad対応？キャラコン対応？慣性？
	if (GetAsyncKeyState('W')) {
		m_playerState = EnPlayerState_Walk;
		acc += camForward * m_speed;
	}
	if (GetAsyncKeyState('S')) {
		m_playerState = EnPlayerState_Buck;
		acc -= camForward * m_speed;
	}
	if (GetAsyncKeyState('D')) {
		m_playerState = EnPlayerState_Walk;
		acc += camRight * m_speed;
	}
	if (GetAsyncKeyState('A')) {
		m_playerState = EnPlayerState_Walk;
		acc -= camRight * m_speed;
	}
	if (GetAsyncKeyState('R')) {
		m_playerState = EnPlayerState_Reload;
		m_wepon->SetRifleEvent(Rifle::EnRifleEvent_Reloading);
	}
	if (GetAsyncKeyState(VK_SHIFT) && m_playerState != EnPlayerState_Shot && m_playerState != EnPlayerState_Buck) {
		//ダッシュ。
		acc *= 3.0f;
	}
	if (m_cCon.IsOnGround()) {
		if (GetAsyncKeyState(VK_SPACE)) {
			//ジャンプ。
			acc += {0, BASE_PARAM::JUMPFORSE, 0};
		}
	}
	else {
		//重力。
#ifdef MASTER
		acc += {0, -BASE_PARAM::GRAVITY, 0};
#endif
	}



	m_move += acc * 2.0;

	if (m_move.Length() >= 300.0f && m_playerState != EnPlayerState_Buck) {
		m_playerState = EnPlayerState_Walk;
	}
	if (m_move.Length() >= 700.0f) {
		m_playerState = EnPlayerState_Run;
	}
	if(m_move.Length() == 0.0f){
		m_playerState = EnPlayerState_Idle;
	}

	if (m_move.Length() != 0) {
		//printf("%f, %f, %f \n", m_pos.x,m_pos.y, m_pos.z);
	}


	//摩擦。
	m_move.x += m_move.x * -0.5f;
	m_move.z += m_move.z * -0.5f;
	m_move.y += m_move.y * -0.025f;
	m_pos = m_cCon.Execute(m_move);
}

