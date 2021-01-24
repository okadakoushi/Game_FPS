#include "stdafx.h"
#include "Game.h"
#include "GameObj/Stage.h"
#include "GameObj/GamePlayer.h"
#include "GameObj/UIs.h"

//#define Mode_Game;
//#define Mode_Test;

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
	//UIs。unityとかもCanvasとかあるしあった方がいいのだろうか。。
	//m_UIs = NewGO<UIs>(EnPriority_UI, "UIs");
	m_effect = NewGO<myEngine::Effect>(0);
	
	return true;

}

void Game::Update()
{
	if(GetAsyncKeyState('R')) {
		//解放テスト用。
		DeleteGO(this);
	}
	if (GetAsyncKeyState('F') && !m_effect->IsPlay()) {
		m_effect = NewGO<myEngine::Effect>(0);
		m_effect->SetScale({ 10.0f, 10.0f, 10.0f });
		m_effect->Play(L"Assets/effect/test.efk");
	}
}
