#include "stdafx.h"
#include "GamePlayer.h"
#include "GameCamera.h"
#include "Bullet.h"
#include "Rifle.h"
#include "Enemy/RifleEnemy.h"
#include "Rifle.h"

#include "PlayerState/IPlayerState.h"
#include "PlayerState/PlayerIdleState.h"
#include "PlayerState/PlayerAttackState.h"
#include "PlayerState/PlayerDeathState.h"
#include "PlayerState/PlayerWalkState.h"
#include "PlayerState/PlayerDashState.h"
#include "PlayerState/PlayerReloadState.h"

const float	GamePlayer::BASE_PARAM::PLAYER_MAX_HP = 100.0f;
const int	GamePlayer::BASE_PARAM::REGENE_VALUE_SECOND = 50;
const float GamePlayer::BASE_PARAM::REGENE_COOL_TIME = 5.0f;
const float GamePlayer::BASE_PARAM::JUMPFORSE = 520.0f;
const float GamePlayer::BASE_PARAM::GRAVITY = 20.0f;


GamePlayer::~GamePlayer()
{
	DeleteGO(m_reticle);
	DeleteGO(m_unityChan);
	DeleteGO(m_footStepSE);
	DeleteGO(m_beHitSE);
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

void GamePlayer::ChangeState(IPlayerState* state)
{
	if (m_currentState != nullptr) {
		m_currentState->Leave();
	}
	m_currentState = state;

	if (state != nullptr) {
		m_currentState->Enter();
	}
}

void GamePlayer::Init()
{
	ChangeState(m_playerIdleState);
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
	m_unityChan->SetAnimLoop(EnPlayerAnimation_Death, false);
	m_unityChan->Init("Assets/modelData/Chara/Enemy.tkm", tkaFilePaths);
	//m_unityChan->Init("Assets/modeldata/unityChan.tkm", "Assets/animData/unityChan/test.tka");
	//シャドウキャスター。
	m_unityChan->SetShadwoCaster(true);
	m_unityChan->SetShadowReciever(true);
	m_unityChan->SetScale({ 1.5f, 1.5f, 1.5f });

	//SkinModelRenderのアニメーション再生が終わったタイミングで呼ばれる処理を設定。
	m_unityChan->SetPostAnimationProgressFunc([&] {	OnPostAnimationProgress(); });
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

	//サウンド初期化。
	m_footStepSE = NewGO<SoundSource>(0);
	m_footStepSE->Init(L"Assets/Audio/footstep.wav");
	m_beHitSE = NewGO<SoundSource>(0);
	m_beHitSE->Init(L"Assets/Audio/shootBlood.wav");

	//ステート初期化。
	m_playerIdleState = new PlayerIdleState(this);
	m_attackState = new PlayerAttackState(this);
	m_deathState = new PlayerDeathState(this);
	m_reloadState = new PlayerReloadState(this);

	m_unityChan->SetPosition(m_pos);

	return true;
}

void GamePlayer::Update()
{
	if (m_currentState == m_deathState) {
		//死んでる場合は更新を行わない。
		return;
	}

	//タイマーを加算。
	m_weponRaitTime += GameTime().GetFrameDeltaTime();

	m_damageReactionTime += GameTime().GetFrameDeltaTime();
	if (m_damageReactionTime >= 1.5f || m_damageEffectValue <= 1.0f) {
		m_damageEffectValue += GameTime().GetFrameDeltaTime() * 0.6f;
	}
	else if (m_damageEffectValue >= 1.0f) {
		m_damageReactionTime = 0.0f;
	}
	GraphicsEngineObj()->GetDefferd().GetDefferdSprite().SetMulColor({ 1.0f, m_damageEffectValue, m_damageEffectValue, 1.0f });

	//リジェネ。
	Regene();
	//ステートをなにもない状態に初期化。
	ChangeState(nullptr);

	Move();

	if (GetWepon()->GetRifleEvent() == Rifle::EnRifleEvent_None) {
		if (GetAsyncKeyState(MK_LBUTTON)) {
			if (m_weponRaitTime >= m_wepon->GetWeponRaito()) {
				//リロード、レイトともにOK。発砲する。
				ChangeState(m_attackState);
				//発射したのでタイマーリセット。
				m_weponRaitTime = 0;
			}
		}
	}
	else {
		//残弾なし、またはリロード中。
		ChangeState(m_reloadState);
	}

	if (GetAsyncKeyState('R')) {
		//リロード。
		ChangeState(m_reloadState);
	}

	if (m_hp <= 0) {
		//死亡。
		ChangeState(m_deathState);
	}

	//現在のステートを更新。
	if (m_currentState != nullptr) {
		m_currentState->Update();
	}

	if (GetAsyncKeyState(VK_BACK)) {
		m_pos = g_vec3Zero;
	}

	m_unityChan->SetPosition(m_pos);
	//リスナーの設定。
	SoundEngineObj().SetListnerPosition(m_pos);
}

void GamePlayer::DamageToPlayer(const int& damage)
{
	m_currentRegeneTime = 0.0f;
	if (m_hp > 0) {
		m_hp -= damage;
	}
	m_damageEffectValue = 0.7f;
	m_damageReactionTime = 0.0f;
	m_beHitSE->Stop();
	m_beHitSE->Play(false);
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

	if (GetAsyncKeyState('W')) {
		acc += camForward * m_speed;
	}
	if (GetAsyncKeyState('S')) {
		acc -= camForward * m_speed;
	}
	if (GetAsyncKeyState('D')) {
		acc += camRight * m_speed;
	}
	if (GetAsyncKeyState('A')) {
		acc -= camRight * m_speed;
	}
	if (GetAsyncKeyState(VK_SHIFT) && m_currentState == nullptr) {
		//プレイヤーの状態に何か入ってる場合は走れない。
		acc *= 2.5f;
	}

	if (m_cCon.IsOnGround()) {
		if (GetAsyncKeyState(VK_SPACE)) {
			//ジャンプ。
			acc += {0, BASE_PARAM::JUMPFORSE, 0};
		}
	}
	else {
		//重力。
#ifdef GAME
		acc += {0, -BASE_PARAM::GRAVITY, 0};
#endif
	}

	m_move += acc * 2.0;

	if (m_move.Length() >= 300.0f) {
		m_footStepSE->Play(true);
		m_unityChan->PlayAnimation(EnPlayerAnimation_Walk, 0.5f);
	}
	if (m_move.Length() >= 700.0f) {
		m_footStepSE->Play(true);
		m_unityChan->PlayAnimation(EnPlayerAnimation_Walk, 0.5f);
	}
	if (m_move.Length() <= 0.99999f) {
		ChangeState(m_playerIdleState);
		m_footStepSE->Stop();
	}
	if (m_move.Length() != 0) {
		//printf("%f, %f, %f \n", m_move.x, m_move.y, m_move.z);
	}

	//摩擦。
	m_move.x += m_move.x * -0.5f;
	m_move.z += m_move.z * -0.5f;
	m_move.y += m_move.y * -0.025f;
	m_pos = m_cCon.Execute(m_move);
}

