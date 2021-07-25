#pragma once

class Rifle;

#include "IEnemy.h"
#include "SrcFile/GameObj/SoldierCollision.h"

enum EnEnemyState {
	EnEnemyState_Wandering,		//徘徊。
	EnEnemyState_Tracking,		//追跡。
	EnEnemyState_Attack,		//攻撃。
	EnEnemyState_AttackWait,	//攻撃待機時間。
	EnEnemyState_Damage,
	EnEnemyState_Death,			//死亡。
	EnEnemyState_Num
};

class RifleEnemy : public Enemy
{
public:
	~RifleEnemy();
	bool Start() override;
	void Update() override;
	void Move();
	void Attack();
	void Tracking();
	void Damage();
	void Death();
	/// <summary>
	/// プレイヤーを発見しているか。
	/// </summary>
	/// <returns></returns>
	bool IsFindPlayer();
	/// <summary>
	/// ステートを設定。
	/// </summary>
	/// <param name="state"></param>
	void SetState(const EnEnemyState& state)
	{
		m_enemyState = state;
	}
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
private:
	Rifle* m_rifle = nullptr;				//銃。
	float m_currentTime = 0.0f;				//タイマー。
	float COOLDOWN = 0.22f;					//クールダウン。
	SoldierCollision m_collision;	
	EnEnemyState m_enemyState = EnEnemyState_Wandering;	//エネミーのステート。
	int m_pathIndex = 0;						//
	const float m_FOV = 40.0f;					//視野角。
	const float m_VISION = 2500.0f;				//エネミーの視野範囲。
	Bone* m_head;								//頭ボーン。
	int m_hp = 100;								//HP。
	const int ATTACK = 10;						//攻撃力。
	const int MAX_RANDOM_AIM = 120;				//エネミーのAIMの最大乱れ値。
	int m_currentRondomAIM = MAX_RANDOM_AIM;	//エネミーの現在のAIMの乱れ値。
	bool m_isFindPlayer = false;				//一度でもプレイヤーを見つけていたか。
	Vector3 m_toPlayerDir = g_vec3Zero;			//エネミーのヘッドからプレイヤーに伸びるベクトル。
	Vector3 m_headPos = g_vec3Zero;				//ヘッドの位置。
	NaviMeshAgent m_agent;						//経路探査AI。
	std::vector<Vector3> m_paths;				//パス。
};

