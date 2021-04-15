#pragma once
#include "ICollider.h"

class SphereCollider : public ICollider
{
public:
	SphereCollider();
	~SphereCollider();
	/// <summary>
	/// �쐬
	/// </summary>
	/// <param name="redius">���a�B</param>
	void Create(const float radius);
	btCollisionShape* GetBody() const override
	{
		return shape;
	}
private:
	btSphereShape* shape;	//�^�B
};

