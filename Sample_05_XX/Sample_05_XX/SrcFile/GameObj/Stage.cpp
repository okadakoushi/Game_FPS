#include "stdafx.h"
#include "Stage.h"
#include "Enemy.h"

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
		//レベルからスポーン情報持ってきて、そこにスポーンさせる。
		//todo:enumとかでうまく管理できるかな？
		if (objData.EqualObjectName(L"noneArea") == true) {
			Enemy* enemy = NewGO<Enemy>(EnPriority_3DModel);
			enemy->SetPosition(objData.position);
			enemy->SetTarget({ -1500.0f, 0.0f, -500.0f });
			return true;
		}		
		if (objData.EqualObjectName(L"factoryArea") == true) {
			//ノーンエリアに出す。
			Enemy* enemy = NewGO<Enemy>(EnPriority_3DModel);
			enemy->SetPosition(objData.position);
			enemy->SetTarget({ 800.0f, 0.0f, -1900.0f });
			return true;
		}		
		if (objData.EqualObjectName(L"loadArea") == true) {
			//ノーンエリアに出す。
			Enemy* enemy = NewGO<Enemy>(EnPriority_3DModel);
			enemy->SetPosition(objData.position);
			enemy->SetTarget({ -1500.0f, 0.0f, -500.0f });
			return true;
		}		
		if (objData.EqualObjectName(L"tentArea") == true) {
			//ノーンエリアに出す。
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