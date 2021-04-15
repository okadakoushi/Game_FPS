#include "stdafx.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider() :
	m_shape(NULL)
{
}

BoxCollider::~BoxCollider()
{
	delete m_shape;
}

void BoxCollider::Create(const Vector3& size)
{
	m_shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
}
