#include "stdafx.h"
#include "AStar.h"

void AStar::CreateCellList(Vector3& start, Vector3& goal, std::vector<cell>& cells)
{
	//リストをクリア。
	m_openCellList.clear();
	m_closeCellList.clear();
	m_noneCellList.clear();
	//セルをリストに積んでいく。
	for (int cellCount = 0; cellCount < cells.size(); cellCount++) {
		//セルのパラメーターを初期化。
		cells[cellCount].costFromStart = 0.0f;
		cells[cellCount].costToEnd = 0.0f;
		cells[cellCount].m_parent = nullptr;
		cells[cellCount].totalCost = 0.0f;
		//なにもされてないセルリストに積む。
		m_noneCellList.push_back(&cells[cellCount]);
	}
	//最小距離。
	float startMin, goalMin;
	startMin = FLT_MAX;
	goalMin = FLT_MAX;
	//最小距離だったセル番号。
	int startCellNo, goalCellNo;
	startCellNo = -1;
	goalCellNo = -1;
	//開くセルを求める。
	for (int cellCount = 0; cellCount < cells.size(); cellCount++) {
		//ゴール地点から、１番距離が近いセルを求める。
		Vector3 goalToCenter = cells[cellCount].m_CenterPos - goal;
		float dist = goalToCenter.Length();
		if (dist < goalMin) {
			//更新。
			goalMin = dist;
			goalCellNo = cellCount;
			m_goalCell = &cells[cellCount];
		}
		//スタート地点から、一番距離が近いセルを求める。
		Vector3 startToCenter = cells[cellCount].m_CenterPos - start;
		dist = startToCenter.Length();
		if (dist < startMin) {
			//更新。
			startMin = dist;
			startCellNo = cellCount;
			m_startCell = &cells[cellCount];
		}
	}

	//コスト計算。スタート位置のコストって別にいらんかも？
	Vector3 startToGoal = m_goalCell->m_CenterPos - m_startCell->m_CenterPos;
	float dist = startToGoal.Length();
	m_startCell->costToEnd = dist;
	//始点はオープンされる。
	moveCellList(m_openCellList, m_startCell, NaviMesh::EnOpenList);
}

float AStar::ClacTraverseCost(cell* node, cell* reserchNode)
{
	//コスト。
	float cost = 0.0f;
	//前のノードからのコストを計算。
	Vector3 dist = reserchNode->m_CenterPos - node->m_CenterPos;
	return cost = dist.Length();
}

NaviMesh::Cell* AStar::CreateNode()
{
	while (m_openCellList.size() != 0) {
		//調査対象セル。
		cell* reserchCell = nullptr;
		//コスト。
		float cost = FLT_MAX;
		//開かれてるセルのうち、一番コストが低いもののが調査する対象セルとなる。
		for (auto cell : m_openCellList) {//todo:このループの書き方の方が綺麗なのでは？
			if (cell->costToEnd < cost) {
				//コストが低いので調査セルを変える。
				reserchCell = cell;
				cost = reserchCell->costToEnd;
			}
		}//reserchCell asking completed.

		//調査セル求まったので経路のノードを作成していく。
		if (reserchCell == m_goalCell) {
			//調査のセルがゴールのセルだった。
			//todo:本来ここから、スタートからゴールまでのノードを返す処理が入る。
			MessageBoxA(nullptr, "経路探索完了！！", "NaviMesh::AStar", MB_OK);
			return reserchCell;
		}
		else {
			//ゴールセルではない。
			//調査中のセルと隣接しているセルのリスト。
			std::vector<cell*> linkNodeList;
			//隣接セルを調べてリストに積む。
			for (int linkCellCount = 0; linkCellCount < 3; linkCellCount++) {
				if (reserchCell->m_linkCell[linkCellCount] != nullptr) {
					linkNodeList.push_back(reserchCell->m_linkCell[linkCellCount]);
				}
			}
			//隣接セルを調査する必要があるのかの識別を行う。
			for (auto& linkCell : linkNodeList) {
				float newCost = linkNodeList.back()->costFromStart + ClacTraverseCost(reserchCell, linkCell);
				//クローズセルに積まれているか検索する。
				auto closeListPtr = std::find(m_closeCellList.begin(), m_closeCellList.end(), linkCell);
				//調査する必要があるかの識別。
				if (closeListPtr != m_closeCellList.end() /*&& linkNodeList.back()->costFromStart <= newCost*/) {
					//クローズセルでコストも改善されないので、こいつはスキップ。
					continue;
				}
				else {
					//新しいセルもしくは、改善されたセルなので情報を保存していく。
					linkCell->m_parent = reserchCell;
					linkCell->costFromStart = newCost;
					linkCell->costToEnd = CalcCellToTargetCell(linkCell, m_goalCell);
					linkCell->totalCost = linkCell->costFromStart + linkCell->costToEnd;
					//調べたセルをオープンセルに変更する。
					moveCellList(m_openCellList, linkCell, NaviMesh::EnOpenList);
				}//新しく隣接していたセルの調査とパラメーター代入。
			}//隣接していたセルの調査とパラメーター代入が終了。
		}//現在調査しているリサーチセルの調査が終了。
		//調査終わったのでクローズリストに積む。
		moveCellList(m_closeCellList, reserchCell, NaviMesh::EnCloseList);
	}//オープンリストが0になった。ゴールまでの経路なし。
	MessageBoxA(nullptr, "経路の検索に失敗しました。", "NaviMesh::AStar", MB_OK);
	return nullptr;
}

NaviMesh::Cell* AStar::Search(Vector3& start, Vector3& goal, std::vector<cell>& cells)
{
	CreateCellList(start, goal, cells);
	cell* goalCellNode = CreateNode();
	return goalCellNode;
}


