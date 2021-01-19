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
	
	return true;

}

void Game::Update()
{
	if(GetAsyncKeyState('R')) {
		//����e�X�g�p�B
		DeleteGO(this);
	}
}
