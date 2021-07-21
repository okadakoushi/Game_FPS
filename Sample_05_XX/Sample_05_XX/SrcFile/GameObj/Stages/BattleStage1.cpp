#include "stdafx.h"
#include "BattleStage1.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"
#include "SrcFile/GameObj/Stages/Stage.h"
#include "SrcFile/GameObj/StageGenerator.h"
#include "SrcFile/Fade.h"

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
		//todo:for-loop ここから要リファクタリング。
		if (objData.EqualObjectName(L"EnemyRespawn1") == true) {
			m_rifleEnemy[0] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[0]->SetNaviMesh(m_naviMesh);
			m_rifleEnemy[0]->SetPosition(objData.position);
			//パス登録。さすがにこれは汚いなぁ。
			m_rifleEnemy[0]->RegistPath({1677.0f, 0.0f, 2242.0f});
			m_rifleEnemy[0]->RegistPath({386.0f, 0.0f, -1608.0f});
			m_rifleEnemy[0]->RegistPath({2283.0f, 0.0f, -1488.0f});
			m_rifleEnemy[0]->RegistPath({1348.0f, 0.0f, -736.0f});
			m_rifleEnemy[0]->RegistPath({2363.0f, 0.0f, -566.0f});
			m_rifleEnemy[0]->RegistPath({1317.0f, 0.0f, 267.0f});
			m_rifleEnemy[0]->RegistPath({474.0f, 0.0f, -663.0f});
			return true;
		}
		if (objData.EqualObjectName(L"EnemyRespawn2") == true) {
			m_rifleEnemy[1] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[1]->SetNaviMesh(m_naviMesh);
			m_rifleEnemy[1]->SetPosition(objData.position);
			//パス登録。
			m_rifleEnemy[1]->RegistPath({ 1677.0f, 0.0f, 2242.0f });
			m_rifleEnemy[1]->RegistPath({ 386.0f, 0.0f, -1608.0f });
			m_rifleEnemy[1]->RegistPath({ 2283.0f, 0.0f, -1488.0f });
			m_rifleEnemy[1]->RegistPath({ 1348.0f, 0.0f, -736.0f });
			m_rifleEnemy[1]->RegistPath({ 2363.0f, 0.0f, -566.0f });
			m_rifleEnemy[1]->RegistPath({ 1317.0f, 0.0f, 267.0f });
			m_rifleEnemy[1]->RegistPath({ 474.0f, 0.0f, -663.0f });
			return true;
		}		
		if (objData.EqualObjectName(L"EnemyRespawn3") == true) {
			m_rifleEnemy[2] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[2]->SetNaviMesh(m_naviMesh);
			m_rifleEnemy[2]->SetPosition(objData.position);
			//パス登録。
			m_rifleEnemy[2]->RegistPath({ 2321.0f, 0.0f, 2347.0f });
			m_rifleEnemy[2]->RegistPath({ 703.0f, 0.0f, 832.0f });
			return true;
		}		
		if (objData.EqualObjectName(L"EnemyRespawn4") == true) {
			m_rifleEnemy[3] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[3]->SetNaviMesh(m_naviMesh);
			m_rifleEnemy[3]->SetPosition(objData.position);
			//パス登録。
			m_rifleEnemy[3]->RegistPath({ 616.0f, 0.0f,	2191.0f });
			m_rifleEnemy[3]->RegistPath({ -497.0f, 0.0f, 1073.0f });
			m_rifleEnemy[3]->RegistPath({ -663.0f, 0.0f, 2109.0f });
			return true;
		}		
		if (objData.EqualObjectName(L"EnemyRespawn5") == true) {
			m_rifleEnemy[4] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[4]->SetNaviMesh(m_naviMesh);
			m_rifleEnemy[4]->SetPosition(objData.position);
			//パス登録。
			m_rifleEnemy[4]->RegistPath({ -1291.0f, 0.0f, -2211.0f });
			m_rifleEnemy[4]->RegistPath({ -937.0f, 0.0f, -1369.0f });
			m_rifleEnemy[4]->RegistPath({ -2100.0f, 0.0f, -1392.0f });
			m_rifleEnemy[4]->RegistPath({ -2274.0f, 0.0f, -338.0f });
			//m_rifleEnemy[4]->RegistPath({ 616.0f, 0.0f,	2191.0f });
			//m_rifleEnemy[4]->RegistPath({ -497.0f, 0.0f, 1073.0f });
			//m_rifleEnemy[4]->RegistPath({ -663.0f, 0.0f, 2109.0f });
			return true;
		}
		if (objData.EqualObjectName(L"EnemyRespawn6") == true) {
			m_rifleEnemy[5] = NewGO<RifleEnemy>(EnPriority_3DModel, "Enemy");
			m_rifleEnemy[5]->SetNaviMesh(m_naviMesh);
			m_rifleEnemy[5]->SetPosition(objData.position);
			//パス登録。
			m_rifleEnemy[5]->RegistPath({ -1291.0f, 0.0f, -2211.0f });
			m_rifleEnemy[5]->RegistPath({ -937.0f, 0.0f, -1369.0f });
			m_rifleEnemy[5]->RegistPath({ -2100.0f, 0.0f, -1392.0f });
			m_rifleEnemy[5]->RegistPath({ -2274.0f, 0.0f, -338.0f });
			//m_rifleEnemy[5]->RegistPath({ 616.0f, 0.0f,	2191.0f });
			//m_rifleEnemy[5]->RegistPath({ -497.0f, 0.0f, 1073.0f });
			//m_rifleEnemy[5]->RegistPath({ -663.0f, 0.0f, 2109.0f });
			return true;
		}
#ifdef NAV_DEBUG
		if (objData.EqualObjectName(L"mesh") == true) {
			return true;
		}
#endif
		return false;
		});

	//何体エネミーいるか調べておく。
	for (auto* sol : m_rifleEnemy) {
		if (sol != nullptr) {
			m_enemyCount++;
		}
	}

	return true;
}

void BattleStage1::Update()
{
	//今の削除予定兵士の数。
	int currentSoldierCount = 0;
	for (auto* sol : m_rifleEnemy) {
		if (sol != nullptr) {
			if (!sol->IsActive()) {
				//アクテイブじゃないからインクリ。
				currentSoldierCount++;
			}
		}
	}
	if (currentSoldierCount >= m_enemyCount || m_player->GetHP() <= 0) {
		for (auto* sol : m_rifleEnemy) {
			if (sol != nullptr) {
				//全兵士アクティブじゃないから削除おおおおお。
				DeleteGO(sol);
			}
		}
		//これ以上更新は行わない。
		m_stageGenerator = FindGO<StageGenerator>("StageGenerator");
		m_stageGenerator->DeleteCurrentStage();

	}
}

void BattleStage1::ForwardRender()
{
#ifndef MASTER
	m_naviMesh->Render();
#endif 
}
