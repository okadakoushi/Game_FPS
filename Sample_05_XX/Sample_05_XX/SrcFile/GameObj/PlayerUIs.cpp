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
	//Render解放。
	DeleteGO(m_uiBoxForHP);
	DeleteGO(m_uiBoxForAmo);
	DeleteGO(m_reticule);
	DeleteGO(m_mainWeponImage);
	DeleteGO(m_damageEffectSprite);
	DeleteGO(m_LeftAmoImage[0]);
	DeleteGO(m_LeftAmoImage[1]);
	DeleteGO(m_LeftAmoImage[2]);
	DeleteGO(m_hpUIText);
	DeleteGO(m_LeftAmoText);
	DeleteGO(m_LeftEnemyCount);
}

bool PlayerUIs::Start()
{
	m_player = FindGO<GamePlayer>("Player");
	
	//スプライト初期化データー。
	SpriteInitData InitData;
	Quaternion qRot;
	InitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//--レイヤー順番に注意--。

	//BoxUI。
	m_uiBoxForHP = NewGO<SpriteRender>(EnPriority_UI);
	m_uiBoxForAmo = NewGO<SpriteRender>(EnPriority_UI);
	InitData.m_ddsFilePath[0] = "Assets/sprite/UIBox.dds";
	InitData.m_width = 420.0f;
	InitData.m_height = 240.0f;
	m_uiBoxForHP->Init(InitData);
	m_uiBoxForHP->SetPos({ FRAME_BUFFER_W * 0.32f + m_saveFrame.x, FRAME_BUFFER_H * -0.41f , 0.0f });
	InitData.m_width = 300.0f;
	InitData.m_height = 500.0f;
	m_uiBoxForAmo->Init(InitData);
	m_uiBoxForAmo->SetPos({ FRAME_BUFFER_W * -0.415f + m_saveFrame.x, FRAME_BUFFER_H * -0.4f + m_saveFrame.y, 0.0f });

	//ダメージエフェクト。
	m_damageEffectSprite = NewGO<SpriteRender>(EnPriority_UI, "DamageEffectSprite");
	InitData.m_ddsFilePath[0] = "Assets/sprite/blood.dds";
	InitData.m_width = FRAME_BUFFER_W;
	InitData.m_height = FRAME_BUFFER_H * 2;
	m_damageEffectSprite->Init(InitData);

	//レティクル初期化。
	m_reticule = NewGO<SpriteRender>(EnPriority_UI);
	InitData.m_ddsFilePath[0] = "Assets/sprite/reticle.dds";
	InitData.m_width = 50.0f;
	InitData.m_height = 50.0f;
	m_reticule->Init(InitData);
	m_reticule->SetPos({0.0f, m_player->GetPos().y, 0.0f });

	//メインウェポンのUI。
	m_mainWeponImage = NewGO<SpriteRender>(EnPriority_UI);
	InitData.m_ddsFilePath[0] = "Assets/sprite/AK47.dds";
	InitData.m_width = 200.0f;
	InitData.m_height = 100.0f;
	m_mainWeponImage->Init(InitData);
	m_mainWeponImage->SetPos({ FRAME_BUFFER_W * -0.42f + m_saveFrame.x, FRAME_BUFFER_H * -0.44f + m_saveFrame.y, 0.0f});

	//弾丸のUI。
	InitData.m_width = 10.0f;
	InitData.m_height = 50.0f;
	InitData.m_ddsFilePath[0] = "Assets/sprite/Bullet.dds";
	for (int i = 0; i < 3; i++) {
		m_LeftAmoImage[i] = NewGO<SpriteRender>(EnPriority_UI);
		m_LeftAmoImage[i]->Init(InitData);
		//回転。
		qRot.SetRotationDegZ(90.0f);
		m_LeftAmoImage[i]->SetRotation(qRot);
		m_LeftAmoImage[i]->SetPos({ FRAME_BUFFER_W * -0.46f + m_saveFrame.x, FRAME_BUFFER_H * -0.38f + i * 15 + m_saveFrame.y , 0.0f });
	}

	InitData.m_ddsFilePath[0] = "Assets/sprite/Damage.dds";
	InitData.m_width = 200.0f;
	InitData.m_height = 200.0f;
	m_enemyDirectionSprite[0].Init(InitData);
	m_enemyDirectionSprite[0].SetMulColor({ 1.0f, 1.0f, 1.0f, 0.5f });

	//HPテキスト。
	m_hpUIText = NewGO<myEngine::CFontRender>(EnPriority_2DRender, "HPText");
	m_hpUIText->SetPosition({ FRAME_BUFFER_W * -0.46f + m_saveFrame.x, FRAME_BUFFER_H * -0.41f + m_saveFrame.y });
	m_hpUIText->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	m_hpUIText->SetRotation(0.0f);
	m_hpUIText->SetScale(1.5f);
	m_hpUIText->SetPivot({ 0.0f, 1.0f });
	m_hpUIText->SetShadowParam(true, 3.0f, { Vector4::Black });

	//残弾テキスト。
	m_LeftAmoText = NewGO<myEngine::CFontRender>(EnPriority_2DRender, "LeftAmoText");
	m_LeftAmoText->SetPosition({ FRAME_BUFFER_W * 0.36f - m_saveFrame.x, FRAME_BUFFER_H * -0.33f + m_saveFrame.y });
	m_LeftAmoText->SetColor(Vector4::White);
	m_LeftAmoText->SetRotation(0.0f);
	m_LeftAmoText->SetScale(1.0f);
	m_LeftAmoText->SetPivot({ 0.0f, 1.0f });
	m_LeftAmoText->SetShadowParam(true, 3.0f, { Vector4::Black });

	return true;
}

