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
private:
	Rifle* m_rifle = nullptr;				//銃。
	float m_currentTime = 0.0f;				//タイマー。
	float COOLDOWN = 0.19f;					//クールダウン。
	SoldierCollision m_collision;	
	EnEnemyState m_enemyState = EnEnemyState_Wandering;	//エネミーのステート。
	std::vector<Vector3> m_paths;				//探索パス。todo:配列。
	int m_pathIndex = 0;						//
	const float m_FOV = 20.0f;					//視野角。
	const float m_VISION = 1500.0f;				//エネミーの視野範囲。
	Bone* m_head;								//頭ボーン。
	Vector3 m_toNextCell;						//次のセルに向かう方位ベクトル。
	int m_hp = 100;								//HP。
	const int MAX_RANDOM_AIM = 120;				//エネミーのAIMの最大乱れ値。
	int m_currentRondomAIM = MAX_RANDOM_AIM;	//エネミーの現在のAIMの乱れ値。
};

