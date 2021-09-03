#include "stdafx.h"
#include "Stage.h"
#include "SrcFile/GameObj/SoldierMob.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/guide.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"
#include "SrcFile/GameObj/StageGenerator.h"

namespace {
	const int SpawnPointCount = 5;
}

Stage::Stage()
{
	m_stageGenerator = FindGO<StageGenerator>("StageGenerator");
	//初期化。
	for (auto* render : m_modelRender) {
		render = nullptr;
	}
	for (auto* soldier : m_rifleEnemys) {
		soldier = nullptr;
	}
}


Stage::~Stage()
{
	//DeleteGO(m_bg);
	//レンダー全消し。
	for (auto* render : m_modelRender) {
		if (render != nullptr) {
			DeleteGO(render);
		}
	}
	//兵士全消し。
	for (auto* soldier : m_rifleEnemys) {
		if (soldier != nullptr) {
			DeleteGO(soldier);
		}
	}
	delete m_naviMesh;
}

void Stage::OnDestroy()
{
}

bool Stage::Start()
{
	//ナビメッシュ作成。
	m_naviMesh = new NaviMesh;
	m_naviMesh->Load("Assets/nvm/StageLevel.nvm");
#ifdef NAV_DEBUG
	m_naviMesh->InitRender();
#endif

	int spawnPointIndex = 0;	//Mobソルジャー用添字。
	int renderIndex = 0;		//レンダー用の添字。
	//スポーン位置
	Vector3 spawnPos[SpawnPointCount] = {
		{ -1500.0f, 0.0f, -500.0f },
		{ 800.0f, 0.0f, -1900.0f },
		{ -1500.0f, 0.0f, -500.0f },
		{ -700.0f, 0.0f, 1600.0f },
		{ 0.0, 0.0f, 0.0 }
	};
	//レベルの初期化。
	m_level.Init("Assets/level/GameStage.tkl", [&](LevelObjectData& objData) {
#ifdef NAV_DEBUG
		if (objData.EqualObjectName(L"test_mesh") == true) {
			return true;
		}
		if (objData.EqualObjectName(L"mesh") == true) {
			return true;
		}
		if (objData.EqualObjectName(L"wareHouse_Mesh") == true) {
			return true;
		}
#endif
		if (objData.EqualObjectName(L"PlayerRespawn") == true) {
			m_playerPos = objData.position;
			if (m_player != nullptr) {
				m_player->SetPos(m_playerPos);
			}
			return true;
		}
		if (objData.EqualObjectName(L"GuideRespawn") == true) {
			//m_guide = NewGO<Guide>(EnPriority_3DModel, "Guide");
			//m_guide->SetPos(objData.position);
			return true;
		}
		if (wcsstr(objData.name, L"Area\0") != nullptr) {
			//エリアオブジェクトが見つかった。
			//モブキャラを作成。
			m_rifleEnemys[spawnPointIndex] = NewGO<RifleEnemy>(EnPriority_3DModel);
			m_rifleEnemys[spawnPointIndex]->SetPosition(objData.position);
			//モブキャラ増やしたら、spawnPointも増やしてね。
			for (int i = 0; i < SpawnPointCount; i++) {
				m_rifleEnemys[spawnPointIndex]->GetAgent().RegistPath(spawnPos[i]);
			}
			m_rifleEnemys[spawnPointIndex]->SetNaviMesh(m_naviMesh);
			spawnPointIndex++;
			return true;
		}
		if (wcsstr(objData.name, L"securityCameraPos") != nullptr) {
			//セキュリティーカメラ。
			m_securityCameraPosList.push_back(objData.position);
			m_securityCameraRotList.push_back(objData.rotatatin);
			return true;
		}
		if (wcsstr(objData.name, L"mountain") != nullptr) {
			objData.isShadowReceiver = false;
			return false;
		}
		return false;
	});
	return true;
}

void Stage::Update()
{
	int currentEnemyCount = m_enemyCount;
	
	for (auto* sol : m_rifleEnemys) {
		if (sol != nullptr) {
			if (!sol->IsActive()) {
				//アクティブじゃない。
				currentEnemyCount--;
			}
		}
	}

	//cheatCode//
	if (GetAsyncKeyState('L')) {
		for (auto* sol : m_rifleEnemys) {
			if (sol != nullptr) {
				sol->SetActive(false);
			}
		}
	}
	//cheatCode//

	if (currentEnemyCount <= 0) {
		//敵を全部倒した。
		//NextStage?Title?
		if (m_stageGenerator->FadeProcess(true)) {
			//フェード開始。
			printf("すべての敵を倒した\n");
			m_stageGenerator->DeleteCurrentStage();
			if (m_stageGenerator->GetCurrentStageNumber() == StageGenerator::EnStageNumber_BattleStage1) {
				m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_BattleStage2);
			}
			else {
				m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_BattleStage4);
			}

		}
	}

	if (m_player->GetHP() <= 0) {
		//プレイヤー死亡。
		//Retry?Title?
		if (m_stageGenerator->FadeProcess(true)) {
			//フェード開始。
			printf("プレイヤー死亡。\n");
			m_stageGenerator->DeleteCurrentStage();
			m_stageGenerator->CreateStage(m_stageGenerator->GetCurrentStageNumber());
		}
	}
}

void Stage::ForwardRender()
{
#ifdef NAV_DEBUG
	m_naviMesh->Render();
#endif
}
