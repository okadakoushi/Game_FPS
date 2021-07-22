#pragma once

#include "SrcFile/Fade.h"

class GamePlayer;
class Guide;
class SoldierMob;
class NaviMesh;
class RifleEnemy;
class StageGenerator;

/// <summary>
/// スタンバイステージクラス。
/// </summary>
class Stage : public IGameObject
{
public:
	Stage();
	~Stage();
	/// <summary>
	/// 削除。
	/// </summary>
	void OnDestroy() override;
	/// <summary>
	/// スタート。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	void ForwardRender() override;
	/// <summary>
	/// プレイヤーを設定。
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(GamePlayer* player)
	{
		m_player = player;
	}
private:
	static const int m_enemyCount = 5;			//現在のエネミーの数。			
	SkinModelRender* m_modelRender[20];			//レンダー。
	RifleEnemy* m_rifleEnemys[m_enemyCount];	//ライフルエネミー。
	Level m_level;								//レベル。
	int m_mobCount = 0;							//モブカウント。
	PhysicsStaticObject m_physicsStaticObject;	//静的オブジェクト。
	GamePlayer* m_player = nullptr;				//プレイヤー。
	Guide* m_guide = nullptr;					//ガイド。
	NaviMesh* m_naviMesh = nullptr;				//ナビゲーションメッシュ。
	Fade m_fade;								//フェード。
	StageGenerator* m_stageGenerator = nullptr;	//ステージジェネレーター。
};

