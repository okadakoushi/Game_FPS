#include "stdafx.h"
#include "BattleStage1.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

BattleStage1::BattleStage1()
{
	delete m_naviMesh;
}

bool BattleStage1::Start()
{
	//ナビメッシュ作成。
	m_naviMesh = new NaviMesh;
	m_naviMesh->Load("Assets/nvm/GameStage_BattleMap1.nvm");
	m_naviMesh->InitRender();

	//レベルの初期化。
	m_level.Init("Assets/level/GameStage_BattleMap1.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"PlayerRespawn") == true) {
			m_player->SetPos(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"EnemyRespawn1") == true) {
			m_rifleEnemy[0] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[0]->SetNaviMesh(m_naviMesh);
			return true;
		}
		if (objData.EqualObjectName(L"EnemyRespawn2") == true) {
			m_rifleEnemy[1] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[1]->SetNaviMesh(m_naviMesh);
			return true;
		}		
		if (objData.EqualObjectName(L"EnemyRespawn3") == true) {
			m_rifleEnemy[2] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[2]->SetNaviMesh(m_naviMesh);
			return true;
		}		
		if (objData.EqualObjectName(L"EnemyRespawn4") == true) {
			m_rifleEnemy[3] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[3]->SetNaviMesh(m_naviMesh);
			return true;
		}		
		if (objData.EqualObjectName(L"EnemyRespawn5") == true) {
			m_rifleEnemy[4] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[4]->SetNaviMesh(m_naviMesh);
			return true;
		}
		if (objData.EqualObjectName(L"EnemyRespawn6") == true) {
			m_rifleEnemy[5] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[5]->SetNaviMesh(m_naviMesh);
			return true;
		}
#ifdef NAV_DEBUG
		if (objData.EqualObjectName(L"mesh") == true) {
			return true;
		}
#endif
		return false;
		});

	return true;
}

void BattleStage1::Update()
{
}

void BattleStage1::ForwardRender()
{
#ifndef MASTER
	m_naviMesh->Render();
#endif 
}
