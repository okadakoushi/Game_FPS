#include "stdafx.h"
#include "GamePlayer.h"
#include "GameCamera.h"
#include "Bullet.h"
#include "Rifle.h"
#include "Enemy/RifleEnemy.h"
#include "Rifle.h"

const float	GamePlayer::BASE_PARAM::PLAYER_MAX_HP = 150.0f;
const int	GamePlayer::BASE_PARAM::REGENE_VALUE_SECOND = 50;
const float GamePlayer::BASE_PARAM::REGENE_COOL_TIME = 5.0f;
const float GamePlayer::BASE_PARAM::JUMPFORSE = 520.0f;
const float GamePlayer::BASE_PARAM::GRAVITY = 20.0f;


GamePlayer::~GamePlayer()
{
	DeleteGO(m_reticle);
	DeleteGO(m_unityChan);
}

void GamePlayer::Regene()
{
	if (m_hp < BASE_PARAM::PLAYER_MAX_HP) {
		//����HP���}�b�N�X�łȂ��B
		m_currentRegeneTime += GameTime().GetFrameDeltaTime();
		if (m_currentRegeneTime >= BASE_PARAM::REGENE_COOL_TIME) {
			//�񕜊J�n�B
			m_hp += BASE_PARAM::REGENE_VALUE_SECOND * GameTime().GetFrameDeltaTime();
		}
	}
}

void GamePlayer::Init()
{
	m_playerState = EnPlayerState_Idle;
	m_hp = BASE_PARAM::PLAYER_MAX_HP;
	m_wepon->Init();
}

bool GamePlayer::Start()
{	
	//unitychan�������B
	m_unityChan = NewGO<SkinModelRender>(EnPriority_3DModel, "Unity");
	const char* tkaFilePaths[] = {
		"Assets/animData/Enemy/Rifle/Idle.tka",
		"Assets/animData/Enemy/Rifle/Walk.tka",
		"Assets/animData/Enemy/Rifle/WalkShoot.tka",
		"Assets/animData/Enemy/Rifle/Death.tka"
	};
	m_unityChan->SetAnimLoop(EnPlayerState_Deth, false);
	m_unityChan->Init("Assets/modelData/Chara/Enemy.tkm", tkaFilePaths);
	//m_unityChan->Init("Assets/modeldata/unityChan.tkm", "Assets/animData/unityChan/test.tka");
	//�V���h�E�L���X�^�[�B
	m_unityChan->SetShadwoCaster(true);
	m_unityChan->SetShadowReciever(true);
	m_unityChan->SetScale({ 1.5f, 1.5f, 1.5f });

	//SkinModelRender�̃A�j���[�V�����Đ����I������^�C�~���O�ŌĂ΂�鏈����ݒ�B
	m_unityChan->SetPostAnimationProgressFunc([&] {	OnPostAnimationProgress(); });
	//�ʒu�������B
	m_pos = m_unityChan->GetPosition();
	//�L�����R���������B
	m_cCon.Init(25.0f, 60.0f, m_pos);
	//���̃{�[���擾�B
	int spineBoneNo = m_unityChan->GetSkelton().FindBoneID(L"mixamorig:Spine");
	m_spineBone = m_unityChan->GetSkelton().GetBone(spineBoneNo);
	//���̃{�[���擾�B
	int headNo = m_unityChan->GetSkelton().FindBoneID(L"mixamorig:Head");;
	m_headBone = m_unityChan->GetSkelton().GetBone(headNo);
	//����B
	m_wepon = NewGO<Rifle>(EnPriority_3DModel, "Wepon");
	m_wepon->SetRefBoneRender(m_unityChan);	

	//�J����Find�B
	m_camera = FindGO<GameCamera>("GameCamera");

	//UIs�B
	m_playerUIs = NewGO<PlayerUIs>(EnPriority_UI);

	//�G�t�F�N�g�����B
	m_effect = NewGO<myEngine::Effect>(EnPriority_3DModel);
	m_effect->SetScale({ 3.0f, 3.0f, 3.0f });

	//�T�E���h�������B
	m_shootSE.Init(L"Assets/Audio/AK47Shoot.wav");
	m_footStepSE.Init(L"Assets/Audio/footstep.wav");

	m_pos = { 0.0f, 0.0f, 0.0f };

	return true;
}

void GamePlayer::Update()
{
	if (m_playerState != EnPlayerState_Deth) {
		//����łȂ��B
		//�ړ��B
		Move();
		//��]�B
		//Rotation();
		//�ˌ��B
		Shot();
		//���W�F�l�B
		Regene();
		m_unityChan->SetPosition(m_pos);
	}

	switch (m_playerState)
	{
	case EnPlayerState_Idle:
		m_unityChan->PlayAnimation(EnPlayerState_Idle, 0.3f);
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
		m_unityChan->PlayAnimation(EnPlayerState_Shot, 0.3f);
		break;
	case EnPlayerState_Reload:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Deth:
		m_unityChan->PlayAnimation(EnPlayerState_Deth, 0.5f);
		//m_camera->SetActive(false);
		break;
	case EnPlayerState_Num:
		break;
	}
	
	if (m_hp <= 0) {
		m_playerState = EnPlayerState_Deth;
	}
}

Vector3& GamePlayer::CalcHeadPos()
{
	Quaternion a;
	m_headBone->CalcWorldTRS(m_headPos, a);
	m_headPos.y += 4.0f;
	m_camera->SetEyePos(m_headPos);
	return m_headPos;
}

