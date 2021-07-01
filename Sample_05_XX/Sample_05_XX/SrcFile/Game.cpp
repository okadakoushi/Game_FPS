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
#include "GameObj/StageGenerator.h"
#include "GameObj/guide.h"

Game::Game()
{
}

Game::~Game()
{
	//DeleteGO(m_stage);
	//DeleteGO(m_UIs);
	//最後に消すこと。
	//DeleteGO(m_player);
}

bool Game::Start()
{
	//全mapの共通要素をインスタンス化。
	m_stageGenerator = NewGO<StageGenerator>(EnPriority_Generator, "StageGenerator");
	m_map = NewGO<Map>(EnPriority_UI, "Map");
	m_player = NewGO<GamePlayer>(EnPriority_3DModel, "Player");
	m_camera = NewGO<GameCamera>(Enpriority_Camera, "GameCamera");
	//m_sky = NewGO<SkyBox>(EnPriority_3DModel, "Sky");

	//スタンバイステージを作成。
	m_stageGenerator->SetPlayer(m_player);
	//m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_StanbyStage);

	//Test
	//m_effect = NewGO<myEngine::Effect>(0);
	//NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
	//NewGO<SoldierMob>(0);
	//NewGO<Guide>(0);

	return true;

}

void Game::Update()
{
	if(GetAsyncKeyState('R')) {
		//解放テスト用。
		DeleteGO(this);
	}
	//if (GetAsyncKeyState('F') && !m_effect->IsPlay()) {
	//	//m_effect = NewGO<myEngine::Effect>(0);
	//	//m_effect->SetScale({ 10.0f, 10.0f, 10.0f });
	//	//m_effect->Play(L"Assets/effect/test.efk");
	//}
	m_map->SetTargetPos(m_player->GetPos());
}

