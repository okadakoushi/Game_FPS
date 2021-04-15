#include "stdafx.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider() :
	shape(NULL)
{
}

SphereCollider::~SphereCollider()
{
	delete shape;
}

void SphereCollider::Create(const float radius)
{
	shape = new btSphereShape(radius);
}
