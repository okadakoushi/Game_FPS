#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

bool Game::Start()
{
	//ステージの初期化。
	m_bg = NewGO<SkinModelRender>(0, "bg");
	//ディファード用のシェーダーを適用させる。todo:wrap
	m_bg->SetShader(L"Assets/shader/DefeardModel.fx");
	m_bg->Init("Assets/modelData/bgReo/bg.tkm");
	m_bg->SetShadowReciever(true);

	//unitychan初期化。
	m_unityChan = NewGO<SkinModelRender>(0, "Unity");
	/*
	//通常描画用コード。
	//エンジンの仕様上Initより早く書くこと。
	m_unityChan->SetShader(L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx");
	m_unityChan->SetForwardRender(true);
	*/
	//ディファード用のシェーダーを適用させる。todo:wrap
	m_unityChan->SetShader(L"Assets/shader/DefeardModel.fx");
	m_unityChan->Init("Assets/modelData/unityChan.tkm", "Assets/animData/unityChan/Idle.tka");
	//シャドウを出す。
	m_unityChan->SetShadwoCaster(true);

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
	m_bg->SetForwardRender(false);
	//Unitychanの表示。
	m_unityChan->SetPosition(m_pos);
	m_unityChan->SetRenderMode(enRenderMode_Skin);
}

void Game::ForwardRender()
{
}