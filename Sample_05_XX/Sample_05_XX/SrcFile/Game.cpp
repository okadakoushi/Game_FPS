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
	//�Ō�ɏ������ƁB
	DeleteGO(m_player);
}

bool Game::Start()
{
	//stage�B
	m_stage = NewGO<Stage>(EnPriority_3DModel, "bg");
	//GamePlayer�B
	m_player = NewGO<GamePlayer>(EnPriority_3DModel, "Player");
	//GameCamera�C���X�^���X���B
	m_camera = NewGO<GameCamera>(Enpriority_Camera, "GameCamera");
	m_player->SetCamera(m_camera);
	//Enemy�B
	m_enemy = NewGO<Enemy>(EnPriority_3DModel, "Enemy");
	m_enemy->SetPosition({ 950,0,650 });
	//Effect�B
	m_effect = NewGO<myEngine::Effect>(0);
	//UIs�Bunity�Ƃ���Canvas�Ƃ����邵���������������̂��낤���B�B
	//m_UIs = NewGO<UIs>(EnPriority_UI, "UIs");

	
	
	return true;

}

void Game::Update()
{
	if(GetAsyncKeyState('R')) {
		//����e�X�g�p�B
		//DeleteGO(this);
	}
	if (GetAsyncKeyState('F') && !m_effect->IsPlay()) {
		m_effect = NewGO<myEngine::Effect>(0);
		m_effect->SetScale({ 10.0f, 10.0f, 10.0f });
		m_effect->Play(L"Assets/effect/test.efk");
	}
}
