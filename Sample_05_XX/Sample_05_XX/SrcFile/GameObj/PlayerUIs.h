#pragma once

class GamePlayer;
class Rifle;

class PlayerUIs : public IGameObject
{
public:
	PlayerUIs();
	~PlayerUIs();
	bool Start() override;
	void Update() override;
	void RenderHUD() override;
private:
	//Finds。
	GamePlayer* m_player = nullptr;						//playerPtr。
	//UIs。
	SpriteRender*		m_reticule = nullptr;			//レティクル。
	SpriteRender*		m_mainWeponImage = nullptr;		//メインウェポンUI。
	myEngine::CFont		m_hpUIText;						//HPテキスト。
	myEngine::CFont		m_LeftAmoText;					//残弾テキスト。
	SpriteRender*		m_LeftAmoImage[2];				//3つのスプライトで残弾を表示。
	myEngine::CFont		m_LeftEnemyCount;				//残り敵数。
};

