#pragma once

class Stage;
class GamePlayer;
class UIs;
class Effect;
class SoldierMob;
class RifleEnemy;
class Rifle;
class GameCamera;
class Map;
class SkyBox;
class Guide;
class StageGenerator;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	bool Start() override;
	void Update() override;
	void RenderHUD() override;

private:
	StageGenerator* m_stageGenerator = nullptr;	//ステージ生成オブジェクト。
	Stage* m_stage = nullptr;			//ステージ。
	GamePlayer* m_player = nullptr;		//プレイヤー。
	GameCamera* m_camera = nullptr;		//カメラ。
	Rifle* m_wepon;						//武器。
	RifleEnemy* m_enemy = nullptr;		//敵。	
	Level m_level;						//レベル。
	UIs* m_UIs = nullptr;				//UI。
	Map* m_map;							//ミニマップ。
	SkyBox* m_sky = nullptr;			//空。
	myEngine::Effect* m_effect = nullptr;			//エフェクト。
	myEngine::CFont m_font;
};
