#include "stdafx.h"
#include "PhysicsDebugDraw.h"

void PhysicsDebugDraw::Init()
{
	m_lineDraw.Init();
}

void PhysicsDebugDraw::drawLine(const btVector3& btFrom, const btVector3& btTo, const btVector3& color)
{
	Vector3 from, to;
	from.Set(btFrom.x(), btFrom.y(), btFrom.z());
	to.Set(btTo.x(), btTo.y(), btTo.z());
	m_lineDraw.AddLine(from, to);
}


void PhysicsDebugDraw::EndDraw()
{
	m_lineDraw.Draw();
}

