#include "stdafx.h"
#include "Stage.h"

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

	m_level.Init("Assets/level/NavTest.tkl", [&](LevelObjectData& objData) {
#ifdef NAV_DEBUG
		if (objData.EqualObjectName(L"test_mesh") == true) {
			return true;
		}
#endif
		return false;
	});
	return true;
}

void Stage::Update()
{
}