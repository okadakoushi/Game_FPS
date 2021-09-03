#pragma once

class GamePlayer;
class Rifle;
class RifleEnemy;

class PlayerUIs : public IGameObject
{
public:
	PlayerUIs();
	~PlayerUIs();
	bool Start() override;
	void Update() override;
	void RenderHUD() override;
public:
	/// <summary>
	/// プレイヤーを見つけているエネミーを追加。
	/// </summary>
	/// <param name="enemy">Enemy</param>
	void AddFindList(RifleEnemy* enemy)
	{
		m_findPlayerEnemyList.push_back(enemy);
	}

	void ActiveProcess(bool flag);
private:
	//Finds。
	GamePlayer* m_player = nullptr;								//playerPtr。
	//UIs。
	SpriteRender*				m_reticule = nullptr;			//レティクル。
	SpriteRender*				m_mainWeponImage = nullptr;		//メインウェポンUI。
	SpriteRender*				m_damageEffectSprite = nullptr;	//ダメージエフェクトのスプライト。
	SpriteRender*				m_LeftAmoImage[3];				//3つのスプライトで残弾を表示。
	SpriteRender*				m_uiBoxForHP = nullptr;			//HP用UIBOX。
	SpriteRender*				m_uiBoxForAmo = nullptr;		//Amo用UIBOX。
	Sprite						m_enemyDirectionSprite[5];		//エネミーの方向スプライト。リストの削除を操作するためNotRender。
	//texts。
	myEngine::CFontRender*		m_hpUIText = nullptr;			//HPテキスト。
	wchar_t						m_hpTex[16];					//HPテキスト格納用。
	myEngine::CFontRender*		m_LeftAmoText = nullptr;		//残弾テキスト。
	wchar_t						m_leftAmoTex[16];				//残弾テキスト格納用。
	myEngine::CFontRender*		m_LeftEnemyCount = nullptr;		//残り敵数。
	wchar_t						m_leftEnemyCountTex[16];		//残り敵数テキスト格納用。
	//Param。
	float						m_damageEffectArea = 0.0f;		//ダメージエフェクトエリア。
	std::vector<RifleEnemy*>	m_findPlayerEnemyList;			//プレイヤーを発見しているエネミーのリスト。
	const Vector2				m_saveFrame = { 30.0f, 30.0f };	//セーブフレーム。
};

