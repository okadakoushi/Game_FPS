#pragma once
#include "ICollider.h"

class CapsuleCollider : public ICollider
{
public:
	/// <summary>
	/// 作成。
	/// </summary>
	/// <param name="radius">半径。</param>
	/// <param name="height">高さ。</param>
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
	/// 半径を取得。
	/// </summary>
	/// <returns></returns>
	float GetRadius() const
	{
		return m_radius;;
	}
	/// <summary>
	/// 高さを取得。
	/// </summary>
	/// <returns></returns>
	float GetHeight() const
	{
		return m_height;
	}
private:
	std::unique_ptr<btCapsuleShape> m_shape;	//型
	float m_radius = 0.0f;						//半径。
	float m_height = 0.0f;						//高さ。
}; 

