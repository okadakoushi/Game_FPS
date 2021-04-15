#pragma once
#include "ICollider.h"

/// <summary>
/// �{�b�N�X�R���C�_�[�B
/// </summary>
class BoxCollider : public ICollider
{
public:
	BoxCollider();
	~BoxCollider();
	/// <summary>
	/// �쐬�B
	/// </summary>
	void Create(const Vector3& size);
	/// <summary>
	/// �{�f�B���擾�B
	/// </summary>
	/// <returns></returns>
	btCollisionShape* GetBody() const override
	{
		return m_shape;
	}
private:
	btBoxShape*		m_shape;		//�{�b�N�X�̌^�B
};

