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
	//�������B
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
	//�����_�[�S�����B
	for (auto* render : m_modelRender) {
		if (render != nullptr) {
			DeleteGO(render);
		}
	}
	//���m�S�����B
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
	//�i�r���b�V���쐬�B
	m_naviMesh = new NaviMesh;
	m_naviMesh->Load("Assets/nvm/StageLevel.nvm");
#ifdef NAV_DEBUG
	m_naviMesh->InitRender();
#endif

	int spawnPointIndex = 0;	//Mob�\���W���[�p�Y���B
	int renderIndex = 0;		//�����_�[�p�̓Y���B
	//�X�|�[���ʒu
	Vector3 spawnPos[SpawnPointCount] = {
		{ -1500.0f, 0.0f, -500.0f },
		{ 800.0f, 0.0f, -1900.0f },
		{ -1500.0f, 0.0f, -500.0f },
		{ -700.0f, 0.0f, 1600.0f },
		{ 0.0, 0.0f, 0.0 }
	};
	//���x���̏������B
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
			//�G���A�I�u�W�F�N�g�����������B
			//���u�L�������쐬�B
			m_rifleEnemys[spawnPointIndex] = NewGO<RifleEnemy>(EnPriority_3DModel);
			m_rifleEnemys[spawnPointIndex]->SetPosition(objData.position);
			//���u�L�������₵����AspawnPoint�����₵�ĂˁB
			for (int i = 0; i < SpawnPointCount; i++) {
				m_rifleEnemys[spawnPointIndex]->GetAgent().RegistPath(spawnPos[i]);
			}
			m_rifleEnemys[spawnPointIndex]->SetNaviMesh(m_naviMesh);
			spawnPointIndex++;
			return true;
		}
		if (wcsstr(objData.name, L"securityCameraPos") != nullptr) {
			//�Z�L�����e�B�[�J�����B
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
				//�A�N�e�B�u����Ȃ��B
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
		//�G��S���|�����B
		//NextStage?Title?
		if (m_stageGenerator->FadeProcess(true)) {
			//�t�F�[�h�J�n�B
			printf("���ׂĂ̓G��|����\n");
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
		//�v���C���[���S�B
		//Retry?Title?
		if (m_stageGenerator->FadeProcess(true)) {
			//�t�F�[�h�J�n�B
			printf("�v���C���[���S�B\n");
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
