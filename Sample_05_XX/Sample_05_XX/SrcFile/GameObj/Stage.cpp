#include "stdafx.h"
#include "Stage.h"
#include "SoldierMob.h"

Stage::~Stage()
{
	//DeleteGO(m_bg);
}

bool Stage::Start()
{
	//���x���̏������B
	//���u�L�������₷�ꍇ�́A���������₵�ĂˁB
	//�C���f�b�N�X�B
	int spawnPointIndex = 0;
	//�X�|�[���ʒu
	Vector3 spawnPos[10] = {
		{ -1500.0f, 0.0f, -500.0f },
		{ 800.0f, 0.0f, -1900.0f },
		{ -1500.0f, 0.0f, -500.0f },
		{ -700.0f, 0.0f, 1600.0f },
		{ 0.0, 0.0f, 0.0 }
	};
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
		if (objData.EqualObjectName(L"soldierDoc") == true) {
			//��҃I�u�W�F�B
			SkinModelRender* modelReder = NewGO<SkinModelRender>(EnPriority_3DModel);
			const char* tkaFilePaths[]{
				"Assets/animData/soldierMob/talk.tka"
			};
			modelReder->Init("Assets/modelData/Chara/soldierDoc.tkm" ,tkaFilePaths);
			modelReder->SetRenderMode(enRenderMode_Skin);
			modelReder->SetPosition(objData.position);
			modelReder->SetScale(objData.scale);
			modelReder->SetRotation(objData.rotatatin);
			modelReder->SetShadwoCaster(true);
			modelReder->SetShadowReciever(true);
			//������ƃA�j���[�V�������炷�B
			modelReder->SetWaitTimeForAnim(m_mobCount * 61);
			m_mobCount++;
			return true;
		}
		if (wcsstr(objData.name, L"Area\0") != nullptr) {
			//�G���A�I�u�W�F�N�g�����������B
			//���u�L�������쐬�B
			SoldierMob* mob = NewGO<SoldierMob>(EnPriority_3DModel);
			mob->SetPosition(objData.position);
			//���u�L�������₵����AspawnPoint�����₵�ĂˁB
			mob->SetTarget(spawnPos[spawnPointIndex]);
			spawnPointIndex++;
			return true;
		}
		return false;
	});
	return true;
}

void Stage::Update()
{
}