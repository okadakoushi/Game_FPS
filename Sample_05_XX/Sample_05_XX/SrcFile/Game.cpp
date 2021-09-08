#include "stdafx.h"
#include "Game.h"
#include "GameObj/GamePlayer.h"
#include "GameObj/UIs.h"
#include "GameObj/SoldierMob.h"
#include "GameObj/Enemy/RifleEnemy.h"
#include "GameObj/GameCamera.h"
#include "GameObj/Rifle.h"
#include "GameObj/testObj/TestBox.h"
#include "SrcFile/Map.h"
#include "SrcFile/nature/SkyBox.h"
#include "GameObj/Stages/StageGenerator.h"
#include "GameObj/guide.h"
#include "Fade.h"

Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_camera);
	DeleteGO(m_map);
	DeleteGO(m_player);
}

bool Game::Start()
{
	//Title消してPtr迷子になってるのでこっちでFind。
	m_camera = FindGO<GameCamera>("GameCamera");
	m_player = FindGO<GamePlayer>("Player");
	//全mapの共通要素をインスタンス化。
	m_map = NewGO<Map>(EnPriority_UI, "Map");

	//Test
	//m_effect = NewGO<myEngine::Effect>(0);
	//NewGO<SoldierMob>(0);
	//NewGO<Guide>(0);
	//NewGO<RifleEnemy>(0);
	return true;

}

void Game::Update()
{
	//表示・非表示。
	//if(EngineObj().GetInputKeyCode().IsTriggerKey('R')) {
	//	if (m_map->IsActive()) {
	//		m_map->SetDraw(false);
	//	}
	//	else {
	//		m_map->SetDraw(true);
	//	}
	//}
	m_map->SetTargetPos(m_player->GetPos());
}

void Game::RenderHUD()
{

}

