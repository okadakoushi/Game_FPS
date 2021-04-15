#pragma once
#include "ICollider.h"

class SphereCollider : public ICollider
{
public:
	SphereCollider();
	~SphereCollider();
	/// <summary>
	/// ì¬
	/// </summary>
	/// <param name="redius">”¼ŒaB</param>
	void Create(const float radius);
	btCollisionShape* GetBody() const override
	{
		return shape;
	}
private:
	btSphereShape* shape;	//Œ^B
};

