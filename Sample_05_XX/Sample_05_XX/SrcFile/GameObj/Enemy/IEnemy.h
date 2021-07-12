#pragma once

#include "SrcFile/GameObject/IGameObject.h"
#include "SrcFile/SkinModelRender.h"

class NaviMesh;

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
		EnEnemyState_Death,			//���S�B
		EnEnemyState_Num
	};
public:
	virtual ~Enemy() {};
	virtual bool Start() override = 0;
	virtual void Update() override = 0;
	/// <summary>
	/// �i�r���b�V����ݒ�B
	/// </summary>
	/// <param name="mesh"></param>
	void SetNaviMesh(NaviMesh* mesh)
	{
		m_naviMesh = mesh;
	}
protected:
	//��{Param�B
	EnEnemyState					m_enemyState = EnEnemyState_Wandering;	//�G�l�~�[�̃X�e�[�g�B
	SkinModelRender*				m_modelRender;							//���f�������_�[�B
	Vector3							m_pos = g_vec3Zero;						//�ʒu�B
	Quaternion						m_rot = g_quatIdentity;					//��]�B
	Vector3							m_scale = { 1.5f, 1.5f, 1.5f };			//�g��B
	const float						m_spped = 1.0f;							//�X�s�[�h�B
	//�o�H�T��Param�B
	NaviMesh*						m_naviMesh = nullptr;					//�i�r���b�V���B
	AStar							m_astar;								//�o�H�T���B
	NaviMesh::Cell*					m_nodeCell = nullptr;					//�m�[�h�Z���B
	bool							m_dirty = false;						//�X�V�K�v�H
	std::vector<NaviMesh::Cell*>	m_nodeList;								//�m�[�h���X�g�B		
	Vector3							m_targetPos = g_vec3Zero;				//�ړI�n�_�B
	Vector3							m_nextTarget = g_vec3Zero;				//���̖ړI�n�B
};