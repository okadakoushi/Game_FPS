#pragma once
#include "NaviMesh/NaviMesh.h"

/// <summary>
/// �e�X�g�p�Q�[���N���X�B
/// #ifdef GameTest�ŃC���X�^���X���B
/// </summary>
class TestGame : public IGameObject
{
public:
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
private:
	//NaviMesh m_naviMeshBase;
	NaviMesh m_naviMesh;
};

