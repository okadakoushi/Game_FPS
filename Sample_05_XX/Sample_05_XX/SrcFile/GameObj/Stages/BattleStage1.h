#pragma once

class GamePlayer;

class BattleStage1 : public IGameObject
{
public:
	BattleStage1();
	bool Start() override;
	void Update() override;
	/// <summary>
	/// プレイヤー設定。
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayer(GamePlayer* pl) {
		m_player = pl;
	}
private:
	Level m_level;
	GamePlayer* m_player = nullptr;
};

