#pragma once

#include "SrcFile/GameObject/IGameObject.h"
#include "SrcFile/SkinModelRender.h"

class NaviMesh;
class GamePlayer;

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
	/// <summary>
	/// �v���C���[��Find���Ď擾�B
	/// <para>������g�p���Ȃ��悤�ɁB</para>
	/// </summary>
	/// <returns></returns>
	GamePlayer* GetPlayerForUseFind() const 
	{
		return FindGO<GamePlayer>("Player");
	}
protected:
	//��{Param�B
	SkinModelRender*				m_modelRender;							//���f�������_�[�B
	Vector3							m_pos = {50.0f, 0.0f, 0.0f};			//�ʒu�B
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
	//
	GamePlayer*						m_player = nullptr;						//�v���C���[�B
};