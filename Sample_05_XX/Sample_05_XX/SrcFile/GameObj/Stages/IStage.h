#pragma once

class NaviMesh;
class StageGenerator;
class GamePlayer;

#include "SrcFile/GameObject/IGameObject.h"

/// <summary>
/// ステージのインターフェイスクラス。
/// </summary>
/// <remarks>
/// ステージ処理についてコードの重複が多く
/// それをポリモーフィズムで解決するため作成。
/// </remarks>
class IStage : public IGameObject
{
public:
	/// <summary>
	/// スタート。
	/// </summary>
	/// <returns></returns>
	virtual bool Start() override = 0;
	/// <summary>
	/// 更新。
	/// </summary>
	virtual void Update() override = 0;
	/// <summary>
	/// ナビゲーションメッシュを設定。
	/// </summary>
	/// <param name="naviMesh"></param>
	void SetNaviMesh(NaviMesh* naviMesh)
	{
		m_naviMesh = naviMesh;
	}
	/// <summary>
	/// ステージジェネレーターを設定。
	/// </summary>
	/// <param name="stageGenerator"></param>
	void SetStageGenerator(StageGenerator* stageGenerator)
	{
		m_stageGenerator = stageGenerator;
	}
	/// <summary>
	/// プレイヤーを設定。
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(GamePlayer* player)
	{
		m_player = player;
	}
protected:
	NaviMesh* m_naviMesh = nullptr;
	StageGenerator* m_stageGenerator = nullptr;
	GamePlayer* m_player = nullptr;
	int m_currentEnemyCount = 0;
};