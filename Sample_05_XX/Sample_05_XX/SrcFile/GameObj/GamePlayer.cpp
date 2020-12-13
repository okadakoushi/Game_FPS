#include "stdafx.h"
#include "GamePlayer.h"
#include "GameCamera.h"
#include "Bullet.h"

GamePlayer::~GamePlayer()
{
	DeleteGO(m_unityChan);
	DeleteGO(m_camera);
}

bool GamePlayer::Start()
{	
	//unitychan�������B
	m_unityChan = NewGO<SkinModelRender>(EnPriority_3DRender, "Unity");
	const char* tkaFilePaths[] = {
		"Assets/animData/soldier/idle.tka",
		"Assets/animData/soldier/walk.tka",
		"Assets/animData/soldier/run.tka"
	};
	m_unityChan->Init("Assets/modelData/Chara/soldier_green.tkm", tkaFilePaths);
	//m_unityChan->Init("Assets/modeldata/unityChan.tkm", "Assets/animData/unityChan/test.tka");
	//�V���h�E�L���X�^�[�B
	m_unityChan->SetShadwoCaster(true);
	//�X�L���`��B
	m_unityChan->SetRenderMode(enRenderMode_Skin);
	//�ʒu�������B
	m_pos = m_unityChan->GetPosition();

	//GameCamera�C���X�^���X���B
	m_camera = NewGO<GameCamera>(EnPriority_3DModel, "GameCamera");
	m_camera->SetCameraType(true);

	return true;
}

void GamePlayer::Update()
{
	if (m_playerState != EnPlayerState_Deth) {
		//����łȂ��B
		//�ړ��B
		Move();
		//�ˌ��B
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
		m_unityChan->PlayAnimation(EnPlayerState_Run, 1.0f);
		break;
	case EnPlayerState_Shot:
		break;
	case EnPlayerState_Reload:
		break;
	case EnPlayerState_Deth:
		break;
	case EnPlayerState_Num:
		break;
	}
	m_unityChan->SetPosition(m_pos);
	//�J�����̈ʒu���X�V�B
	m_camera->SetEyePos(m_pos);

}

void GamePlayer::Rotation()
{
	//�J�����ɍ��킹�Č������ς���B
	float angle = atan2(m_camera->GetToPos().x, m_camera->GetToPos().z);
	m_rot.SetRotation(g_vec3AxisY, angle);
	m_unityChan->SetRotation(m_rot);
}

void GamePlayer::Shot()
{
	m_flame++;
	if (GetAsyncKeyState(VK_SPACE) && m_flame >= 60) {
		NewGO<Bullet>(EnPriority_3DModel);
		m_flame = 0;
	}
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
	
	//�ړ��ʁB
	Vector3 move = g_vec3Zero;

	//�ړ������B
	//todo:Pad�Ή��H�L�����R���Ή��H�����H
	if (GetAsyncKeyState('W')) {
		move += camForward * m_speed;
	}
	if (GetAsyncKeyState('S')) {
		move -= camForward * m_speed;
	}
	if (GetAsyncKeyState('D')) {
		move += camRight * m_speed;
	}
	if (GetAsyncKeyState('A')) {
		move -= camRight * m_speed;
	}

	if (GetAsyncKeyState(VK_SHIFT)) {
		move *= 3.0f;
	}

	if (move.Length() >= 0.5f) {
		m_playerState = EnPlayerState_Walk;
	}
	if (move.Length() >= 2.0f) {
		m_playerState = EnPlayerState_Run;
	}
	if(move.Length() == 0.0f){
		m_playerState = EnPlayerState_Idle;
	}

	m_pos += move;
}

