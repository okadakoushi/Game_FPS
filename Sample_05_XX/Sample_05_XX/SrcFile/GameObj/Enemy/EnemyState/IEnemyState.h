#pragma once

//#include "SrcFile/GameObj/Enemy/RifleEnemy.h"
class RifleEnemy;

/// <summary>
/// エネミーのステートを管理するインターフェイスクラス。
/// </summary>
class IEnemyState
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	/// <param name="enemy">エネミー。</param>
	IEnemyState(RifleEnemy* enemy)
		: m_enemy(enemy)
	{
	}
	virtual void Enter() = 0;
	virtual void Leave() = 0;
	virtual void Update() = 0;
	/// <summary>
	/// 前フレームでアップデートされたステートを取得。
	/// </summary>
	/// <returns></returns>
	IEnemyState* GetLasetUpdateState()
	{
		return m_lastUpdateState;
	}
protected:
	RifleEnemy* m_enemy = nullptr;
	IEnemyState* m_lastUpdateState = nullptr;	//前フレームのステート。
};

