#include "stdafx.h"
#include "Stage.h"

Stage::~Stage()
{
	//DeleteGO(m_bg);
}

bool Stage::Start()
{
	////ステージのインスタンス。
	//m_bg = NewGO<SkinModelRender>(EnPriority_Render);
	////初期化。
	//m_bg->Init("Assets/modelData/levelTkm/L_SampleStage.tkm");
	////シャドウレシーバー。
	//m_bg->SetShadowReciever(true);

	m_level.Init("Assets/level/StageLevel.tkl", [&](LevelObjectData& objData) {
		return false;
	});

	return true;
}

void Stage::Update()
{
}