#pragma once

class Stage;
class GamePlayer;
class UIs;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	bool Start() override;
	void Update() override;
private:
	Stage* m_stage = nullptr;		//ステージ。
	GamePlayer* m_player = nullptr;	//プレイヤー。
	Level m_level;					//レベル。
	UIs* m_UIs;						//UI。
};

