#pragma once

class GameCamera;
class Rifle;
class Bullet;

#include "SrcFile/RayTestCallBack.h"

/// <summary>
/// �v���C���[�N���X�B
/// </summary>
class GamePlayer : public IGameObject
{
public:
	void OnDestroy() override;
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void PostUpdate() override;
	/// <summary>
	/// �ړ��B
	/// <para>�ړ��̓J��������ɍs����B</para>
	/// </summary>
	void Move();
	/// <summary>
	/// ��]�B
	/// <para>�s���ɂ��GameCamera���Ăяo���B</para>
	/// </summary>
	void Rotation();
	/// <summary>
	/// ���ˁI�I
	/// </summary>
	void Shot();
	/// <summary>
	/// �ʒu���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPos()const
	{
		return m_pos;
	}

	void GetRHandPos(Vector3& vec, Quaternion& qRot) const
	{
		vec = m_rHandBonePos;
		qRot = m_rHandBoneRot;
	}
	/// <summary>
	/// �����_�[�擾�B
	/// </summary>
	/// <returns></returns>
	SkinModelRender* GetRender()
	{
		return m_unityChan;
	}
	void SetPos(Vector3& pos)
	{
		m_pos = pos;
		m_cCon.SetPosition(m_pos);
	}
private:
	/// <summary>
	/// SkinModelRender�̃A�j���[�V�����i�s���I������^�C�~���O�ŌĂ΂�鏈���B
	/// </summary>
	/// <remarks>
	/// �A�j���[�V������i�s���āA���̃��[�J���s����X�V�������ƂŁA
	/// ���̍����v���O�����I�ɓ������K�v���������̂ŁA�{�֐���ǉ��B
	/// </remarks>
	void OnPostAnimationProgress();
private:
	enum EnPlayerState {
		EnPlayerState_Idle,		//�����Ȃ��B
		EnPlayerState_Walk,		//�����B
		EnPlayerState_Run,		//����B
		EnPlayerState_Buck,		//��ށB
		EnPlayerState_Shot,		//�ˌ�
		EnPlayerState_Reload,	//�����[�h�B
		EnPlayerState_Deth,		//���S�B
		EnPlayerState_Num		//���B
	};
	EnPlayerState m_playerState = EnPlayerState_Idle;
	SkinModelRender* m_unityChan = nullptr;		//���j�e�B�[�����B
	CharacterController m_cCon;					//�L�����R���B
	SpriteRender* m_reticle = nullptr;			//���e�B�N���B
	Vector3 m_pos = g_vec3Zero;					//�ʒu�B
	Quaternion m_rot = g_quatIdentity;			//��]�B
	Rifle* m_wepon = nullptr;					//����B
	Bone* m_spineBone = nullptr;				//���̃{�[���B
	Bone* m_rHandBone = nullptr;				//�E��̃{�[���B
	Bone* m_headBone = nullptr;					//���{�[���B
	Vector3 m_headPos = g_vec3Zero;				//���̈ʒu�B
	Vector3 m_rHandBonePos = g_vec3Zero;
	Quaternion m_rHandBoneRot = g_quatIdentity;	
	Vector3 m_move = g_vec3Zero;				//�ړ��B
	float m_speed = 100.0f;						//�ړ����x�B
	float const m_JUMPFORSE = 520.0f;			//�W�����v�B
	float const m_GRAVITY = 20.0f;				//�d�́B
	const float fixYToEyePos = 110.0f;			//���_���W�ɕς���Y���C���B
	int m_flame = 0;
	GameCamera* m_camera = nullptr;				//�J�����B
	myEngine::Effect* m_effect = nullptr;		//�G�t�F�N�g�B
	const float RAY_RANGE = 2000.0f;
	int m_hp = 150;								//HP�B
};

