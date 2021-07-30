#pragma once

//#include "SrcFile/GameObj/Enemy/RifleEnemy.h"
class RifleEnemy;

/// <summary>
/// �G�l�~�[�̃X�e�[�g���Ǘ�����C���^�[�t�F�C�X�N���X�B
/// </summary>
class IEnemyState
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	/// <param name="enemy">�G�l�~�[�B</param>
	IEnemyState(RifleEnemy* enemy)
		: m_enemy(enemy)
	{
	}
	virtual void Enter() = 0;
	virtual void Leave() = 0;
	virtual void Update() = 0;
	/// <summary>
	/// �O�t���[���ŃA�b�v�f�[�g���ꂽ�X�e�[�g���擾�B
	/// </summary>
	/// <returns></returns>
	IEnemyState* GetLasetUpdateState()
	{
		return m_lastUpdateState;
	}
protected:
	RifleEnemy* m_enemy = nullptr;
	IEnemyState* m_lastUpdateState = nullptr;	//�O�t���[���̃X�e�[�g�B
};

