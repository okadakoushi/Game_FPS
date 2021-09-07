#pragma once

class Rifle;
class GamePlayer;
class SoldierCollision;

class IEnemyState;
class EnemyAttackState;
class EnemyDamageState;
class EnemyDethState;
class EnemyTrackingState;
class EnemyWanderingState;

//#include "IEnemy.h"
#include "SrcFile/GameObj/SoldierCollision.h"

//#include "EnemyState/IEnemyState.h"
//#include "EnemyState/EnemyAttackState.h"
//#include "EnemyState/EnemyDamageState.h"
//#include "EnemyState/EnemyDethState.h"
//#include "EnemyState/EnemyTrackingState.h"
//#include "EnemyState/EnemyWanderingState.h"

enum EnEnemyAnimation {
	EnEnemyAnimation_Walk,
	EnEnemyAnimation_Run,
	EnEnemyAnimation_Damage,
	EnEnemyAnimation_Shoot,
	EnEnemyAnimation_Death,
};

class RifleEnemy : public IGameObject
{
public:
	~RifleEnemy();
	bool Start() override;
	void Update() override;
	/// <summary>
	/// 移動。
	/// </summary>
	/// <remarks>
	///	他ステートクラスからも参照されるためpublic。
	/// </remarks>
	void Move();
	/// <summary>
	/// プレイヤーを発見しているか。
	/// </summary>
	/// <returns></returns>
	bool IsFindPlayer();
	/// <summary>
	/// ステートを設定。
	/// </summary>
	/// <param name="state"></param>
	void ChangeState(IEnemyState* state);
	/// <summary>
	/// ダメージを受ける。
	/// </summary>
	void GetDamage();
	/// <summary>
	/// 位置を設定。
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(Vector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// パスを登録。
	/// </summary>
	/// <param name="path"></param>
	void RegistPath(const Vector3& path)
	{
		m_paths.push_back(path);
	}
	/// <summary>
	/// 位置を取得。
	/// </summary>
	/// <returns></returns>
	Vector3& GetPosition()
	{
		return m_pos;
	}
	/// <summary>
	/// 回転を取得。
	/// </summary>
	/// <returns></returns>
	Quaternion& GetRotation()
	{
		return m_rot;
	}
	/// <summary>
	/// モデルレンダーを取得。
	/// </summary>
	/// <returns></returns>
	SkinModelRender* GetRender()
	{
		return m_modelRender;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	NaviMeshAgent& GetAgent()
	{
		return m_agent;
	}
	/// <summary>
	/// ナビメッシュを設定。
	/// </summary>
	/// <param name="mesh"></param>
	void SetNaviMesh(NaviMesh* mesh)
	{
		m_naviMesh = mesh;
	}
	/// <summary>
	/// プレイヤー見つけているかのフラグを設定。
	/// </summary>
	/// <param name="flag"></param>
	void SetFindPlayerFlag(const bool& flag)
	{
		m_isMissingPlayer = flag;
	}
	/// <summary>
	/// プレイヤーのポインタを取得。
	/// </summary>
	/// <returns>player</returns>
	GamePlayer* GetPlayerPtr()
	{
		return m_player;
	}
	/// <summary>
	/// 頭の位置を取得。
	/// </summary>
	/// <returns></returns>
	Vector3& GetHeadPos()
	{
		return m_headPos;
	}
	/// <summary>
	/// 武器を取得。
	/// </summary>
	/// <returns></returns>
	Rifle* GerRifle()
	{
		return m_rifle;
	}
	/// <summary>
	/// ヒットポイントを取得。
	/// </summary>
	/// <returns></returns>
	int& GetHP()
	{
		return m_hp;
	}
	/// <summary>
	/// 見失ったフラグを設定。
	/// </summary>
	/// <param name="flag"></param>
	void SetIsMissingFlag(const bool& flag)
	{
		m_isMissingPlayer = flag;
	}
	/// <summary>
	/// コリジョンを取得。
	/// </summary>
	/// <returns></returns>
	SoldierCollision& GetCollision()
	{
		return m_collision;
	}
protected:
	Rifle* m_rifle = nullptr;				//銃。
	SoldierCollision m_collision;	
	int m_pathIndex = 0;						//
	float m_FOV = 40.0f;						//視野角。
	float m_vision = 700.0f;					//エネミーの視野範囲。
	Bone* m_head;								//頭ボーン。
	int m_hp = 100;								//HP。
	bool m_isMissingPlayer = false;				//プレイヤーを見失った。
	Vector3 m_toPlayerDir = g_vec3Zero;			//エネミーのヘッドからプレイヤーに伸びるベクトル。
	Vector3 m_headPos = g_vec3Zero;				//ヘッドの位置。
	NaviMeshAgent m_agent;						//経路探査AI。
	std::vector<Vector3> m_paths;				//パス。
	//基本Param。
	SkinModelRender* m_modelRender;							//モデルレンダー。
	Vector3							m_pos = { 50.0f, 0.0f, 0.0f };			//位置。
	Quaternion						m_rot = g_quatIdentity;					//回転。
	Vector3							m_scale = { 1.5f, 1.5f, 1.5f };			//拡大。					
	NaviMesh* m_naviMesh = nullptr;
	SoundSource* m_findSE;						//見つけた
	SoundSource* m_footStep = nullptr;			//足音。
	GamePlayer* m_player = nullptr;				//プレイヤー。
	SpriteRender* m_findMark = nullptr;			//！マーク。
	float m_rayTestTimer = 0.0f;				//現在のレイテストタイマー。
	const float RAY_TEST_INTERVAL = 0.5f;		//レイテトを行う間隔。

protected:
	IEnemyState*		m_enemyState;			//ステート。
	EnemyAttackState*	m_attackState;			//攻撃
	EnemyDamageState*	m_damageState;			//ダメージ
	EnemyDethState*		m_deathState;			//死亡。
	EnemyWanderingState* m_wanderingState;		//徘徊。
	EnemyTrackingState*	m_trackingState;		//追跡。
};

