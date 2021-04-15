#pragma once

#include "SrcFile/Physics/CapsuleCollider.h"
#include "SrcFile/Physics/RigidBody.h"

/// <summary>
/// �L�����N�^�[�R���g���[���[�B
/// </summary>
class CharacterController
{
public:
	CharacterController() {};
	~CharacterController() {
		
	};
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="radius">�J�v�Z���R���C�_�[�̔��a�B</param>
	/// <param name="height">�J�v�Z���R���C�_�[�̍����B</param>
	/// <param name="position">�����ʒu�B</param>
	void Init(float radius, float height, const Vector3& position);
	/// <summary>
	/// ���s�B
	/// </summary>
	/// <param name="moveSpeed">�ړ����x�B</param>
	/// <param name="deltaTime">�o�ߎ��ԁB</param>
	/// <returns></returns>
	const Vector3& Execute(Vector3& moveSpeed, float deltaTime = GameTime().GetFrameDeltaTime());
	/// <summary>
	/// �ʒu���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// �ʒu��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �W�����v��������B
	/// </summary>
	/// <returns></returns>
	bool IsJump() const
	{
		return m_isJump;
	}
	/// <summary>
	/// �n�ʏ�ɂ��邩����B
	/// </summary>
	/// <returns></returns>
	bool IsOnGround() const
	{
		return m_isOnGround;
	}
	/// <summary>
	/// �R���C�_�[���擾�B
	/// </summary>
	/// <returns></returns>
	CapsuleCollider* GetCollider()
	{
		return &m_collider;
	}
	/// <summary>
	/// ���̂��擾�B
	/// </summary>
	/// <returns></returns>
	RigidBody* GetRigidBody()
	{
		return &m_rigidBody;
	}
	/// <summary>
	/// ���̂𕨗��G���W������폜�B
	/// </summary>
	void RemoveRigidBody();
private:
	bool			m_isInited = false;			//�������ς݃t���O�B
	Vector3			m_position = g_vec3Zero;	//���W�B
	bool			m_isJump = false;			//�W�����v�B
	bool			m_isOnGround = true;		//�n�ʂɂ���H
	CapsuleCollider	m_collider;					//�R���C�_�[�B
	float			m_radius = 0.0f;			//�J�v�Z���̔��a�B
	float			m_height = 0.0f;			//�J�v�Z���̍����B
	RigidBody		m_rigidBody;				//���́B
};

