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
	//unitychan�������B
	m_unityChan = NewGO<SkinModelRender>(EnPriority_3DModel, "Unity");
	m_unityChan->Init("Assets/modelData/unityChan.tkm", "Assets/animData/unityChan/Idle.tka");
	//�V���h�E�L���X�^�[�B
	m_unityChan->SetShadwoCaster(true);
	//�X�L���`��B
	m_unityChan->SetRenderMode(enRenderMode_Skin);
	//�ʒu�������B
	m_pos = m_unityChan->GetPosition();

	//GameCamera�C���X�^���X���B
	m_camera = NewGO<GameCamera>(EnPriority_Camera, "GameCamera");

	return true;
}

void GamePlayer::Update()
{
	//�ړ��B
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
	//UnityChan�̈ʒu�X�V�B
	m_unityChan->SetPosition(m_pos);
	//�J�����̈ʒu���X�V�B
	m_camera->SetEyePos(m_pos);
}