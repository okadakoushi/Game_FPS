#pragma once

#include "IStage.h"

class GamePlayer;
class NaviMesh;
class RifleEnemy;
class StageGenerator;
class Fade;

class BattleStage1 : public IStage
{
public:
	BattleStage1();
	~BattleStage1();
	bool Start() override;
	void Update() override;
	void ForwardRender() override;
	/// <summary>
	/// プレイヤー設定。
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayer(GamePlayer* pl) {
		m_player = pl;
	}
private:
	const int STAGE_ENEMY_XOUNT = 6;
	Level m_level;
	RifleEnemy* m_rifleEnemy[8];	//エネミー。
	int m_enemyCount;
	//NaviMesh* m_naviMesh = nullptr;	//使用するナビメッシュ。
	//StageGenerator* m_stageGenerator = nullptr;
	//GamePlayer* m_player = nullptr;
	
	//CheatCode
	bool m_deleteStageFlag = false;
};

