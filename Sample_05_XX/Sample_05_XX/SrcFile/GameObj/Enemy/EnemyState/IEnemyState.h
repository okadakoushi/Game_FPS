#pragma once

#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

/// <summary>
/// �G�l�~�[�̃X�e�[�g���Ǘ�����C���^�[�t�F�C�X�N���X�B
/// </summary>
class IEnemyState : public RifleEnemy
{
public:
	virtual void Enter() = 0;
	virtual void Leave() = 0;
	virtual void Update() = 0;
};

