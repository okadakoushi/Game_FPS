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
		return false;
	});
	return true;
}

void Stage::Update()
{
}