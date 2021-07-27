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
	/// プレイヤーをFindして取得。
	/// <para>複数回使用しないように。</para>
	/// </summary>
	/// <returns></returns>
	GamePlayer* GetPlayerForUseFind() const;
	friend class EnemyAttackState;
	friend class EnemyDamageState;
	friend class EnemyDethState;
	friend class EnemyWanderingState;
	friend class EnemyTrackingState;
protected:
	Rifle* m_rifle = nullptr;				//銃。
	float m_currentTime = 0.0f;				//タイマー。
	float COOLDOWN = 0.22f;					//クールダウン。
	SoldierCollision m_collision;	
	int m_pathIndex = 0;						//
	const float m_FOV = 40.0f;					//視野角。
	const float m_VISION = 2500.0f;				//エネミーの視野範囲。
	Bone* m_head;								//頭ボーン。
	int m_hp = 100;								//HP。
	const int ATTACK = 10;						//攻撃力。
	const int MAX_RANDOM_AIM = 120;				//エネミーのAIMの最大乱れ値。
	int m_currentRondomAIM = MAX_RANDOM_AIM;	//エネミーの現在のAIMの乱れ値。
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
	//
	GamePlayer* m_player = nullptr;						//プレイヤー。
private:
	IEnemyState*		m_enemyState;			//ステート。
	EnemyAttackState*	m_attackState;			//攻撃
	EnemyDamageState*	m_damageState;			//ダメージ
	EnemyDethState*		m_deathState;			//死亡。
	EnemyWanderingState* m_wanderingState;		//徘徊。
	EnemyTrackingState*	m_trackingState;		//追跡。
};

