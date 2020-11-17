#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

bool Game::Start()
{
	//�X�e�[�W�̏������B
	m_bg = NewGO<SkinModelRender>(0, "bg");
	//�f�B�t�@�[�h�p�̃V�F�[�_�[��K�p������Btodo:wrap
	m_bg->SetShader(L"Assets/shader/DefeardModel.fx");
	m_bg->Init("Assets/modelData/bgReo/bg.tkm");
	m_bg->SetShadowReciever(true);

	//unitychan�������B
	m_unityChan = NewGO<SkinModelRender>(0, "Unity");
	/*
	//�ʏ�`��p�R�[�h�B
	//�G���W���̎d�l��Init��葁���������ƁB
	m_unityChan->SetShader(L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx");
	m_unityChan->SetForwardRender(true);
	*/
	//�f�B�t�@�[�h�p�̃V�F�[�_�[��K�p������Btodo:wrap
	m_unityChan->SetShader(L"Assets/shader/DefeardModel.fx");
	m_unityChan->Init("Assets/modelData/unityChan.tkm", "Assets/animData/unityChan/Idle.tka");
	//�V���h�E���o���B
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
	//��]���쐬�B
	Quaternion qRot = Quaternion::Identity;
	qRot.SetRotationDeg(Vector3::AxisX, -90.0f);
	//bg�̕\���B
	m_bg->SetRotation(qRot);
	m_bg->SetRenderMode(enRenderMode_NonSkin);
	m_bg->SetForwardRender(false);
	//Unitychan�̕\���B
	m_unityChan->SetPosition(m_pos);
	m_unityChan->SetRenderMode(enRenderMode_Skin);
}

void Game::ForwardRender()
{
}