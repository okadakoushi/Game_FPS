#include "stdafx.h"
#include "PlayerUIs.h"
#include "GamePlayer.h"
#include "Rifle.h"
#include "Enemy/RifleEnemy.h"

PlayerUIs::PlayerUIs()
{
}

PlayerUIs::~PlayerUIs()
{
}

bool PlayerUIs::Start()
{
	m_player = FindGO<GamePlayer>("Player");
	
	//�X�v���C�g�������f�[�^�[�B
	SpriteInitData InitData;
	Quaternion qRot;
	InitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//--���C���[���Ԃɒ���--�B

	//�_���[�W�G�t�F�N�g�B
	m_damageEffectSprite = NewGO<SpriteRender>(EnPriority_UI, "DamageEffectSprite");
	InitData.m_ddsFilePath[0] = "Assets/sprite/blood.dds";
	InitData.m_width = FRAME_BUFFER_W;
	InitData.m_height = FRAME_BUFFER_H * 2;
	m_damageEffectSprite->Init(InitData);

	//���e�B�N���������B
	m_reticule = NewGO<SpriteRender>(EnPriority_UI);
	InitData.m_ddsFilePath[0] = "Assets/sprite/reticle.dds";
	InitData.m_width = 50.0f;
	InitData.m_height = 50.0f;
	m_reticule->Init(InitData);
	m_reticule->SetPos({0.0f, m_player->GetPos().y, 0.0f });

	//���C���E�F�|����UI�B
	m_mainWeponImage = NewGO<SpriteRender>(EnPriority_UI);
	InitData.m_ddsFilePath[0] = "Assets/sprite/AK47.dds";
	InitData.m_width = 200.0f;
	InitData.m_height = 100.0f;
	m_mainWeponImage->Init(InitData);
	m_mainWeponImage->SetPos({ FRAME_BUFFER_W * -0.42f, FRAME_BUFFER_H * -0.45f , 0.0f});

	//�e�ۂ�UI�B
	InitData.m_width = 10.0f;
	InitData.m_height = 50.0f;
	InitData.m_ddsFilePath[0] = "Assets/sprite/Bullet.dds";
	for (int i = 0; i < 3; i++) {
		m_LeftAmoImage[i] = NewGO<SpriteRender>(EnPriority_UI);
		m_LeftAmoImage[i]->Init(InitData);
		//��]�B
		qRot.SetRotationDegZ(90.0f);
		m_LeftAmoImage[i]->SetRotation(qRot);
		m_LeftAmoImage[i]->SetPos({ FRAME_BUFFER_W * -0.47f, FRAME_BUFFER_H * -0.39f + i * 15 , 0.0f });
	}

	InitData.m_ddsFilePath[0] = "Assets/sprite/Damage.dds";
	InitData.m_width = 200.0f;
	InitData.m_height = 200.0f;
	m_enemyDirectionSprite[0].Init(InitData);
	m_enemyDirectionSprite[0].SetMulColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	//m_enemyDirectionSprite[0].SetPivot({ 0.5f, 0.0f });
	//m_enemyDirectionSprite[0]->SetRotation(qRot);

	//HP�e�L�X�g�B
	m_hpUIText = NewGO<myEngine::CFontRender>(EnPriority_2DRender, "HPText");
	m_hpUIText->SetPosition({ FRAME_BUFFER_W * -0.52f, FRAME_BUFFER_H * -0.41f });
	m_hpUIText->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	m_hpUIText->SetRotation(0.0f);
	m_hpUIText->SetScale(1.5f);
	m_hpUIText->SetPivot({ 0.0f, 0.0f });

	//�c�e�e�L�X�g�B
	m_LeftAmoText = NewGO<myEngine::CFontRender>(EnPriority_2DRender, "LeftAmoText");
	m_LeftAmoText->SetPosition({ FRAME_BUFFER_W * 0.34f, FRAME_BUFFER_H * -0.34f });
	m_LeftAmoText->SetColor(Vector4::White);
	m_LeftAmoText->SetRotation(0.0f);
	m_LeftAmoText->SetScale(1.0f);
	m_LeftAmoText->SetPivot({ 0.0f, 0.0f });

	return true;
}

