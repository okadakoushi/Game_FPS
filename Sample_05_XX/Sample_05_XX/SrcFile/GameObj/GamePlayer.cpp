#include "stdafx.h"
#include "GamePlayer.h"
#include "GameCamera.h"

GamePlayer::~GamePlayer()
{
	DeleteGO(m_unityChan);
	DeleteGO(m_camera);
}

bool GamePlayer::Start()
{	
	//unitychan初期化。
	m_unityChan = NewGO<SkinModelRender>(EnPriority_3DModel, "Unity");
	m_unityChan->Init("Assets/modelData/unityChan.tkm", "Assets/animData/unityChan/Idle.tka");
	//シャドウキャスター。
	m_unityChan->SetShadwoCaster(true);
	//スキン描画。
	m_unityChan->SetRenderMode(enRenderMode_Skin);
	//位置初期化。
	m_pos = m_unityChan->GetPosition();

	//GameCameraインスタンス化。
	m_camera = NewGO<GameCamera>(EnPriority_Camera, "GameCamera");
	m_camera->SetCameraType(true);

	return true;
}

void GamePlayer::Update()
{
	//移動。
	Move();
	//カメラの位置も更新。
	m_camera->SetEyePos(m_pos);
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
	
	//移動処理。
	//todo:Pad対応？キャラコン対応？慣性？
	if (GetAsyncKeyState('W')) {
		m_pos += camForward * m_speed;
	}
	if (GetAsyncKeyState('S')) {
		m_pos -= camForward * m_speed;
	}
	if (GetAsyncKeyState('D')) {
		m_pos += camRight * m_speed;
	}
	if (GetAsyncKeyState('A')) {
		m_pos -= camRight * m_speed;
	}

	//カメラに合わせて向きも変える。
	float angle = atan2(m_camera->GetToPos().x, m_camera->GetToPos().z);
	m_rot.SetRotation(g_vec3AxisY, angle);
	m_unityChan->SetRotation(m_rot);

	//UnityChanの位置更新。
	m_unityChan->SetPosition(m_pos);
}
