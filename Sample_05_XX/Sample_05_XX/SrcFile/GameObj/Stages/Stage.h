#pragma once

#include "SrcFile/Fade.h"
#include "IStage.h"

class GamePlayer;
class Guide;
class SoldierMob;
class NaviMesh;
class RifleEnemy;
class StageGenerator;

/// <summary>
/// スタンバイステージクラス。
/// </summary>
class Stage : public IStage
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
	/// <summary>
	/// フォワードレンダリング。
	/// </summary>
	void ForwardRender() override;
	/// <summary>
	/// プレイヤーを設定。
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(GamePlayer* player)
	{
		m_player = player;
	}
	/// <summary>
	/// セキュリティーカメラの位置リストを取得。
	/// </summary>
	/// <returns></returns>
	std::vector<Vector3>& GetSecurityCameraPosList()
	{
		return m_securityCameraPosList;
	}
	/// <summary>
	/// セキュリティカメラの回転リストを取得。
	/// </summary>
	/// <returns></returns>
	std::vector<Quaternion>& GetSecurityCameraRotList()
	{
		return m_securityCameraRotList;
	}
	/// <summary>
	/// プレイヤーのリスポーン位置を取得。
	/// </summary>
	/// <returns></returns>
	Vector3& GetPlayerPos()
	{
		return m_playerRootPos;
	}
private:
	static const int m_enemyCount = 5;					//現在のエネミーの数。			
	SkinModelRender* m_modelRender[20];					//レンダー。
	RifleEnemy* m_rifleEnemys[m_enemyCount];			//ライフルエネミー。
	Level m_level;										//レベル。
	PhysicsStaticObject m_physicsStaticObject;			//静的オブジェクト。
	Vector3 m_playerRootPos = g_vec3Zero;				//レベルから引っ張って来たプレイヤーのリスポーン位置。titleからの遷移の都合上保存しておく。
	std::vector<Vector3> m_securityCameraPosList;		//監視カメラの位置リスト。
	std::vector<Quaternion> m_securityCameraRotList;	//監視カメラの向きリスト。
	//GamePlayer* m_player = nullptr;					//プレイヤー。
	//NaviMesh* m_naviMesh = nullptr;					//ナビゲーションメッシュ。
	//StageGenerator* m_stageGenerator = nullptr;		//ステージジェネレーター。

};

