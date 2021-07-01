#include "stdafx.h"
#include "BattleStage1.h"
#include "SrcFile/GameObj/GamePlayer.h"

BattleStage1::BattleStage1()
{
}

bool BattleStage1::Start()
{
	//ƒŒƒxƒ‹‚Ì‰Šú‰»B
	m_level.Init("Assets/level/GameStage_BattleMap1.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"PlayerRespawn") == true) {
			m_player->SetPos(objData.position);
			return true;
		}
		return false;
		});
	return true;
}

void BattleStage1::Update()
{
}
