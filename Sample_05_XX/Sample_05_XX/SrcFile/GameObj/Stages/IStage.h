#pragma once

class NaviMesh;
class StageGenerator;
class GamePlayer;

#include "SrcFile/GameObject/IGameObject.h"

/// <summary>
/// �X�e�[�W�̃C���^�[�t�F�C�X�N���X�B
/// </summary>
/// <remarks>
/// �X�e�[�W�����ɂ��ăR�[�h�̏d��������
/// ������|�����[�t�B�Y���ŉ������邽�ߍ쐬�B
/// </remarks>
class IStage : public IGameObject
{
public:
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	virtual bool Start() override = 0;
	/// <summary>
	/// �X�V�B
	/// </summary>
	virtual void Update() override = 0;
	/// <summary>
	/// �i�r�Q�[�V�������b�V����ݒ�B
	/// </summary>
	/// <param name="naviMesh"></param>
	void SetNaviMesh(NaviMesh* naviMesh)
	{
		m_naviMesh = naviMesh;
	}
	/// <summary>
	/// �X�e�[�W�W�F�l���[�^�[��ݒ�B
	/// </summary>
	/// <param name="stageGenerator"></param>
	void SetStageGenerator(StageGenerator* stageGenerator)
	{
		m_stageGenerator = stageGenerator;
	}
	/// <summary>
	/// �v���C���[��ݒ�B
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(GamePlayer* player)
	{
		m_player = player;
	}
protected:
	NaviMesh* m_naviMesh = nullptr;
	StageGenerator* m_stageGenerator = nullptr;
	GamePlayer* m_player = nullptr;
	int m_currentEnemyCount = 0;
};