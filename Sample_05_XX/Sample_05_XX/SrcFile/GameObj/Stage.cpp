#include "stdafx.h"
#include "Stage.h"
#include "SoldierMob.h"

Stage::~Stage()
{
	//DeleteGO(m_bg);
}

bool Stage::Start()
{
	//レベルの初期化。
	//モブキャラ増やす場合は、ここも増やしてね。
	//インデックス。
	int spawnPointIndex = 0;
	//スポーン位置
	Vector3 spawnPos[10] = {
		{ -1500.0f, 0.0f, -500.0f },
		{ 800.0f, 0.0f, -1900.0f },
		{ -1500.0f, 0.0f, -500.0f },
		{ -700.0f, 0.0f, 1600.0f },
		{ 0.0, 0.0f, 0.0 }
	};
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
			//医者オブジェ。
			SkinModelRender* modelReder = NewGO<SkinModelRender>(EnPriority_3DModel);
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
			//ちょっとアニメーションずらす。
			modelReder->SetWaitTimeForAnim(m_mobCount * 61);
			m_mobCount++;
			return true;
		}
		if (wcsstr(objData.name, L"Area\0") != nullptr) {
			//エリアオブジェクトが見つかった。
			//モブキャラを作成。
			SoldierMob* mob = NewGO<SoldierMob>(EnPriority_3DModel);
			mob->SetPosition(objData.position);
			//モブキャラ増やしたら、spawnPointも増やしてね。
			mob->SetTarget(spawnPos[spawnPointIndex]);
			spawnPointIndex++;
			return true;
		}
		return false;
	});
	return true;
}

void Stage::Update()
{
}