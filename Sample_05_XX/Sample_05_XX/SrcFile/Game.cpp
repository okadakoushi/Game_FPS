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
#include "Fade.h"

Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_stageGenerator);
	DeleteGO(m_sky);
	DeleteGO(m_camera);
	DeleteGO(m_map);
	DeleteGO(m_player);
	//DeleteGO(m_UIs);
	//�Ō�ɏ������ƁB
	//DeleteGO(m_player);
}

bool Game::Start()
{
	m_camera = NewGO<GameCamera>(Enpriority_Camera, "GameCamera");
	//�Smap�̋��ʗv�f���C���X�^���X���B
	m_stageGenerator = NewGO<StageGenerator>(EnPriority_Generator, "StageGenerator");
	m_map = NewGO<Map>(EnPriority_UI, "Map");
	m_player = NewGO<GamePlayer>(EnPriority_3DModel, "Player");
#ifdef MASTER
	m_sky = NewGO<SkyBox>(EnPriority_3DModel, "Sky");
#endif

	//�X�^���o�C�X�e�[�W���쐬�B
	m_stageGenerator->SetPlayer(m_player);
#if 1
	m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_StanbyStage);
#else
	//Test
	m_effect = NewGO<myEngine::Effect>(0);
	//NewGO<SoldierMob>(0);
	//NewGO<Guide>(0);
	//NewGO<RifleEnemy>(0);
#endif
	return true;

}

void Game::Update()
{
	if(GetAsyncKeyState('R')) {
		//����e�X�g�p�B
		//DeleteGO(this);
	}
	m_map->SetTargetPos(m_player->GetPos());
}

void Game::RenderHUD()
{

}

