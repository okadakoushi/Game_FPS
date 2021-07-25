#pragma once

#include "SrcFile/GameObject/IGameObject.h"
#include "SrcFile/SkinModelRender.h"
#include "SrcFile/GameObj/GamePlayer.h"

class NaviMesh;

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
	virtual ~Enemy() {};
	virtual bool Start() override = 0;
	virtual void Update() override = 0;
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
	GamePlayer* GetPlayerForUseFind() const 
	{
		return FindGO<GamePlayer>("Player");
	}
protected:
	//基本Param。
	SkinModelRender*				m_modelRender;							//モデルレンダー。
	Vector3							m_pos = {50.0f, 0.0f, 0.0f};			//位置。
	Quaternion						m_rot = g_quatIdentity;					//回転。
	Vector3							m_scale = { 1.5f, 1.5f, 1.5f };			//拡大。					
	NaviMesh*						m_naviMesh = nullptr;
	//
	GamePlayer*						m_player = nullptr;						//プレイヤー。
};