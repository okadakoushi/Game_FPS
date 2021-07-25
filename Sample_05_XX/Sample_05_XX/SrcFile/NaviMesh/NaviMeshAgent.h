#pragma once

/// <summary>
/// A*を利用したAI。
/// </summary>
/// <code>
/// Init();
/// SetPath();
/// Move();
/// </code>
class NaviMeshAgent
{
public:
	/// <summary>
	/// エージェントを初期化。
	/// </summary>
	/// <param name="m_render">参照レンダー。</param>
	/// <param name="mesh">参照メッシュ。</param>
	/// <param name="target">目標地点。</param>
	/// <param name="randomMove">無作為に移動地点を選ぶか。</param>
	void Init(SkinModelRender* m_render, NaviMesh* mesh, const bool& randMove);
	/// <summary>
	/// エージェントの移動速度を設定。
	/// </summary>
	/// <param name="speed"></param>
	void SetAgentSpeed(float& speed)
	{
		m_speed = speed;
	}
	/// <summary>
	/// 到着地点のパスを登録。
	/// </summary>
	/// <param name="pathList">パスリスト。</param>
	/// <param name="randomMove">パスをランダムで移動するか。</param>
	void RegistPath(const Vector3& pos)
	{
		m_paths.push_back(pos);
	}
	/// <summary>
	/// 目標地点を設定。
	/// </summary>
	/// <param name="target"></param>
	void SetTargetPos(const Vector3& target)
	{
		m_targetPos = target;
		m_isPathMove = false;
		m_dirty = false;
	}
	/// <summary>
	/// 移動方向を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& MoveDirection() const
	{
		return m_toNextCell;
	}
	/// <summary>
	/// 移動処理。
	/// <para>移動方向に応じて回転も計算している。</para>
	/// </summary>
	void Move();

	Vector3& GetAgentPos()
	{
		return m_agentPos;
	}
	Quaternion& GetAgentRot()
	{
		return m_agentRot;
	}
private:
	SkinModelRender*				m_modelRender = nullptr;	//参照モデル。
	NaviMesh*						m_naviMesh = nullptr;		//ナビメッシュ。
	float							m_speed = 10.0f;				//エージェントの移動速度。
	AStar							m_astar;					//経路探査。
	NaviMesh::Cell*					m_nodeCell = nullptr;		//ノードセル。
	bool							m_dirty = false;			//更新必要。
	std::vector<NaviMesh::Cell*>	m_nodeList;					//ノードリスト。		
	Vector3							m_targetPos = g_vec3Zero;	//目的地点。
	Vector3							m_toNextCell;				//次のセルに向かう方位ベクトル。
	std::vector<Vector3>			m_paths;					//探索パス。todo:配列。
	bool							m_isPathMoveRandom = false;	//パスをランダムで移動する？
	bool							m_isPathMove = true;		//パスで移動。
	int								m_pathIndex = 0;			//パスの添字。
	Vector3							m_agentPos;
	Quaternion						m_agentRot;					
};

