#include "stdafx.h"
#include "Stage.h"

Stage::~Stage()
{
	DeleteGO(m_bg);
}

bool Stage::Start()
{
	////�X�e�[�W�̃C���X�^���X�B
	//m_bg = NewGO<SkinModelRender>(EnPriority_Render);
	////�������B
	//m_bg->Init("Assets/modelData/levelTkm/L_SampleStage.tkm");
	////�V���h�E���V�[�o�[�B
	//m_bg->SetShadowReciever(true);

	m_level.Init("Assets/level/StageLevel.tkl", [&](LevelObjectData& objData) {
		//if (objData.EqualObjectName(L"StageLevelObj") == true) {
		//	objData.isShadowReceiver = true;
		//	return false;
		//}
		//if (objData.EqualObjectName(L"barrel_G") == true) {
		//	objData.isShadowCaster = true;
		//	return false;
		//}
		return false;
	});

	return true;
}

void Stage::Update()
{
}