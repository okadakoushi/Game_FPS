#include "stdafx.h"
#include "PlayerUIs.h"
#include "GamePlayer.h"
#include "Rifle.h"

PlayerUIs::PlayerUIs()
{
}

PlayerUIs::~PlayerUIs()
{
}

bool PlayerUIs::Start()
{
	m_player = FindGO<GamePlayer>("Player");

	//レティクル初期化。
	m_reticule = NewGO<SpriteRender>(EnPriority_UI);
	SpriteInitData InitData;
	//ddsファイル初期化。
	InitData.m_ddsFilePath[0] = "Assets/sprite/reticle.dds";
	InitData.m_width = 50.0f;
	InitData.m_height = 50.0f;
	InitData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_reticule->Init(InitData);
	m_reticule->SetPos({0.0f, m_player->GetPos().y, 0.0f });

	//メインウェポンのUI。
	m_mainWeponImage = NewGO<SpriteRender>(EnPriority_UI);
	//ddsファイル初期化。
	InitData.m_ddsFilePath[0] = "Assets/sprite/AK47.dds";
	InitData.m_width = 200.0f;
	InitData.m_height = 100.0f;
	m_mainWeponImage->Init(InitData);
	m_mainWeponImage->SetPos({ FRAME_BUFFER_W * -0.42f, FRAME_BUFFER_H * -0.45f , 0.0f});
	//m_reticule->SetPos({ 0.0f, m_player->GetPos().y, 0.0f });

	//弾丸のUI。
	InitData.m_width = 10.0f;
	InitData.m_height = 50.0f;
	InitData.m_ddsFilePath[0] = "Assets/sprite/Bullet.dds";
	for (int i = 0; i < 3; i++) {
		m_LeftAmoImage[i] = NewGO<SpriteRender>(EnPriority_UI);
		m_LeftAmoImage[i]->Init(InitData);
		//回転。
		Quaternion rot;
		rot.SetRotationDegZ(90.0f);
		m_LeftAmoImage[i]->SetRotation(rot);
		m_LeftAmoImage[i]->SetPos({ FRAME_BUFFER_W * -0.47f, FRAME_BUFFER_H * -0.39f + i * 15 , 0.0f });
	}

	return true;
}

void PlayerUIs::Update()
{
}

void PlayerUIs::RenderHUD()
{
	//HP。
	m_hpUIText.Begin();
	wchar_t hpTex[16];
	swprintf_s(hpTex, L" HP = %d", m_player->GetHP());
	m_hpUIText.Draw(hpTex, { FRAME_BUFFER_W * -0.52f, FRAME_BUFFER_H * -0.41f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f, 1.5f, { 0.0f, 0.0f });
	//m_hpUIText.End();

	//
	//m_LeftAmoText.Begin();
	wchar_t leftAmoTex[16];
	swprintf_s(leftAmoTex, L"%d/%d", m_player->GetWepon()->GetCurrentAmo(), m_player->GetWepon()->GetMAX_AMO());
	m_LeftAmoText.Draw(leftAmoTex, { FRAME_BUFFER_W * 0.34f, FRAME_BUFFER_H * -0.34f }, Vector4::White, 0.0f, 1.0f, { 0.0f, 0.0f });
	m_LeftAmoText.End();
}
