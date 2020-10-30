#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

bool Game::Start()
{
	//�X�e�[�W�̏������B
	m_bg = NewGO<SkinModelRender>(0, "bg");
	m_bg->Init("Assets/modelData/bgReo/bg.tkm");
	m_bg->SetShadowReciever(true);

	//unitychan�������B
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
	//�A�j���[�V�����̃v���C�B
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
	//��]���쐬�B
	Quaternion qRot = Quaternion::Identity;
	qRot.SetRotationDeg(Vector3::AxisX, -90.0f);
	//bg�̕\���B
	m_bg->SetRotation(qRot);
	m_bg->SetRenderMode(enRenderMode_NonSkin);
	//Unitychan�̕\���B
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