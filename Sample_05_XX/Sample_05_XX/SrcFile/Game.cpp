#include "stdafx.h"
#include "Game.h"
#include "GameObj/Stage.h"
#include "GameObj/GamePlayer.h"
#include "GameObj/UIs.h"
#include "GameObj/SoldierMob.h"
#include "GameObj/Enemy/RifleEnemy.h"
#include "GameObj/GameCamera.h"
#include "GameObj/Rifle.h"
#include "GameObj/testObj/TestBox.h"
#include "SrcFile/Map.h"
#include "SrcFile/nature/SkyBox.h"

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
	//GameCameraインスタンス化。
	m_camera = NewGO<GameCamera>(Enpriority_Camera, "GameCamera");
	//stage。
	m_stage = NewGO<Stage>(EnPriority_3DModel, "bg");
	//sky
	m_sky = NewGO<SkyBox>(EnPriority_3DModel, "Sky");
	//GamePlayer。
	m_player = NewGO<GamePlayer>(EnPriority_3DModel, "Player");
	m_player->SetCamera(m_camera);
	//testEnemy
	m_enemy = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
	//map。
	m_map = NewGO<Map>(EnPriority_UI, "Map");
	//Effect。
	m_effect = NewGO<myEngine::Effect>(0);
	
	
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
	m_map->SetTargetPos(m_player->GetPos());
}
