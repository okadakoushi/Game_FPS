#pragma once

#include "SrcFile/GameObject/IGameObject.h"
#include "SrcFile/SkinModelRender.h"

/// <summary>
/// �G�l�~�[�̃C���^�[�t�F�C�X�B
/// </summary>
/// <remarks>
/// factoryMethod�g����Enemy�̎�ނƂ������_���z�u��������ˁB
/// templateMethod�p�^�[���̓��[�U�[���_����݂Ă���Ȃ��A�Ȃ�Ȃ�ז��B
/// �����o�p��(state,render)�͂ǂ��Ȃ񂾂낤�Befective�ɂ́uprotected�̓J�v�Z������i�߂���̂ł͂Ȃ��v�Ƃ��邯�ǁB
/// </remarks>
class Enemy : public IGameObject{
public:
	enum EnEnemyState {
		EnEnemyState_Wandering,		//�p�j�B
		EnEnemyState_Tracking,		//�ǐՁB
		EnEnemyState_Attack,		//�U���B
		EnEnemyState_AttackWait,	//�U���ҋ@���ԁB
		EnEnemyState_Num
	};
public:
	virtual ~Enemy() {};
	virtual bool Start() override = 0;
	virtual void Update() override = 0;
};