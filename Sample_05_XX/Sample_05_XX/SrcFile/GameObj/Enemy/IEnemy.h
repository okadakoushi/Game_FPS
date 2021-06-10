#pragma once

#include "SrcFile/GameObject/IGameObject.h"
#include "SrcFile/SkinModelRender.h"

/// <summary>
/// エネミーのインターフェイス。
/// </summary>
/// <remarks>
/// factoryMethod使ってEnemyの種類とかランダム配置したいよね。
/// templateMethodパターンはユーザー視点からみていらない、なんなら邪魔。
/// メンバ継承(state,render)はどうなんだろう。efectiveには「protectedはカプセル化を進めるものではない」とあるけど。
/// </remarks>
class Enemy : public IGameObject{
public:
	enum EnEnemyState {
		EnEnemyState_Wandering,		//徘徊。
		EnEnemyState_Tracking,		//追跡。
		EnEnemyState_Attack,		//攻撃。
		EnEnemyState_AttackWait,	//攻撃待機時間。
		EnEnemyState_Num
	};
public:
	virtual ~Enemy() {};
	virtual bool Start() override = 0;
	virtual void Update() override = 0;
};