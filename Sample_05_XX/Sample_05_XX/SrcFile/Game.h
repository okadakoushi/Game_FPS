#pragma once

class Stage;
class GamePlayer;
class UIs;
class Effect;
class Enemy;
class Rifle;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	bool Start() override;
	void Update() override;
private:
	Stage* m_stage = nullptr;			//ステージ。
	GamePlayer* m_player = nullptr;		//プレイヤー。
	Rifle* m_wepon;						//武器。
	Level m_level;						//レベル。
	UIs* m_UIs = nullptr;				//UI。
	Enemy* m_enemy = nullptr;			//エネミー。
	myEngine::Effect* m_effect = nullptr;	//エフェクト。
};