void GamePlayer::OnPostAnimationProgress()
{
	Quaternion qRot;
	//X������̉�]���v�Z�B
	Vector3 to = GraphicsEngineObj()->GetCamera3D().GetForward();
	to.Normalize();
	//���ւ̉�]�͏������߂Ɏ��B
	qRot.SetRotation(g_vec3AxisX, atan2f(to.y * -1.2f, g_vec3Front.z));
	Matrix local = m_spineBone->GetLocalMatrix();

	Matrix trans;
	trans.MakeRotationFromQuaternion(qRot);
	local *= trans;
	//���̃{�[���ɉ�]��K�p������B
	m_spineBone->SetLocalMatrixFromUser(local);
}
void GamePlayer::Rotation()
{	
	Vector3 toPos = GraphicsEngineObj()->GetCamera3D().GetTarget() - GraphicsEngineObj()->GetCamera3D().GetPosition();
	toPos.Normalize();
	float angle = atan2(toPos.x, toPos.z);
	m_rot.SetRotation(g_vec3AxisY, angle);
	m_unityChan->SetRotation(m_rot);
}

void GamePlayer::Shot()
{
	//���C�̃R�[���o�b�N�B
	RayTestCallBack::PlayerRayTestResult rayCallBack;
	//���C�e�X�g���s���B
	Vector3 toDir = GraphicsEngineObj()->GetCamera3D().GetTarget() - m_headPos;
	toDir.Normalize();
	toDir *= RAY_RANGE;
	PhysicObj().RayTest(m_headPos, toDir + m_headPos, rayCallBack);

	m_flame++;
	if (GetAsyncKeyState(MK_LBUTTON)) {
		m_playerState = EnPlayerState_Shot;
		if (m_flame >= 20) {
			if (m_wepon->GetRifleEvent() == Rifle::EnRifleEvent_None) {
				m_shootSE.Stop();
				m_shootSE.Play(false);
				m_wepon->ReduseAmo();
				//Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel, "Bullet");
				//bullet->SetPos(m_wepon->GetPos());
				//bullet->SetRot(m_rot);
				//bullet->SetToTarget(toDir);
				m_flame = 0;
				//printf("StaticObjectDist = %f\n", rayCallBack.StaticObjectDist);
				//printf("CharacterObjectDist = %f\n", rayCallBack.CharacterObjectDist);
				if (rayCallBack.hasHit() && rayCallBack.StaticObjectDist > rayCallBack.CharacterObjectDist) {
					//�G�Ƀ��C�������B
					//���|�C���^����G�ɋ����L���X�g�B
					RifleEnemy* enemy = reinterpret_cast<RifleEnemy*>(rayCallBack.m_collisionObject->getUserPointer());
					if (enemy != nullptr) {
						enemy->GetDamage();
					}
				}
				else if (rayCallBack.hasHit() && rayCallBack.StaticObjectDist < rayCallBack.CharacterObjectDist) {
					//�Ȃ񂩂̃��b�V���ɖ����B
					//���������n�_����G�t�F�N�g���Đ��B
					Vector3 effectPos = m_headPos + toDir * rayCallBack.StaticObjectDist;
					Quaternion effectRot;
					effectRot.SetRotation(g_vec3AxisY, atan2f(g_vec3Front.z, toDir.x));
					m_effect->SetPosition(effectPos);
					m_effect->SetRotation(effectRot);
					//Y�����͂���Ȃ��B
					//printf("%f\n", rayCallBack.StaticObjectDist);
					//printf("%f, %f, %f\n", toDir.x /* rayCallBack.StaticObjectDist*/, toDir.y /* rayCallBack.StaticObjectDist*/, toDir.z /* rayCallBack.StaticObjectDist*/);
					m_effect->Play(L"Assets/effect/aaaa.efk");
				}
				else if (!rayCallBack.hasHit()) {
					//printf("%f\n", rayCallBack.StaticObjectDist);
				}
			}
			else {
				m_playerState = EnPlayerState_Reload;
			}
		}
	}
}

void GamePlayer::Reload()
{
	if (m_wepon->GetRifleEvent() == Rifle::EnRifleEvent_Reloading) {
		m_wepon->AddReloadTime();
	}

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
	if (GetAsyncKeyState('R')) {
		m_playerState = EnPlayerState_Reload;
		m_wepon->SetRifleEvent(Rifle::EnRifleEvent_Reloading);
	}
	if (GetAsyncKeyState(VK_SHIFT) && m_playerState != EnPlayerState_Shot && m_playerState != EnPlayerState_Buck) {
		//�_�b�V���B
		acc *= 3.0f;
	}
	if (m_cCon.IsOnGround()) {
		if (GetAsyncKeyState(VK_SPACE)) {
			//�W�����v�B
			acc += {0, BASE_PARAM::JUMPFORSE, 0};
		}
	}
	else {
		//�d�́B
#ifdef MASTER
		acc += {0, -BASE_PARAM::GRAVITY, 0};
#endif
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
		//printf("%f, %f, %f \n", m_pos.x,m_pos.y, m_pos.z);
	}


	//���C�B
	m_move.x += m_move.x * -0.5f;
	m_move.z += m_move.z * -0.5f;
	m_move.y += m_move.y * -0.025f;
	m_pos = m_cCon.Execute(m_move);
}