void PlayerUIs::Update()
{
	Vector4 color = { 1.0f, 1.0f, 1.0f, 0.0f };
	//�_���[�W�G�t�F�N�g�i�X�N���[������̌��g�j
	//HP�̔䗦���v�Z�B
	float hpRaito = 1.0f - (m_player->GetHP() / GamePlayer::BASE_PARAM::PLAYER_MAX_HP);
	
	//HP���ɉ����ă_���[�W�G�t�F�N�g�̃A���t�@��ύX�B
	m_damageEffectSprite->SetMulColor({ 1.0f, 1.0f, 1.0f, hpRaito * 0.5f });

	//Find�G�t�F�N�g�B
	float angle = 0.0f;
	Quaternion qRot = g_quatIdentity;
	//�G�l�~�[�����G�t�F�N�g�B
	for (int i = 0; i < m_findPlayerEnemyList.size(); i++) {
		//�������Ă�G�l�~�[���X�v���C�g��\������B

		//�^�[�Q�b�g�Ɍ������x�N�g���B
		Vector3 toTarget = GraphicsEngineObj()->GetCamera3D().GetCameraToTaget();
		toTarget.Normalize();



		//���_����G�Ɍ������x�N�g���B
		Vector3 toEnemy = m_findPlayerEnemyList[i]->GetPosition() - GraphicsEngineObj()->GetCamera3D().GetPosition();
		toEnemy.Normalize();

		//�G�l�~�[�̃A���O�����v�Z
		angle -= atan2f(toEnemy.z, toEnemy.x);
		//�v���C���[����G�l�~�[�Ɍ������A���O�����璍���_�Ɍ������A���O���̍��������߂�B
		angle += atan2f(toTarget.z, toTarget.x);

		//�f�O���[�p�x�ɕϊ����ĉ�]�B
		angle = Math::RadToDeg(angle);
		qRot.SetRotationDegZ(angle);

		m_enemyDirectionSprite[i].Update(g_vec3Zero, qRot, g_vec3One, { 0.5f, 0.0f });
	}


	//�_���[�W�G�t�F�N�g�i�X�N���[���[�̍��g�j�������B
	//�_���[�W�G�t�F�N�g�̉e���͈́BMIN::0.0f ~ MAX::1.0f 
	if (hpRaito <= 0.3f) {
		m_damageEffectArea = 0.2f;
	}
	GraphicsEngineObj()->GetDefferd().SetDamageArea(m_damageEffectArea);

	//�A���Z�[�t�ł��g�p���邽�߁A������̓����o�B
	swprintf_s(m_hpTex, L" HP = %.0f", m_player->GetHP());
	m_hpUIText->SetTextUnsafe(m_hpTex);
	swprintf_s(m_leftAmoTex, L" %d", m_player->GetWepon()->GetCurrentAmo());
	m_LeftAmoText->SetTextUnsafe(m_leftAmoTex);
}

void PlayerUIs::RenderHUD()
{
	for (int i = 0; i < m_findPlayerEnemyList.size(); i++)
	{
		m_enemyDirectionSprite->Draw(
			GraphicsEngineObj()->GetRenderContext(),
			GraphicsEngineObj()->GetCamera2D().GetViewMatrix(),
			GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix()
		);
	}
	//�`��I�������̂Ń��X�g�͍폜�B
	m_findPlayerEnemyList.clear();
}

void PlayerUIs::ActiveProcess(bool flag)
{
	//m_reticule->SetActive(flag);
	//m_mainWeponImage->SetActive(flag);
	//m_LeftAmoImage[0]->SetActive(flag);
	//m_LeftAmoImage[1]->SetActive(flag);
	//m_LeftAmoImage[2]->SetActive(flag);
	//m_damageEffectSprite->SetActive(flag);
	//if (flag) {
	//	m_hpUIText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	//	m_LeftAmoText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	//}
	//else {
	//	m_hpUIText->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	//	m_LeftAmoText->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	//}

}
