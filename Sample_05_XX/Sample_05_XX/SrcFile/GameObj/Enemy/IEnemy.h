#pragma once

#include "SrcFile/GameObject/IGameObject.h"
#include "SrcFile/SkinModelRender.h"

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
	enum EnEnemyState {
		EnEnemyState_Wandering,		//徘徊。
		EnEnemyState_Tracking,		//追跡。
		EnEnemyState_Attack,		//攻撃。
		EnEnemyState_AttackWait,	//攻撃待機時間。
		EnEnemyState_Death,			//死亡。
		EnEnemyState_Num
	};
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
protected:
	//基本Param。
	EnEnemyState					m_enemyState = EnEnemyState_Wandering;	//エネミーのステート。
	SkinModelRender*				m_modelRender;							//モデルレンダー。
	Vector3							m_pos = g_vec3Zero;						//位置。
	Quaternion						m_rot = g_quatIdentity;					//回転。
	Vector3							m_scale = { 1.5f, 1.5f, 1.5f };			//拡大。
	const float						m_spped = 1.0f;							//スピード。
	//経路探査Param。
	NaviMesh*						m_naviMesh = nullptr;					//ナビメッシュ。
	AStar							m_astar;								//経路探査。
	NaviMesh::Cell*					m_nodeCell = nullptr;					//ノードセル。
	bool							m_dirty = false;						//更新必要？
	std::vector<NaviMesh::Cell*>	m_nodeList;								//ノードリスト。		
	Vector3							m_targetPos = g_vec3Zero;				//目的地点。
	Vector3							m_nextTarget = g_vec3Zero;				//次の目的地。
};