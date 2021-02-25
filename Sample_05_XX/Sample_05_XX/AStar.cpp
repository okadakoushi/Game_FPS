#include "stdafx.h"
#include "AStar.h"

void AStar::CreateCellList(Vector3& start, Vector3& goal, std::vector<cell>& cells)
{
	//���X�g���N���A�B
	m_openCellList.clear();
	m_closeCellList.clear();
	m_noneCellList.clear();
	//�Z�������X�g�ɐς�ł����B
	for (int cellCount = 0; cellCount < cells.size(); cellCount++) {
		//�Z���̃p�����[�^�[���������B
		cells[cellCount].costFromStart = 0.0f;
		cells[cellCount].costToEnd = 0.0f;
		cells[cellCount].m_parent = nullptr;
		cells[cellCount].totalCost = 0.0f;
		//�Ȃɂ�����ĂȂ��Z�����X�g�ɐςށB
		m_noneCellList.push_back(&cells[cellCount]);
	}
	//�ŏ������B
	float startMin, goalMin;
	startMin = FLT_MAX;
	goalMin = FLT_MAX;
	//�ŏ������������Z���ԍ��B
	int startCellNo, goalCellNo;
	startCellNo = -1;
	goalCellNo = -1;
	//�J���Z�������߂�B
	for (int cellCount = 0; cellCount < cells.size(); cellCount++) {
		//�S�[���n�_����A�P�ԋ������߂��Z�������߂�B
		Vector3 goalToCenter = cells[cellCount].m_CenterPos - goal;
		float dist = goalToCenter.Length();
		if (dist < goalMin) {
			//�X�V�B
			goalMin = dist;
			goalCellNo = cellCount;
			m_goalCell = &cells[cellCount];
		}
		//�X�^�[�g�n�_����A��ԋ������߂��Z�������߂�B
		Vector3 startToCenter = cells[cellCount].m_CenterPos - start;
		dist = startToCenter.Length();
		if (dist < startMin) {
			//�X�V�B
			startMin = dist;
			startCellNo = cellCount;
			m_startCell = &cells[cellCount];
		}
	}

	//�R�X�g�v�Z�B�X�^�[�g�ʒu�̃R�X�g���ĕʂɂ���񂩂��H
	Vector3 startToGoal = m_goalCell->m_CenterPos - m_startCell->m_CenterPos;
	float dist = startToGoal.Length();
	m_startCell->costToEnd = dist;
	//�n�_�̓I�[�v�������B
	moveCellList(m_openCellList, m_startCell, NaviMesh::EnOpenList);
}

float AStar::ClacTraverseCost(cell* node, cell* reserchNode)
{
	//�R�X�g�B
	float cost = 0.0f;
	//�O�̃m�[�h����̃R�X�g���v�Z�B
	Vector3 dist = reserchNode->m_CenterPos - node->m_CenterPos;
	return cost = dist.Length();
}

NaviMesh::Cell* AStar::CreateNode()
{
	while (m_openCellList.size() != 0) {
		//�����ΏۃZ���B
		cell* reserchCell = nullptr;
		//�R�X�g�B
		float cost = FLT_MAX;
		//�J����Ă�Z���̂����A��ԃR�X�g���Ⴂ���̂̂���������ΏۃZ���ƂȂ�B
		for (auto cell : m_openCellList) {//todo:���̃��[�v�̏������̕����Y��Ȃ̂ł́H
			if (cell->costToEnd < cost) {
				//�R�X�g���Ⴂ�̂Œ����Z����ς���B
				reserchCell = cell;
				cost = reserchCell->costToEnd;
			}
		}//reserchCell asking completed.

		//�����Z�����܂����̂Ōo�H�̃m�[�h���쐬���Ă����B
		if (reserchCell == m_goalCell) {
			//�����̃Z�����S�[���̃Z���������B
			//todo:�{����������A�X�^�[�g����S�[���܂ł̃m�[�h��Ԃ�����������B
			MessageBoxA(nullptr, "�o�H�T�������I�I", "NaviMesh::AStar", MB_OK);
			return reserchCell;
		}
		else {
			//�S�[���Z���ł͂Ȃ��B
			//�������̃Z���Ɨאڂ��Ă���Z���̃��X�g�B
			std::vector<cell*> linkNodeList;
			//�אڃZ���𒲂ׂă��X�g�ɐςށB
			for (int linkCellCount = 0; linkCellCount < 3; linkCellCount++) {
				if (reserchCell->m_linkCell[linkCellCount] != nullptr) {
					linkNodeList.push_back(reserchCell->m_linkCell[linkCellCount]);
				}
			}
			//�אڃZ���𒲍�����K�v������̂��̎��ʂ��s���B
			for (auto& linkCell : linkNodeList) {
				float newCost = linkNodeList.back()->costFromStart + ClacTraverseCost(reserchCell, linkCell);
				//�N���[�Y�Z���ɐς܂�Ă��邩��������B
				auto closeListPtr = std::find(m_closeCellList.begin(), m_closeCellList.end(), linkCell);
				//��������K�v�����邩�̎��ʁB
				if (closeListPtr != m_closeCellList.end() /*&& linkNodeList.back()->costFromStart <= newCost*/) {
					//�N���[�Y�Z���ŃR�X�g�����P����Ȃ��̂ŁA�����̓X�L�b�v�B
					continue;
				}
				else {
					//�V�����Z���������́A���P���ꂽ�Z���Ȃ̂ŏ���ۑ����Ă����B
					linkCell->m_parent = reserchCell;
					linkCell->costFromStart = newCost;
					linkCell->costToEnd = CalcCellToTargetCell(linkCell, m_goalCell);
					linkCell->totalCost = linkCell->costFromStart + linkCell->costToEnd;
					//���ׂ��Z�����I�[�v���Z���ɕύX����B
					moveCellList(m_openCellList, linkCell, NaviMesh::EnOpenList);
				}//�V�����אڂ��Ă����Z���̒����ƃp�����[�^�[����B
			}//�אڂ��Ă����Z���̒����ƃp�����[�^�[������I���B
		}//���ݒ������Ă��郊�T�[�`�Z���̒������I���B
		//�����I������̂ŃN���[�Y���X�g�ɐςށB
		moveCellList(m_closeCellList, reserchCell, NaviMesh::EnCloseList);
	}//�I�[�v�����X�g��0�ɂȂ����B�S�[���܂ł̌o�H�Ȃ��B
	MessageBoxA(nullptr, "�o�H�̌����Ɏ��s���܂����B", "NaviMesh::AStar", MB_OK);
	return nullptr;
}

NaviMesh::Cell* AStar::Search(Vector3& start, Vector3& goal, std::vector<cell>& cells)
{
	CreateCellList(start, goal, cells);
	cell* goalCellNode = CreateNode();
	return goalCellNode;
}


