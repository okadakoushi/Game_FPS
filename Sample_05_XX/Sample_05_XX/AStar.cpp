#include "stdafx.h"
#include "AStar.h"

namespace {
	struct Line {
		Vector3 start;
		Vector3 end;
	};
	float CalcLen(Vector3& v0, Vector3& v1)
	{
		float len;
		len = sqrt((v0.x - v1.x) * (v0.x - v1.x) + (v0.y - v1.y) * (v0.y - v1.y) + (v0.z - v1.z) * (v0.z - v1.z));
		return len;
	}
	/// <summary>
	/// ラインとラインのXZ平面での当たり判定。
	/// </summary>
	/// <param name="line0">ライン０</param>
	/// <param name="line1">ライン１</param>
	/// <returns>trueが返ってくると当たっている</returns>
	bool IntersectLineToLineXZ(Line& line0, Line& line1)
	{
		//ライン0にXZ平面で直交する単位ベクトルを求める。
		//XZ平面での判定。
		line0.start.y = 0;
		line0.end.y = 0;
		line1.start.y = 0;
		line1.end.y = 0;
		//ライン0の始点から終点。
		Vector3 nom = line0.end - line0.start;
		//ラインの法線を求める。
		nom.Cross({ 0,1,0 });
		nom.Normalize();
		
		//ライン0を含む無限線分との交差判定。
		//ライン0の始点からライン1の終点。
		Vector3 L0StoL1EN = line1.end - line0.start;
		//ライン0の始点からライン1の始点。
		Vector3 L0StoL1SN = line1.start - line0.start;
		//L1の終点と法線の内積。
		float startDot = L0StoL1EN.Dot(nom);
		//L1の始点と法線の内積。
		float endDot = L0StoL1SN.Dot(nom);
		//交差しているなら違う方向。
		float dot = startDot * endDot;
		if (dot < 0.0f) {
			//交差しているので交点を求めていく。
			//辺の絶対値求める。
			float EndLen = fabsf(endDot);
			float StartLen = fabsf(startDot);
			if ((StartLen + EndLen) > 0.0f) {
				//交点の位置を求める。
				//辺の割合を求める。
				float t = EndLen / (StartLen + EndLen);
				//終点から始点。
				Vector3 EtoS = line1.start - line1.end;
				//終点から交点。
				Vector3 EtoHit = EtoS * t;
				//交点。
				Vector3 hitPos = line1.end + EtoHit;
			
				auto len = CalcLen(line0.end, line0.start);
				auto len1 = CalcLen(hitPos, line0.start);
				auto len2 = CalcLen(line0.end ,hitPos);
				
				if (len == len1 + len2) {
					return true;
				}

				//if ((VSL < hitL && hitL < VEL) || (VEL < hitL && hitL < VSL)) {
				//	//片方が小さくて、片方が大きい。
				//}
			}
			//StartLenとEndLenが0よりも小さくなったおかしい。
			return false;
		}
		//交点なし。
		return false;
	}
}
void AStar::CreateCellList(Vector3& start, Vector3& goal, std::vector<cell>& cells)
{
#if 0
	//交差ラインテスト。
	Line line0, line1;
	line0.start = { 0, 500, 0 };
	line0.end = { 200, 500, 0 };
	line1.start = { 100, 500, -100 };
	line1.end = { 100, 500, 100 };
	//判定。
	if (IntersectLineToLineXZ(line0, line1) == true) {
		printf("交差していました。\n");
	}
#else
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
#endif
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
			//MessageBoxA(nullptr, "経路探索完了！！", "NaviMesh::AStar", MB_OK);

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

void AStar::Smoothing(std::vector<cell*>& nodeCellList)
{
//#ifdef NAV_DEBUG
	//ナビゲーションデバッグ用。
	//最初のセル。
	auto* baseCell = nodeCellList.back();
	//隣接セルとはスムージング処理を行わなくていいので、隣接セルの次のセルから当たり判定調査を行う。
	auto* reserchCell = baseCell->m_parent->m_parent;
	while (reserchCell != nullptr) {
		//ゴールに到着するまで、スムージング処理を行う。
		//ベースのセルの中心から、調査セルの中心に向かう線分を求める。
		Line BaseLine;
		BaseLine.start = baseCell->m_CenterPos;
		BaseLine.end = reserchCell->m_CenterPos;
		//セルを構成する３本の線分を求める。
		Line line[3];
		line[0].start = reserchCell->pos[0];
		line[0].end = reserchCell->pos[1];
		line[1].start = reserchCell->pos[1];
		line[1].end = reserchCell->pos[2];
		line[2].start = reserchCell->pos[2];
		line[2].end = reserchCell->pos[0];
		//ここから当たり判定を行う。
		bool hantei = false;
		for (int lineNo = 0; lineNo < 3; lineNo++) {
			hantei = IntersectLineToLineXZ(BaseLine, line[lineNo]);
			if (hantei == true) {
				//衝突したのでノードリストから該当セルを削除。
				auto it = std::find(nodeCellList.begin(), nodeCellList.end(), reserchCell);
				nodeCellList.erase(it);
				//リサーチセルを更新。
				reserchCell = reserchCell->m_parent;
				break;
			}
		}

		if (hantei == false) {
			//衝突しているセルは存在していないので、ベースセルを更新する。
			baseCell = reserchCell;
			//リサーチセルも更新する。
			if (reserchCell->m_parent != nullptr) {
				reserchCell = baseCell->m_parent->m_parent;
			}
			else {
				//これ以上リサーチセルは存在しないため終了。
				break;
			}

		}
	}

	//ゴールまでのノードを再構成する。
	for (int nodeNo = 0; nodeNo < nodeCellList.size(); nodeNo++) {
		if (nodeCellList[nodeNo]->costToEnd != 0.0f) {
			//ゴールじゃないなら。親ノードをつなぎなおす。
			nodeCellList[nodeNo]->m_parent = nodeCellList[nodeNo + 1];
		}
	}
//#endif
}



std::vector<NaviMesh::Cell*> AStar::Search(Vector3& start, Vector3& goal, std::vector<cell>& cells)
{
	//セルリストの初期化。
	CreateCellList(start, goal, cells);
	//ノードを作成。
	cell* goalCellNode = CreateNode();

	//ノード情報を管理しやすいように、リストに変換。
	//スタート位置から近い順。
	std::vector<cell*> m_nodeCellList;
	//現在のセル。
	cell* currentNode = goalCellNode;
	while (currentNode != nullptr)
	{
		//リストに積む。
		m_nodeCellList.insert(m_nodeCellList.begin(), currentNode);
		currentNode = currentNode->m_parent;
	}

	//スムージング。
	Smoothing(m_nodeCellList);
	return m_nodeCellList;
}


