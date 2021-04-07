#include "stdafx.h"
#include "Game.h"
#include "GameObj/Stage.h"
#include "GameObj/GamePlayer.h"
#include "GameObj/UIs.h"
#include "GameObj/Enemy.h"
#include "GameObj/GameCamera.h"
#include "GameObj/Rifle.h"

Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_stage);
	//DeleteGO(m_UIs);
	//最後に消すこと。
	DeleteGO(m_player);
}

bool Game::Start()
{
	//stage。
	m_stage = NewGO<Stage>(EnPriority_3DModel, "bg");
	//GamePlayer。
	m_player = NewGO<GamePlayer>(EnPriority_3DModel, "Player");
	//GameCameraインスタンス化。
	m_camera = NewGO<GameCamera>(Enpriority_Camera, "GameCamera");
	m_player->SetCamera(m_camera);
	//Enemy。
	m_enemy = NewGO<Enemy>(EnPriority_3DModel, "Enemy");
	m_enemy->SetPosition({ 950,0,650 });
	//Effect。
	m_effect = NewGO<myEngine::Effect>(0);
	//UIs。unityとかもCanvasとかあるしあった方がいいのだろうか。。
	//m_UIs = NewGO<UIs>(EnPriority_UI, "UIs");

	
	
	return true;

}

void Game::Update()
{
	if(GetAsyncKeyState('R')) {
		//解放テスト用。
		//DeleteGO(this);
	}
	if (GetAsyncKeyState('F') && !m_effect->IsPlay()) {
		m_effect = NewGO<myEngine::Effect>(0);
		m_effect->SetScale({ 10.0f, 10.0f, 10.0f });
		m_effect->Play(L"Assets/effect/test.efk");
	}
}
