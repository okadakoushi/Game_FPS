#include "stdafx.h"
#include "Stage.h"

Stage::~Stage()
{
	DeleteGO(m_bg);
}

bool Stage::Start()
{
	////ステージのインスタンス。
	//m_bg = NewGO<SkinModelRender>(EnPriority_Render);
	////初期化。
	//m_bg->Init("Assets/modelData/NavSample/L_SampleStage.tkm");
	////シャドウレシーバー。
	//m_bg->SetShadowReciever(true);
	////軸を合わせる。
	//Quaternion qRot = Quaternion::Identity;
	//qRot.SetRotationDeg(Vector3::AxisX, -90.0f);
	////回転。
	//m_bg->SetRotation(qRot);

	m_level.Init("Assets/level/StageLevel.tkl", [&](LevelObjectData& objData) {
		return false;
	});

	return true;
}

void Stage::Update()
{
}