#pragma once
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

	void ForwardRender() override;
private:
	SkinModelRender* m_modelRender;			//モデルレンダー。
	Vector3 m_pos = g_vec3Zero;				//位置。
	Quaternion m_rot = g_quatIdentity;		//回転。
	Vector3 m_targetPos = {0, 0, -800};		//目的地点。
	const float m_spped = 1.0f;				//スピード。
	AStar m_astar;							//経路探査。
	NaviMesh::Cell* m_nodeCell = nullptr;	//ノードセル。
	bool m_dirty = false;					//更新必要？
	std::vector<NaviMesh::Cell*> m_nodeList;		//ノードリスト。
	float m_kakudo = 0.0f;
};

