#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

bool Game::Start()
{
	//ステージの初期化。
	m_bg = NewGO<SkinModelRender>(0, "bg");
	m_bg->Init("Assets/modelData/bgReo/bg.tkm");
	m_bg->SetShadowReciever(true);

	//unitychan初期化。
	m_unityChan = NewGO<SkinModelRender>(0, "Unity");
	m_unityChan->Init("Assets/modelData/unityChan.tkm", "Assets/animData/unityChan/Idle.tka");
	//m_unityChan->Init("Assets/modelData/tkSample_thief/Thethief_H.tkm","Assets/modelData/tkSample_thief/anim/idle.tka");
	//Quaternion qRot;
	//qRot.SetRotationDegX(90.0f);
	//m_unityChan->SetRotation(qRot);
	//Vector3 scale = g_vec3One * 1.5;
	//m_unityChan->SetScale(scale);
	//m_unityChan->SetShadwoCaster(true);

	return true;

}

Game::~Game()
{
}

void Game::Update()
{
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
	//回転を作成。
	Quaternion qRot = Quaternion::Identity;
	qRot.SetRotationDeg(Vector3::AxisX, -90.0f);
	//bgの表示。
	m_bg->SetRotation(qRot);
	m_bg->SetRenderMode(enRenderMode_NonSkin);
	//Unitychanの表示。
	m_unityChan->SetPosition(m_pos);
	m_unityChan->SetRenderMode(enRenderMode_Skin);
}

void Game::PreRender()
{
}

void Game::Draw()
{
}