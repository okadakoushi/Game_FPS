#include "stdafx.h"
#include "PhysicsStaticObject.h"
#include "PhysicsObjectBase.h"
#include "SrcFile/Physics/CollisionAttr.h"

PhysicsStaticObject::PhysicsStaticObject()
{
}

void PhysicsStaticObject::Release()
{
	PhysicObj().RemoveRigidBody(m_rigidBody);
}

void PhysicsStaticObject::CreateCommon(Vector3 pos, Quaternion rot)
{
	RigidBodyInfo rbInfo;
	rbInfo.collider = m_collider.get();
	rbInfo.mass = 0.0f;
	rbInfo.pos = pos;
	rbInfo.rot = rot;
	m_rigidBody.Create(rbInfo);
	PhysicObj().AddRigidBody(m_rigidBody);
}
