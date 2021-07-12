#include "stdafx.h"
#include "GamePlayer.h"
#include "GameCamera.h"
#include "Bullet.h"
#include "Rifle.h"

void GamePlayer::OnDestroy()
{
	DeleteGO(m_reticle);
	DeleteGO(m_unityChan);
}

bool GamePlayer::Start()
{	
	//unitychan�������B
	m_unityChan = NewGO<SkinModelRender>(EnPriority_3DModel, "Unity");
	const char* tkaFilePaths[] = {
		"Assets/animData/soldier/idle.tka",
		"Assets/animData/soldier/Walk_BattleMode.tka",
		"Assets/animData/soldier/run.tka",
		"Assets/animData/soldier/buck.tka",
		"Assets/animData/soldier/singleShot.tka"
	};
	m_unityChan->Init("Assets/modelData/Chara/soldier_green.tkm", tkaFilePaths);
	//m_unityChan->Init("Assets/modeldata/unityChan.tkm", "Assets/animData/unityChan/test.tka");
	//�V���h�E�L���X�^�[�B
	m_unityChan->SetShadwoCaster(true);
	m_unityChan->SetShadowReciever(true);
	m_unityChan->SetScale({ 1.5f, 1.5f, 1.5f });
	//�ʒu�������B
	m_pos = m_unityChan->GetPosition();
	//�L�����R���������B
	m_cCon.Init(25.0f, 60.0f, m_pos);
	//���̃{�[���擾�B
	int spineBoneNo = m_unityChan->GetSkelton().FindBoneID(L"mixamorig:Spine");
	m_spineBone = m_unityChan->GetSkelton().GetBone(spineBoneNo);
	//����B
	m_wepon = NewGO<Rifle>(EnPriority_3DModel, "Wepon");
	m_wepon->SetRefBoneRender(m_unityChan);	

	//���e�B�N���������B
	//m_reticle = NewGO<SpriteRender>(EnPriority_UI);
	//SpriteInitData testInitData;
	////dds�t�@�C���������B
	//testInitData.m_ddsFilePath[0] = "Assets/sprite/reticle.dds";
	//testInitData.m_width = 50.0f;
	//testInitData.m_height = 50.0f;
	//testInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	//m_reticle->Init(testInitData);
	//m_reticle->SetPos({ 0.0f, m_pos.y, 0.0f });

	m_pos = { 0.0f, 0.0f, 0.0f };

	return true;
}

void GamePlayer::Update()
{
	if (m_playerState != EnPlayerState_Deth) {
		//����łȂ��B
		//��]�B
		Rotation();
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
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Buck:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Shot:
		m_unityChan->PlayAnimation(EnPlayerState_Shot, 0.2f);
		break;
	case EnPlayerState_Reload:
		break;
	case EnPlayerState_Deth:
		break;
	case EnPlayerState_Num:
		break;
	}
	m_unityChan->SetPosition(m_pos);
}

void GamePlayer::Rotation()
{	
	Quaternion qRot;
	//X������̉�]���v�Z�B
	Vector3 to = GraphicsEngineObj()->GetCamera3D().GetForward();
	to.Normalize();
	//���ւ̉�]�͏������߂Ɏ��B
	qRot.SetRotation(g_vec3AxisX, atan2f(to.y * -1.2f, g_vec3Front.z ));
	Matrix local = m_spineBone->GetLocalMatrix();
	Matrix trans;
	trans.MakeRotationFromQuaternion(qRot);
	local *= trans;
	//���̃{�[���ɉ�]��K�p������B
	m_spineBone->SetLocalMatrixFromUser(local);

	//Y������̉�]�쐬�B
	float x = g_pad[0]->GetRStickXF();
	qRot.SetRotationDeg(g_vec3AxisY, 1.0f * x);
	m_rot.Multiply(qRot);
	m_unityChan->SetRotation(m_rot);
}

void GamePlayer::Shot()
{
	m_flame++;
	if (GetAsyncKeyState('F')) {
		if (m_flame >= 20) {
			Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel);
			bullet->SetPos(m_wepon->GetPos());
			bullet->SetRot(m_rot);
			m_flame = 0;
		}
		m_playerState = EnPlayerState_Shot;
	}
	PhysicObj().RayTest(GraphicsEngineObj()->GetCamera3D().GetPosition(), GraphicsEngineObj()->GetCamera3D().GetTarget());
}

void GamePlayer::Move()
{
	float deltaTime = GameTime().GetFrameDeltaTime();
	//�J�����̌������B
	Vector3 camForward = GraphicsEngineObj()->GetCamera3D().GetForward();
	Vector3 camRight = GraphicsEngineObj()->GetCamera3D().GetRight();
	camForward.y = 0.0f;
	camForward.Normalize();
	camRight.y = 0.0f;
	camRight.Normalize();
	
	//�����x�B
	Vector3 acc;

	//�ړ������B
	//todo:Pad�Ή��H�L�����R���Ή��H�����H
	if (GetAsyncKeyState('W')) {
		m_playerState = EnPlayerState_Walk;
		acc += camForward * m_speed;
	}
	if (GetAsyncKeyState('S')) {
		m_playerState = EnPlayerState_Buck;
		acc -= camForward * m_speed;
	}
	if (GetAsyncKeyState('D')) {
		m_playerState = EnPlayerState_Walk;
		acc += camRight * m_speed;
	}
	if (GetAsyncKeyState('A')) {
		m_playerState = EnPlayerState_Walk;
		acc -= camRight * m_speed;
	}
	if (GetAsyncKeyState(VK_SHIFT) && m_playerState != EnPlayerState_Shot && m_playerState != EnPlayerState_Buck) {
		//�_�b�V���B
		acc *= 3.0f;
	}
	if (m_cCon.IsOnGround()) {
		if (GetAsyncKeyState(VK_SPACE)) {
			//�W�����v�B
			acc += {0, m_JUMPFORSE, 0};
		}
	}
	else {
		//�d�́B
		//acc += {0, -m_GRAVITY, 0};
	}



	m_move += acc * 2.0;

	if (m_move.Length() >= 300.0f && m_playerState != EnPlayerState_Buck) {
		m_playerState = EnPlayerState_Walk;
	}
	if (m_move.Length() >= 700.0f) {
		m_playerState = EnPlayerState_Run;
	}
	if(m_move.Length() == 0.0f){
		m_playerState = EnPlayerState_Idle;
	}

	if (m_move.Length() != 0) {
		printf("%f, %f, %f \n", m_pos.x,m_pos.y, m_pos.z);
	}


	//���C�B
	m_move.x += m_move.x * -0.5f;
	m_move.z += m_move.z * -0.5f;
	m_move.y += m_move.y * -0.05f;
	m_pos = m_cCon.Execute(m_move);
}

