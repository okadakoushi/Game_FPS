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
	m_unityChan->Init("Assets/modelData/unityChan.tkm", "Assets/animData/unityChan/run.tka");
	m_unityChan->SetShadwoCaster(true);

	return true;

}

Game::~Game()
{
}

void Game::Update()
{
	//アニメーションのプレイ。
	m_unityChan->PlayAnimation(0, 0.0f);
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
	m_unityChan->PlayAnimation(0, 1.0f);
}

void Game::PreRender()
{
}

void Game::Draw()
{
}