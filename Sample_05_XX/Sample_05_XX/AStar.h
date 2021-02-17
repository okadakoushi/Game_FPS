#pragma once

#include "SrcFile/NaviMesh/NaviMesh.h"

/// <summary>
/// �o�H�T���p�̌v�Z���s���N���X�B
/// </summary>
class AStar
{
	using cell = NaviMesh::Cell;
	using openCellList = vector<cell*>;
	using closeCellList = vector<cell*>;
public:
	/// <summary>
	/// �X�^�[�g�ʒu����S�[���ʒu�܂ł̌o�H��T���B
	/// </summary>
	/// <param name="start">�X�^�[�g�ʒu�B</param>
	/// <param name="goal">�S�[���ʒu�B</param>
	/// <param name="cell">�Z���B</param>
	void AStarSearch(Vector3& start, Vector3& goal, NaviMesh::Cell& cell);
private:
	openCellList m_openCellList;		//�J���Ă�Z�����X�g�B
	closeCellList m_closeCellList;		//���Ă�Z�����X�g�B
};

