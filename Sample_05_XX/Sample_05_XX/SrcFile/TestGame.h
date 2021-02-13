#pragma once
#include "NaviMesh/NaviMesh.h"

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
	//NaviMesh m_naviMeshBase;
	NaviMesh m_naviMesh;
};

