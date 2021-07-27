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
protected:
	RifleEnemy* m_enemy = nullptr;
};

