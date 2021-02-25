#pragma once

class NaviMesh;
class AStar;

/// <summary>
/// テスト用ゲームクラス。
/// #ifdef GameTestでインスタンス化。
/// </summary>
class TestGame : public IGameObject
{
public:
	/// <summary>
	/// スタート。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
private:
	Vector3 m_start = { 0,0,0 };
	Vector3 m_end = { 200,0,0 };
};

