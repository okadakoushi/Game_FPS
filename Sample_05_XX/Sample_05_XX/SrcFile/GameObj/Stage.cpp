#include "stdafx.h"
#include "Stage.h"
#include "Enemy.h"

Stage::~Stage()
{
	//DeleteGO(m_bg);
}

bool Stage::Start()
{
	////�X�e�[�W�̃C���X�^���X�B
	//m_bg = NewGO<SkinModelRender>(EnPriority_Render);
	////�������B
	//m_bg->Init("Assets/modelData/levelTkm/L_SampleStage.tkm");
	////�V���h�E���V�[�o�[�B
	//m_bg->SetShadowReciever(true);

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
			SkinModelRender* modelReder = NewGO<SkinModelRender>(EnPriority_3DRender);
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
		//���x������X�|�[����񎝂��Ă��āA�����ɃX�|�[��������B
		//todo:enum�Ƃ��ł��܂��Ǘ��ł��邩�ȁH
		if (objData.EqualObjectName(L"noneArea") == true) {
			Enemy* enemy = NewGO<Enemy>(EnPriority_3DModel);
			enemy->SetPosition(objData.position);
			enemy->SetTarget({ -1500.0f, 0.0f, -500.0f });
			return true;
		}		
		if (objData.EqualObjectName(L"factoryArea") == true) {
			//�m�[���G���A�ɏo���B
			Enemy* enemy = NewGO<Enemy>(EnPriority_3DModel);
			enemy->SetPosition(objData.position);
			enemy->SetTarget({ 800.0f, 0.0f, -1900.0f });
			return true;
		}		
		if (objData.EqualObjectName(L"loadArea") == true) {
			//�m�[���G���A�ɏo���B
			Enemy* enemy = NewGO<Enemy>(EnPriority_3DModel);
			enemy->SetPosition(objData.position);
			enemy->SetTarget({ -1500.0f, 0.0f, -500.0f });
			return true;
		}		
		if (objData.EqualObjectName(L"tentArea") == true) {
			//�m�[���G���A�ɏo���B
			Enemy* enemy = NewGO<Enemy>(EnPriority_3DModel);
			enemy->SetPosition(objData.position);
			enemy->SetTarget({ -700.0f, 0.0f, 1600.0f });
			return true;
		}
		return false;
	});
	return true;
}

void Stage::Update()
{
}