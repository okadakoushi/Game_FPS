#pragma once

class GamePlayer;

class Enemy : public IGameObject
{
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();
	/// <summary>
	/// 位置を設定。
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// ターゲット(A*のゴールノード)を指定。
	/// </summary>
	/// <param name="pos"></param>
	void SetTarget(const Vector3& pos)
	{
		m_targetPos = pos;
	}
private:
	/// <summary>
	/// プレイヤーが視野角内に存在しているかどうか。
	/// </summary>
	/// <returns>true,false</returns>
	bool IsFindPlayer();
private:
	enum EnEnemyState
	{
		En_Move,
		En_Hello,
		En_Idle
	};
	EnEnemyState m_currentState = EnEnemyState::En_Move;	//現在のステート。
	SkinModelRender* m_modelRender;			//モデルレンダー。
	Vector3 m_pos = g_vec3Zero;				//位置。
	Quaternion m_rot = g_quatIdentity;		//回転。
	Vector3 m_targetPos = {1230, 0, 960};		//目的地点。
	Vector3 m_nextTarget = g_vec3Zero;		//次の目的地。
	const float m_spped = 1.0f;				//スピード。
	AStar m_astar;							//経路探査。
	NaviMesh::Cell* m_nodeCell = nullptr;	//ノードセル。
	bool m_dirty = false;					//更新必要？
	std::vector<NaviMesh::Cell*> m_nodeList;		//ノードリスト。
	float m_kakudo = 0.0f;
	GamePlayer* m_player = nullptr;			//プレイヤー。
	const float m_helloTime = 15.0f;	//挨拶クールタイム。
	float time = m_helloTime;			//タイム。
	const float m_FOV = 45.0f;			//エネミー側の視野角。
	const float m_vision = 500.0f;		//エネミーのビジョン。
};

