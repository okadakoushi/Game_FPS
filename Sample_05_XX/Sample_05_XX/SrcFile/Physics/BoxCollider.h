#pragma once
#include "ICollider.h"

/// <summary>
/// ボックスコライダー。
/// </summary>
class BoxCollider : public ICollider
{
public:
	BoxCollider();
	~BoxCollider();
	/// <summary>
	/// 作成。
	/// </summary>
	void Create(const Vector3& size);
	/// <summary>
	/// ボディを取得。
	/// </summary>
	/// <returns></returns>
	btCollisionShape* GetBody() const override
	{
		return m_shape;
	}
private:
	btBoxShape*		m_shape;		//ボックスの型。
};