void PlayerUIs::Update()
{
	Vector4 color = { 1.0f, 1.0f, 1.0f, 0.0f };
	//ダメージエフェクト（スクリーン周りの血枠）
	//HPの比率を計算。
	float hpRaito = 1.0f - (m_player->GetHP() / GamePlayer::BASE_PARAM::PLAYER_MAX_HP);
	
	//HP率に応じてダメージエフェクトのアルファを変更。
	m_damageEffectSprite->SetMulColor({ 1.0f, 1.0f, 1.0f, hpRaito * 0.5f });

	//Findエフェクト。
	float angle = 0.0f;
	Quaternion qRot = g_quatIdentity;
	//エネミー方向エフェクト。
	for (int i = 0; i < m_findPlayerEnemyList.size(); i++) {
		//見つけられてるエネミー分スプライトを表示する。

		//ターゲットに向かうベクトル。
		Vector3 toTarget = GraphicsEngineObj()->GetCamera3D().GetCameraToTaget();
		toTarget.Normalize();

		//視点から敵に向かうベクトル。
		Vector3 toEnemy = m_findPlayerEnemyList[i]->GetPosition() - GraphicsEngineObj()->GetCamera3D().GetPosition();
		toEnemy.Normalize();

		//エネミーのアングルを計算
		angle -= atan2f(toEnemy.z, toEnemy.x);
		//プレイヤーからエネミーに向かうアングルから注視点に向かうアングルの差分を求める。
		angle += atan2f(toTarget.z, toTarget.x);

		//デグリー角度に変換して回転。
		angle = Math::RadToDeg(angle);
		qRot.SetRotationDegZ(angle);

		m_enemyDirectionSprite[i].Update(g_vec3Zero, qRot, g_vec3One, { 0.5f, 0.0f });
	}

	//アンセーフ版を使用するため、文字列はメンバ。
	swprintf_s(m_hpTex, L" HP %.0f", m_player->GetHP());
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
	//描画終了したのでリストは削除。
	m_findPlayerEnemyList.clear();
}

void PlayerUIs::ActiveProcess(bool flag)
{
	m_reticule->SetActive(flag);
	m_mainWeponImage->SetActive(flag);
	m_LeftAmoImage[0]->SetActive(flag);
	m_LeftAmoImage[1]->SetActive(flag);
	m_LeftAmoImage[2]->SetActive(flag);
	m_damageEffectSprite->SetActive(flag);
	m_hpUIText->SetActive(flag);
	m_LeftAmoText->SetActive(flag);
	m_uiBoxForHP->SetActive(flag);
	m_uiBoxForAmo->SetActive(flag);
}
