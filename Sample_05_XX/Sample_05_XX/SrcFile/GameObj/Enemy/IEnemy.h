#pragma once

#include "SrcFile/GameObject/IGameObject.h"
#include "SrcFile/SkinModelRender.h"
#include "SrcFile/GameObj/GamePlayer.h"

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
	NaviMesh*						m_naviMesh = nullptr;
	//
	GamePlayer*						m_player = nullptr;						//�v���C���[�B
};