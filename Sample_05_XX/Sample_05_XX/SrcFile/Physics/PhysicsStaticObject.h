#pragma once

#include "PhysicsObjectBase.h"

class PhysicsStaticObject : public PhysicsObjectBase
{
public:
	~PhysicsStaticObject()
	{
		Release();
	}
	/// <summary>
	/// ����B
	/// </summary>
	void Release() override final;
	/// <summary>
	/// ���W�Ɖ�]��ݒ�B
	/// </summary>
	/// <param name="pos">�ʒu�B</param>
	/// <param name="rot">��]�B</param>
	void SetPositionAndRotation(const Vector3& pos, const Quaternion& rot)
	{
		m_rigidBody.SetPositionAndRotation(pos, rot);
	}
	/// <summary>
	/// ���̂��擾�B
	/// </summary>
	/// <returns></returns>
	RigidBody& GetRigidBody()
	{
		return m_rigidBody;
	}
private:
	/// <summary>
	/// �ÓI�����I�u�W�F�N�g�쐬�����̋��ʏ����B
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void CreateCommon(Vector3 pos, Quaternion rot) override final;
private:
	RigidBody m_rigidBody;		//���́B
};

