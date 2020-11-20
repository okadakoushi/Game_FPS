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

	return true;
}

void GamePlayer::Update()
{
	//移動。
	if (GetAsyncKeyState('A')) {
		//m_pos.x++;
	}
	else if (GetAsyncKeyState('D')) {
		//m_pos.x--;
	}
	else if (GetAsyncKeyState('W')) {
		m_pos.z++;
	}
	else if (GetAsyncKeyState('S')) {
		m_pos.z--;
	}
	m_unityChan->SetPosition(m_pos);
	
	//カメラの設定。
	Vector3 m_cameraPos = g_vec3Zero;			
	m_cameraPos = { m_pos.x, m_pos.y + 50.0f, m_pos.z + 10.0f };
	m_camera->SetPosOnFPS(m_cameraPos);
	//ターゲット設定。
	Vector3 m_target = g_vec3Zero;
	m_target = { m_cameraPos.x,m_cameraPos.y, m_cameraPos.z + 50.0f };
	m_camera->SetTarget(m_target);
}