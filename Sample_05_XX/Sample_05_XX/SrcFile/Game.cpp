#include "stdafx.h"
#include "Game.h"
#include "GameObj/Stage.h"
#include "GameObj/GamePlayer.h"

Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_stage);
	DeleteGO(m_player);
}

bool Game::Start()
{
	//stage�B
	m_stage = NewGO<Stage>(EnPriority_3DModel, "bg");
	//GamePlayer�B
	m_player = NewGO<GamePlayer>(EnPriority_3DModel, "Player");

	return true;

}

void Game::Update()
{
	if(GetAsyncKeyState('R')) {
		//����e�X�g�p�B
		DeleteGO(this);
	}
}
