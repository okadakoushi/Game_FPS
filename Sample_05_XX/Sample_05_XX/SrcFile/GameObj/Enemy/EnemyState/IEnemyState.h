#pragma once

#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

/// <summary>
/// エネミーのステートを管理するインターフェイスクラス。
/// </summary>
class IEnemyState : public RifleEnemy
{
public:
	virtual void Enter() = 0;
	virtual void Leave() = 0;
	virtual void Update() = 0;
};

