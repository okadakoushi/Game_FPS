#pragma once

class Stage;
class GamePlayer;
class UIs;
class Effect;
class Enemy;
class Rifle;
class GameCamera;
class Map;

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
	GameCamera* m_camera = nullptr;		//カメラ。
	Rifle* m_wepon;						//武器。
	Level m_level;						//レベル。
	UIs* m_UIs = nullptr;				//UI。
	Enemy* m_enemy = nullptr;			//エネミー。
	Map* m_map;							//ミニマップ。
	myEngine::Effect* m_effect = nullptr;	//エフェクト。
};
