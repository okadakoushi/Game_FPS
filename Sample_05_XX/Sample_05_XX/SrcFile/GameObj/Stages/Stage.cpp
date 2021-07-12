#include "stdafx.h"
#include "Stage.h"
#include "SrcFile/GameObj/SoldierMob.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/guide.h"

Stage::Stage()
{
	//初期化。
	for (auto* render : m_modelRender) {
		render = nullptr;
	}
	for (auto* soldier : m_soldierMob) {
		soldier = nullptr;
	}
	delete m_naviMesh;
}


Stage::~Stage()
{
	//DeleteGO(m_bg);
}

void Stage::OnDestroy()
{
	DeleteGO(m_guide);
	//レンダー全消し。
	for (auto* render : m_modelRender) {
		if (render != nullptr) {
			DeleteGO(render);
		}
	}
	//兵士全消し。
	for (auto* soldier : m_soldierMob) {
		if (soldier != nullptr) {
			DeleteGO(soldier);
		}
	}
}

bool Stage::Start()
{
	//ナビメッシュ作成。
	m_naviMesh = new NaviMesh;
	m_naviMesh->Load("Assets/nvm/StanbyStage.nvm");
	m_naviMesh->InitRender();

	int spawnPointIndex = 0;	//Mobソルジャー用添字。
	int renderIndex = 0;		//レンダー用の添字。
	//スポーン位置
	Vector3 spawnPos[10] = {
		{ -1500.0f, 0.0f, -500.0f },
		{ 800.0f, 0.0f, -1900.0f },
		{ -1500.0f, 0.0f, -500.0f },
		{ -700.0f, 0.0f, 1600.0f },
		{ 0.0, 0.0f, 0.0 }
	};
	//レベルの初期化。
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
		if (objData.EqualObjectName(L"PlayerRespawn") == true) {
			m_player->SetPos(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"GuideRespawn") == true) {
			m_guide = NewGO<Guide>(EnPriority_3DModel, "Guide");
			m_guide->SetPos(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"soldierDoc") == true) {
			//医者オブジェ。
			m_modelRender[m_mobCount] = NewGO<SkinModelRender>(EnPriority_3DModel);
			const char* tkaFilePaths[]{
				"Assets/animData/soldierMob/talk.tka"
			};
			m_modelRender[m_mobCount]->Init("Assets/modelData/Chara/soldierDoc.tkm" ,tkaFilePaths);
			m_modelRender[m_mobCount]->SetRenderMode(enRenderMode_Skin);
			m_modelRender[m_mobCount]->SetPosition(objData.position);
			m_modelRender[m_mobCount]->SetScale(objData.scale);
			m_modelRender[m_mobCount]->SetRotation(objData.rotatatin);
			m_modelRender[m_mobCount]->SetShadwoCaster(true);
			m_modelRender[m_mobCount]->SetShadowReciever(true);
			//ちょっとアニメーションずらす。
			m_modelRender[m_mobCount]->SetWaitTimeForAnim(m_mobCount * 61);
			m_mobCount++;
			return true;
		}
		if (wcsstr(objData.name, L"Area\0") != nullptr) {
			//エリアオブジェクトが見つかった。
			//モブキャラを作成。
			m_soldierMob[spawnPointIndex] = NewGO<SoldierMob>(EnPriority_3DModel);
			m_soldierMob[spawnPointIndex]->SetPosition(objData.position);
			//モブキャラ増やしたら、spawnPointも増やしてね。
			m_soldierMob[spawnPointIndex]->SetTarget(spawnPos[spawnPointIndex]);
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

void Stage::ForwardRender()
{
#ifndef MASTER
	m_naviMesh->Render();
#endif
}
