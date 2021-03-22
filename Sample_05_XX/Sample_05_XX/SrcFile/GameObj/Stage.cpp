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