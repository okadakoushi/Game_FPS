#pragma once
#include "ICollider.h"

class CapsuleCollider : public ICollider
{
public:
	/// <summary>
	/// ì¬B
	/// </summary>
	/// <param name="radius">”¼ŒaB</param>
	/// <param name="height">‚‚³B</param>
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
	/// ”¼Œa‚ğæ“¾B
	/// </summary>
	/// <returns></returns>
	float GetRadius() const
	{
		return m_radius;;
	}
	/// <summary>
	/// ‚‚³‚ğæ“¾B
	/// </summary>
	/// <returns></returns>
	float GetHeight() const
	{
		return m_height;
	}
private:
	std::unique_ptr<btCapsuleShape> m_shape;	//Œ^
	float m_radius = 0.0f;						//”¼ŒaB
	float m_height = 0.0f;						//‚‚³B
}; 

