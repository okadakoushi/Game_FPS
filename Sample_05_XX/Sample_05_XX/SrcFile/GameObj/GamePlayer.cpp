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
	m_camera->SetCameraType(true);

	return true;
}

void GamePlayer::Update()
{
	//�ړ��B
	Move();
	//�J�����̈ʒu���X�V�B
	m_camera->SetEyePos(m_pos);
}

void GamePlayer::Move()
{
	//�J�����̌������B
	Vector3 camForward = GraphicsEngineObj()->GetCamera3D().GetForward();
	Vector3 camRight = GraphicsEngineObj()->GetCamera3D().GetRight();
	camForward.y = 0.0f;
	camForward.Normalize();
	camRight.y = 0.0f;
	camRight.Normalize();
	
	//�ړ������B
	//todo:Pad�Ή��H�L�����R���Ή��H�����H
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

	//�J�����ɍ��킹�Č������ς���B
	float angle = atan2(m_camera->GetToPos().x, m_camera->GetToPos().z);
	m_rot.SetRotation(g_vec3AxisY, angle);
	m_unityChan->SetRotation(m_rot);

	//UnityChan�̈ʒu�X�V�B
	m_unityChan->SetPosition(m_pos);
}
