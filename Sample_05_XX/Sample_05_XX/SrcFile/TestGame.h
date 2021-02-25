#pragma once

class NaviMesh;
class AStar;

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
	Vector3 m_start = { 0,0,0 };
	Vector3 m_end = { 200,0,0 };
};

