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
	//�Ō�ɏ������ƁB
	DeleteGO(m_player);
}

bool Game::Start()
{
	//stage�B
	m_stage = NewGO<Stage>(EnPriority_3DModel, "bg");
	//GamePlayer�B
	m_player = NewGO<GamePlayer>(EnPriority_3DModel, "Player");
	//UIs�Bunity�Ƃ���Canvas�Ƃ����邵���������������̂��낤���B�B
	//m_UIs = NewGO<UIs>(EnPriority_UI, "UIs");
	m_effect = NewGO<myEngine::Effect>(0);
	
	return true;

}

void Game::Update()
{
	if(GetAsyncKeyState('R')) {
		//����e�X�g�p�B
		DeleteGO(this);
	}
	if (GetAsyncKeyState('F') && !m_effect->IsPlay()) {
		m_effect = NewGO<myEngine::Effect>(0);
		m_effect->SetScale({ 10.0f, 10.0f, 10.0f });
		m_effect->Play(L"Assets/effect/test.efk");
	}
}
