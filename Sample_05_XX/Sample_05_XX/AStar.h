#pragma once

#include "SrcFile/NaviMesh/NaviMesh.h"

/// <summary>
/// 経路探査用の計算を行うクラス。
/// </summary>
class AStar
{
	using cell = NaviMesh::Cell;
	using openCellList = vector<cell*>;
	using closeCellList = vector<cell*>;
public:
	/// <summary>
	/// スタート位置からゴール位置までの経路を探索。
	/// </summary>
	/// <param name="start">スタート位置。</param>
	/// <param name="goal">ゴール位置。</param>
	/// <param name="cell">セル。</param>
	void AStarSearch(Vector3& start, Vector3& goal, NaviMesh::Cell& cell);
private:
	openCellList m_openCellList;		//開いてるセルリスト。
	closeCellList m_closeCellList;		//閉じてるセルリスト。
};

