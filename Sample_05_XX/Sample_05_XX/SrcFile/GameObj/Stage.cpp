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
	//m_bg->Init("Assets/modelData/NavSample/L_SampleStage.tkm");
	////�V���h�E���V�[�o�[�B
	//m_bg->SetShadowReciever(true);
	////�������킹��B
	//Quaternion qRot = Quaternion::Identity;
	//qRot.SetRotationDeg(Vector3::AxisX, -90.0f);
	////��]�B
	//m_bg->SetRotation(qRot);

	m_level.Init("Assets/level/StageLevel.tkl", [&](LevelObjectData& objData) {
		return false;
	});

	return true;
}

void Stage::Update()
{
}