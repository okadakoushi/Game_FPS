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
	DeleteGO(m_guide);
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
	m_naviMesh->InitRender();

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
			m_player->SetPos(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"GuideRespawn") == true) {
			//m_guide = NewGO<Guide>(EnPriority_3DModel, "Guide");
			//m_guide->SetPos(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"soldierDoc") == true) {
			////��҃I�u�W�F�B
			//m_modelRender[m_mobCount] = NewGO<SkinModelRender>(EnPriority_3DModel);
			//const char* tkaFilePaths[]{
			//	"Assets/animData/soldierMob/talk.tka"
			//};
			//m_modelRender[m_mobCount]->Init("Assets/modelData/Chara/soldierDoc.tkm" ,tkaFilePaths);
			//m_modelRender[m_mobCount]->SetRenderMode(enRenderMode_Skin);
			//m_modelRender[m_mobCount]->SetPosition(objData.position);
			//m_modelRender[m_mobCount]->SetScale(objData.scale);
			//m_modelRender[m_mobCount]->SetRotation(objData.rotatatin);
			//m_modelRender[m_mobCount]->SetShadwoCaster(true);
			//m_modelRender[m_mobCount]->SetShadowReciever(true);
			////������ƃA�j���[�V�������炷�B
			//m_modelRender[m_mobCount]->SetWaitTimeForAnim(m_mobCount * 61);
			//m_mobCount++;
			//return true;
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

	if (currentEnemyCount <= 0) {
		//�G��S���|�����B
		//NextStage?Title?
		printf("���ׂĂ̓G��|����\n");
		m_stageGenerator->DeleteCurrentStage();
		m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_BattleStage2);
	}

	if (m_player->GetHP() <= 0) {
		//�v���C���[���S�B
		//Retry?Title?
		printf("�v���C���[���S�B\n");
		m_stageGenerator->DeleteCurrentStage();
		m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_BattleStage1);
	}
}

void Stage::ForwardRender()
{
#ifndef MASTER
	m_naviMesh->Render();
#endif
}
