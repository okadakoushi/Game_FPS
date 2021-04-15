#pragma once
#include "ICollider.h"

class CapsuleCollider : public ICollider
{
public:
	/// <summary>
	/// �쐬�B
	/// </summary>
	/// <param name="radius">���a�B</param>
	/// <param name="height">�����B</param>
	void Create(float radius, float height)
	{
		m_shape = std::make_unique<btCapsuleShape>(radius, height);
		m_radius = radius;
		m_height = height;
	}
	btCollisionShape* GetBody() const override
	{
		return m_shape.get();
	}
	/// <summary>
	/// ���a���擾�B
	/// </summary>
	/// <returns></returns>
	float GetRadius() const
	{
		return m_radius;;
	}
	/// <summary>
	/// �������擾�B
	/// </summary>
	/// <returns></returns>
	float GetHeight() const
	{
		return m_height;
	}
private:
	std::unique_ptr<btCapsuleShape> m_shape;	//�^
	float m_radius = 0.0f;						//���a�B
	float m_height = 0.0f;						//�����B
}; 

