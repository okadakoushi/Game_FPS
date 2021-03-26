#include "stdafx.h"
#include "GamePlayer.h"
#include "GameCamera.h"
#include "Bullet.h"
#include "Rifle.h"

void GamePlayer::OnDestroy()
{
	DeleteGO(m_reticle);
	DeleteGO(m_camera);
	DeleteGO(m_unityChan);
}

bool GamePlayer::Start()
{	
	//unitychan初期化。
	m_unityChan = NewGO<SkinModelRender>(EnPriority_3DRender, "Unity");
	const char* tkaFilePaths[] = {
		"Assets/animData/soldier/idle.tka",
		"Assets/animData/soldier/walk.tka",
		"Assets/animData/soldier/run.tka",
		"Assets/animData/soldier/buck.tka",
		"Assets/animData/soldier/singleShot.tka"
	};
	m_unityChan->Init("Assets/modelData/Chara/soldierMob.tkm", tkaFilePaths);
	//m_unityChan->Init("Assets/modeldata/unityChan.tkm", "Assets/animData/unityChan/test.tka");
	//シャドウキャスター。
	m_unityChan->SetShadwoCaster(true);
	m_unityChan->SetShadowReciever(true);
	//スキン描画。
	m_unityChan->SetRenderMode(enRenderMode_Skin);
	m_unityChan->SetScale({ 1.5f, 1.5f, 1.5f });
	//位置初期化。
	m_pos = m_unityChan->GetPosition();

	//GameCameraインスタンス化。
	m_camera = NewGO<GameCamera>(EnPriority_3DModel, "GameCamera");
	m_camera->SetCameraType(true);

	//武器。
	m_wepon = NewGO<Rifle>(EnPriority_3DModel, "Wepon");

	//レティクル初期化。
	m_reticle = NewGO<SpriteRender>(EnPriority_UI);
	SpriteInitData testInitData;
	//ddsファイル初期化。
	testInitData.m_ddsFilePath[0] = "Assets/sprite/reticle.dds";
	testInitData.m_width = 50.0f;
	testInitData.m_height = 50.0f;
	testInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_reticle->Init(testInitData);
	m_reticle->SetPos({ 0.0f, m_pos.y, 0.0f });

	return true;
}

void GamePlayer::Update()
{
	if (m_playerState != EnPlayerState_Deth) {
		//死んでない。
		//移動。
		Move();
		//射撃。
		Shot();

	}

	switch (m_playerState)
	{
	case EnPlayerState_Idle:
		m_unityChan->PlayAnimation(EnPlayerState_Idle, 0.5f);
		break;
	case EnPlayerState_Walk:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Run:
		m_unityChan->PlayAnimation(EnPlayerState_Run, 0.5f);
		break;
	case EnPlayerState_Buck:
		m_unityChan->PlayAnimation(EnPlayerState_Buck, 0.5f);
		break;
	case EnPlayerState_Shot:
		m_unityChan->PlayAnimation(EnPlayerState_Shot, 0.2f);
		break;
	case EnPlayerState_Reload:
		break;
	case EnPlayerState_Deth:
		break;
	case EnPlayerState_Num:
		break;
	}
	m_unityChan->SetPosition(m_pos);
	m_cameraPos = { m_pos.x, m_pos.y + fixYToEyePos, m_pos.z };
	//カメラの位置も更新。
	m_camera->SetEyePos(m_cameraPos);

}

void GamePlayer::Rotation()
{
	//カメラに合わせて向きも変える。
	float angle = atan2(m_camera->GetToPos().x, m_camera->GetToPos().z);
	m_rot.SetRotation(g_vec3AxisY, angle);
	//m_rot.AddRotationY(3.14);
	m_unityChan->SetRotation(m_rot);
}

void GamePlayer::Shot()
{
	m_flame++;
	if (GetAsyncKeyState(VK_SPACE)) {
		if (m_flame >= 20) {
			Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel);
			bullet->SetPos({ m_pos.x, m_pos.y + fixYToEyePos, m_pos.z });
			bullet->SetRot(m_rot);
			m_flame = 0;
		}
		m_playerState = EnPlayerState_Shot;
	}
}

void GamePlayer::Move()
{
	//カメラの向き情報。
	Vector3 camForward = GraphicsEngineObj()->GetCamera3D().GetForward();
	Vector3 camRight = GraphicsEngineObj()->GetCamera3D().GetRight();
	camForward.y = 0.0f;
	camForward.Normalize();
	camRight.y = 0.0f;
	camRight.Normalize();
	
	//移動量。
	Vector3 move = g_vec3Zero;

	//移動処理。
	//todo:Pad対応？キャラコン対応？慣性？
	if (GetAsyncKeyState('W')) {
		move += camForward * m_speed;
	}
	if (GetAsyncKeyState('S')) {
		m_playerState = EnPlayerState_Buck;
		move -= camForward * m_speed;
	}
	if (GetAsyncKeyState('D')) {
		move += camRight * m_speed;
	}
	if (GetAsyncKeyState('A')) {
		move -= camRight * m_speed;
	}

	if (GetAsyncKeyState(VK_SHIFT) && m_playerState != EnPlayerState_Shot && m_playerState != EnPlayerState_Buck) {
		//射撃、後退時はダメー。
		move *= 3.0f;
	}

	if (move.Length() >= 0.5f && m_playerState != EnPlayerState_Buck) {
		m_playerState = EnPlayerState_Walk;
	}
	if (move.Length() >= 2.0f) {
		m_playerState = EnPlayerState_Run;
	}
	if(move.Length() == 0.0f){
		m_playerState = EnPlayerState_Idle;
	}

	if (move.Length() != 0) {
		printf("%f, %f \n", m_pos.x, m_pos.z);
	}

	m_pos += move;
}

