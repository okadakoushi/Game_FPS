#include "stdafx.h"
#include "Stage.h"

Stage::~Stage()
{
	DeleteGO(m_bg);
}

bool Stage::Start()
{
	//ステージのインスタンス。
	m_bg = NewGO<SkinModelRender>(EnPriority_Render);
	//初期化。
	m_bg->Init("Assets/modelData/NavSample/NavStageBase.tkm");
	//シャドウレシーバー。
	m_bg->SetShadowReciever(true);	

	//軸を合わせる。
	Quaternion qRot = Quaternion::Identity;
	qRot.SetRotationDeg(Vector3::AxisX, -90.0f);
	//回転。
	m_bg->SetRotation(qRot);

	return true;
}

void Stage::Update()
{
}